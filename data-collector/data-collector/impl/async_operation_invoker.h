#ifndef ASYNCOPERATIONINVOKER_H
#define ASYNCOPERATIONINVOKER_H

#include <QList>
#include <QMutex>
#include <QPointer>
#include "../i_operation.h"
#include "../i_operation_invoker.h"

class QThread;

class AsyncOperation : public IOperation
{
    Q_OBJECT

    QSharedPointer<IOperation> m_operation;
    const qint64 m_id;

public:
    AsyncOperation(qint64 id, QSharedPointer<IOperation> operation) noexcept;

    // IOperation interface
    void start() noexcept final;

    Q_INVOKABLE void startAsync();

signals:
    void asyncFinished(qint64 id, bool ok, const QString &msg);

private slots:
    void onOperationFinished(bool ok, const QString &msg);

};

class AsyncOperationInvoker : public IOperationInvoker
{
    Q_OBJECT

    QPointer<QThread> m_thread;

    QMutex m_mutex;
    QHash<qint64, QSharedPointer<IOperation> > m_started_operations;
    qint64 m_executed_operation = 0;

public:
    AsyncOperationInvoker() noexcept;
    ~AsyncOperationInvoker() noexcept;

    // IOperationInvoker interface
    void start(QSharedPointer<IOperation> operation) noexcept final;

private slots:
    void onAsyncOperationFinished(qint64 id, bool ok, const QString &msg);

};

#endif // ASYNCOPERATIONINVOKER_H
