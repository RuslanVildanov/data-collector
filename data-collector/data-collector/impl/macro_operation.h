#ifndef MACROOPERATION_H
#define MACROOPERATION_H

#include <QList>
#include <QString>
#include <QSharedPointer>
#include "../i_operation.h"

class MacroOperation : public IOperation
{
    Q_OBJECT

    struct FinishedOperation
    {
        QString msg;
        bool ok = false;
    };

    QList<QSharedPointer<IOperation> > m_operations;
    QList<FinishedOperation> m_finished_operations;
    QSharedPointer<IOperation> m_current_operation;
    const qint32 m_total_operations;

public:
    explicit MacroOperation(
            const QList<QSharedPointer<IOperation> > &operations) noexcept;

    // IOperation interface
    void start() noexcept final;

private slots:
    void onOperationProgress(qint8 value, const QString &text);
    void onOperationFinished(bool ok, const QString &msg);

private:
    void startNextOperation() noexcept;

    Q_DISABLE_COPY(MacroOperation)
};

#endif // MACROOPERATION_H
