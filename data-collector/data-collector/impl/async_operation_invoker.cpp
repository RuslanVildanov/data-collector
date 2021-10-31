#include "async_operation_invoker.h"

#include <QDebug>
#include <QThread>
#include <QMetaObject>
#include "../i_operation.h"

AsyncOperation::AsyncOperation(
        qint64 id,
        QSharedPointer<IOperation> operation) noexcept :
    IOperation(),
    m_operation(operation),
    m_id(id)
{
    Q_ASSERT_X(m_operation, "check input arg", "arg is empty");
    if (m_operation)
    {
        connect(m_operation.data(), &IOperation::finished,
                this, &AsyncOperation::onOperationFinished);
    }
}

void AsyncOperation::start() noexcept
{
    if (!m_operation)
    {
        return;
    }
    m_operation->start();
}

void AsyncOperation::startAsync()
{
    qInfo() << "AsyncOperation::startAsync: " << m_id << " async thread: " << QThread::currentThread();

    start();
}

void AsyncOperation::onOperationFinished(bool ok, const QString &msg)
{
    qInfo() << "AsyncOperation::onOperationFinished" << m_id << " async thread: " << QThread::currentThread();

    disconnect(m_operation.data(), &IOperation::finished,
               this, &AsyncOperation::onOperationFinished);
    emit asyncFinished(m_id, ok, msg);
}

AsyncOperationInvoker::AsyncOperationInvoker() noexcept :
    IOperationInvoker()
{
    m_thread = new QThread();
    if (m_thread)
    {
        connect(m_thread.data(), &QThread::finished, m_thread.data(), &QThread::deleteLater);
        m_thread->start();
    }
    else
    {
        qWarning() << "Memory allocation failure" << __FILE__ << __LINE__;
    }
}

AsyncOperationInvoker::~AsyncOperationInvoker() noexcept
{
    if (m_thread)
    {
        m_thread->quit();
    }
}

void AsyncOperationInvoker::start(QSharedPointer<IOperation> operation) noexcept
{
    Q_ASSERT_X(operation, "check input arg", "arg is empty");
    if (!operation)
    {
        return;
    }

    if (!m_thread)
    {
        operation->finished(false, tr("Thread is empty"));
        return;
    }

    QSharedPointer<AsyncOperation> o;
    {
        QMutexLocker l(&m_mutex);
        m_executed_operation++;
        o = QSharedPointer<AsyncOperation>(
                    new AsyncOperation(m_executed_operation, operation),
                    &QObject::deleteLater);
        if (!o)
        {
            qWarning() << "Memory allocation failure" << __FILE__ << __LINE__;
        }
        else
        {
            connect(o.data(), &AsyncOperation::asyncFinished,
                    this, &AsyncOperationInvoker::onAsyncOperationFinished);
            operation->moveToThread(m_thread);
            o->moveToThread(m_thread);
            m_started_operations.insert(m_executed_operation, o);
        }
    }
    if (o)
    {
        const bool ok = QMetaObject::invokeMethod(
                    o.data(),
                    "startAsync",
                    Qt::QueuedConnection);
        Q_ASSERT_X(ok, "check slot method", "reqired method startAsync is not exist");
    }
}

void AsyncOperationInvoker::onAsyncOperationFinished(qint64 id, bool ok, const QString &msg)
{
    Q_UNUSED(ok)
    Q_UNUSED(msg)
    QMutexLocker l(&m_mutex);

    QSharedPointer<AsyncOperation> o = qSharedPointerObjectCast<AsyncOperation>(
                m_started_operations.value(id));
    Q_ASSERT_X(o, "check started operation", "AsyncOperation is null");
    if (o)
    {
        disconnect(o.data(), &AsyncOperation::asyncFinished,
                   this, &AsyncOperationInvoker::onAsyncOperationFinished);
    }

    m_started_operations.remove(id);
}
