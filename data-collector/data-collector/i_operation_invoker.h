#ifndef I_OPERATION_INVOKER_H
#define I_OPERATION_INVOKER_H

#include <QObject>
#include <QSharedPointer>

class IOperation;

class IOperationInvoker : public QObject
{
    Q_OBJECT

public:
    virtual void start(QSharedPointer<IOperation> operation) noexcept = 0;

protected:
    IOperationInvoker() noexcept :
        QObject()
    {}

};

#endif // I_OPERATION_INVOKER_H
