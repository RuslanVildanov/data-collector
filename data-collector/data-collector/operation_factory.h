#ifndef OPERATIONFACTORY_H
#define OPERATIONFACTORY_H

#include "i_operation_factory.h"

class OperationFactory : public IOperationFactory
{
public:
    OperationFactory() noexcept;

    // IOperationFactory interface
    QSharedPointer<IOperationInvoker> createAsyncOperationInvoker() noexcept final;
    QSharedPointer<IOperation> createConsoleUtilOperation(
            const QString &prog,
            const QStringList &args,
            const QString &target_dir) noexcept final;
    QSharedPointer<IOperation> createCopyByWildcardOperation(
            const QString &source_dir,
            const QStringList &wildcards,
            const QString &target_dir) noexcept final;
    QSharedPointer<IOperation> createCopyLastNOperation(
            const QString &source_dir,
            qint32 number,
            const QString &target_dir) noexcept final;
    QSharedPointer<IOperation> createMacroOperation(
            const QList<QSharedPointer<IOperation> > &operations) noexcept final;
    QSharedPointer<IOperation> createZipOperation(
            const QStringList &source_files,
            const QString &target_zip_file_path) noexcept final;

};

#endif // OPERATIONFACTORY_H
