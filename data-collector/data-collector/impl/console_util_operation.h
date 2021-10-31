#ifndef CONSOLEUTILOPERATION_H
#define CONSOLEUTILOPERATION_H

#include <QString>
#include <QStringList>
#include "../i_operation.h"

class ConsoleUtilOperation : public IOperation
{
    Q_OBJECT

    const QString m_prog;
    const QString m_working_dir;
    const QStringList m_args;
    const QString m_output_file_path;

public:
    ConsoleUtilOperation(
            const QString &prog,
            const QString &working_dir,
            const QStringList &args,
            const QString &output_file_path) noexcept;

    // IOperation interface
    void start() noexcept final;

    Q_DISABLE_COPY(ConsoleUtilOperation)
};

#endif // CONSOLEUTILOPERATION_H
