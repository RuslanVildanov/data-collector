#include "macro_operation.h"

#include <QDebug>
#include <QThread>

MacroOperation::MacroOperation(const QList<QSharedPointer<IOperation> > &operations) noexcept :
    IOperation(),
    m_operations(operations),
    m_total_operations(operations.size())
{
}

void MacroOperation::start() noexcept
{
    startNextOperation();
}

void MacroOperation::onOperationProgress(qint8 value, const QString &text)
{
    // Прогресс данной операции исчесляется от 0 до MAX_OPERATION_PROGRESS,
    // а так же прогресс каждой запущеной операции так же исчисляется,
    // поэтому нужно произвести пересчёт текущего прогресса
    qint32 finished_operations = m_finished_operations.size();
    qint8 converted_value = static_cast<qint8>(
                static_cast<qreal>(finished_operations*value)/static_cast<qreal>(m_total_operations));
    emit progress(converted_value, text);
}

void MacroOperation::onOperationFinished(bool ok, const QString &msg)
{
    disconnect(m_current_operation.data(), &IOperation::progress,
               this, &MacroOperation::onOperationProgress);
    disconnect(m_current_operation.data(), &IOperation::finished,
               this, &MacroOperation::onOperationFinished);
    FinishedOperation s;
    s.ok = ok;
    s.msg = msg;
    m_finished_operations.append(s);
    startNextOperation();
}

void MacroOperation::startNextOperation() noexcept
{
//    qInfo() << "MacroOperation::startNextOperation" << m_operations.size() << " macro operation thread: " << QThread::currentThread();;

    if (m_operations.isEmpty())
    {
        bool ok = true;
        QString msg;
        for(const auto &result : m_finished_operations)
        {
            if (!result.ok)
            {
                if (!msg.isEmpty())
                {
                    msg += '\n';
                }
                msg += result.msg;
                ok = false;
            }
        }
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(ok, msg);
        return;
    }
    m_current_operation = m_operations.takeFirst();
    Q_ASSERT_X(m_current_operation, "trying to start next operation", "incorrect input null operation");
    if (!m_current_operation)
    {
        startNextOperation();
        return;
    }

    connect(m_current_operation.data(), &IOperation::progress,
            this, &MacroOperation::onOperationProgress);
    connect(m_current_operation.data(), &IOperation::finished,
            this, &MacroOperation::onOperationFinished);
    m_current_operation->start();
}
