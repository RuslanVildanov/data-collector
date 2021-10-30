#include "operation_factory.h"

#include "impl/async_operation_invoker.h"
#include "impl/console_util_operation.h"
#include "impl/copy_by_wildcard_operation.h"

OperationFactory::OperationFactory() noexcept :
    IOperationFactory()
{
}

QSharedPointer<IOperationInvoker> OperationFactory::createAsyncOperationInvoker() noexcept
{
    return QSharedPointer<IOperationInvoker>(
                new AsyncOperationInvoker(),
                &QObject::deleteLater);
}

QSharedPointer<IOperation> OperationFactory::createConsoleUtilOperation(
        const QString &prog,
        const QStringList &args,
        const QString &target_dir) noexcept
{
    return QSharedPointer<IOperation>(
                new ConsoleUtilOperation(prog, args, target_dir),
                &QObject::deleteLater);
}

QSharedPointer<IOperation> OperationFactory::createCopyByWildcardOperation(
        const QString &source_dir,
        const QStringList &wildcards,
        const QString &target_dir) noexcept
{
    return QSharedPointer<IOperation>(
                new CopyByWildcardOperation(source_dir, wildcards, target_dir),
                &QObject::deleteLater);
}

QSharedPointer<IOperation> OperationFactory::createCopyLastNOperation(
        const QString &source_dir,
        qint32 number,
        const QString &target_dir) noexcept
{
}

QSharedPointer<IOperation> OperationFactory::createMacroOperation(
        const QList<QSharedPointer<IOperation> > &operations) noexcept
{
}

QSharedPointer<IOperation> OperationFactory::createZipOperation(
        const QStringList &source_files,
        const QString &target_zip_file_path) noexcept
{
}
