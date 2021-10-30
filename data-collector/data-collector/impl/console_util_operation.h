#ifndef CONSOLEUTILOPERATION_H
#define CONSOLEUTILOPERATION_H

#include <QString>
#include <QStringList>
#include "../i_operation.h"

class ConsoleUtilOperation : public IOperation
{
    Q_OBJECT

    const QString m_prog;
    const QStringList m_args;
    const QString m_target_dir;

public:
    ConsoleUtilOperation(
            const QString &prog,
            const QStringList &args,
            const QString &target_dir) noexcept;

    // IOperation interface
    void start() noexcept final;

};

#endif // CONSOLEUTILOPERATION_H
