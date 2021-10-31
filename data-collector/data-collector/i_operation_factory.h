#ifndef I_OPERATION_FACTORY_H
#define I_OPERATION_FACTORY_H

#include <QSharedPointer>

class QString;
class QStringList;
class IOperation;
class IOperationInvoker;

class IOperationFactory
{
public:
    virtual ~IOperationFactory() = default;

    virtual QSharedPointer<IOperationInvoker> createAsyncOperationInvoker() noexcept = 0;
    virtual QSharedPointer<IOperation> createConsoleUtilOperation (
            const QString &prog,
            const QString &working_dir,
            const QStringList &args,
            const QString &output_file_path) noexcept = 0;
    virtual QSharedPointer<IOperation> createCopyByWildcardOperation (
            const QString &source_dir,
            const QStringList &wildcards,
            const QString &target_dir) noexcept = 0;
    virtual QSharedPointer<IOperation> createCopyLastNOperation (
            const QString &source_dir,
            qint32 number,
            const QStringList &wildcards,
            const QString &target_dir) noexcept = 0;
    virtual QSharedPointer<IOperation> createMacroOperation (
            const QList<QSharedPointer<IOperation> > &operations) noexcept = 0;
    virtual QSharedPointer<IOperation> createZipOperation (
            const QStringList &source_files,
            const QString &target_zip_file_path) noexcept = 0;

protected:
    IOperationFactory() = default;

};

#endif // I_OPERATION_FACTORY_H
