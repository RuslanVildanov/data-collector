#ifndef IOPERATION_H
#define IOPERATION_H

#include <QObject>

static qint8 MAX_OPERATION_PROGRESS = 100;

class QString;

class IOperation : public QObject
{
    Q_OBJECT

public:
    virtual void start() noexcept = 0;

signals:
    void progress(qint8 value, const QString &text);
    void finished(bool ok, const QString &msg);

protected:
    IOperation() noexcept :
        QObject()
    {}

};

#endif // IOPERATION_H
