#include "console_util_operation.h"

ConsoleUtilOperation::ConsoleUtilOperation(
        const QString &prog,
        const QStringList &args,
        const QString &target_dir) noexcept :
    IOperation(),
    m_prog(prog),
    m_args(args),
    m_target_dir(target_dir)
{
}

void ConsoleUtilOperation::start() noexcept
{
    // ugly code 4 process starting...


//    emit finished(true, QString());
    emit progress(MAX_OPERATION_PROGRESS, QString());
    emit finished(false, "it is not implemented");
}
