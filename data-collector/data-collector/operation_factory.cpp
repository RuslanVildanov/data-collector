#include "operation_factory.h"

#include "impl/async_operation_invoker.h"
#include "impl/console_util_operation.h"
#include "impl/copy_by_wildcard_operation.h"
#include "impl/copy_last_n_operation.h"
#include "impl/macro_operation.h"
#include "impl/zip_operation.h"

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
        const QString &working_dir,
        const QStringList &args,
        const QString &output_file_path) noexcept
{
    return QSharedPointer<IOperation>(
                new ConsoleUtilOperation(prog, working_dir, args, output_file_path),
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
        const QStringList &wildcards,
        const QString &target_dir) noexcept
{
    return QSharedPointer<IOperation>(
                new CopyLastNOperation(source_dir, number, wildcards, target_dir),
                &QObject::deleteLater);
}

QSharedPointer<IOperation> OperationFactory::createMacroOperation(
        const QList<QSharedPointer<IOperation> > &operations) noexcept
{
    return QSharedPointer<IOperation>(
                new MacroOperation(operations),
                &QObject::deleteLater);
}

QSharedPointer<IOperation> OperationFactory::createZipOperation(
        const QStringList &source_files,
        const QString &target_zip_file_path) noexcept
{
    return QSharedPointer<IOperation>(
                new ZipOperation(source_files, target_zip_file_path),
                &QObject::deleteLater);
}
