#ifndef FRAMEWORKTEST_H
#define FRAMEWORKTEST_H

#include <QObject>
#include <QString>
#include <QTemporaryDir>
#include <QSharedPointer>

class IOperation;
class IOperationFactory;
class IOperationInvoker;

class FrameworkTest : public QObject
{
    Q_OBJECT

    QSharedPointer<IOperationFactory> m_factory;
    QSharedPointer<IOperationInvoker> m_invoker;
    QSharedPointer<IOperation> m_collect_files_operation;
    QSharedPointer<IOperation> m_zip_operation;
    QTemporaryDir m_target_dir;

public:
    explicit FrameworkTest(QObject *parent = nullptr) noexcept;

    bool run(const QString &prog,
             const QString &working_dir,
             const QStringList &args,
             const QString &output_file_path,
             const QString &last_n_source_dir,
             const QStringList &wildcards,
             qint32 number) noexcept;

signals:
    void finished();

private slots:
    void onOperationProgress(qint8 value, const QString &text);
    void onCollectFilesOperationFinished(bool ok, const QString &msg);
    void onZipOperationFinished(bool ok, const QString &msg);

private:
    static QString generateUniqueFileZipPath(
            const QString &target_dir,
            const QString &file_name_prefix) noexcept;
    static void clearFilesInDirectory(const QString &target_dir) noexcept;

};

#endif // FRAMEWORKTEST_H
