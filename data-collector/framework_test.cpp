#include "framework_test.h"

#include <QDir>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QStandardPaths>
#include "data-collector/i_operation.h"
#include "data-collector/i_operation_invoker.h"
#include "data-collector/operation_factory.h"

FrameworkTest::FrameworkTest(QObject *parent) noexcept :
    QObject(parent)
{
}

bool FrameworkTest::run(
        const QString &prog,
        const QString &working_dir,
        const QStringList &args,
        const QString &output_file_path,
        const QString &last_n_source_dir,
        const QStringList &wildcards,
        qint32 number) noexcept
{
    clearFilesInDirectory(m_target_dir.path());

    m_factory = QSharedPointer<IOperationFactory>(new OperationFactory());
    if (!m_factory)
    {
        qWarning() << tr("Memory allocation failure: %1").arg("IOperationFactory");
        return false;
    }

    QSharedPointer<IOperation> console_util_operation =
            m_factory->createConsoleUtilOperation(
                prog,
                working_dir,
                args,
                output_file_path);

    QSharedPointer<IOperation> last_n_operation =
            m_factory->createCopyLastNOperation(
                last_n_source_dir,
                number,
                wildcards,
                m_target_dir.path());

    QList<QSharedPointer<IOperation> > operations;
    operations << console_util_operation << last_n_operation;

    m_collect_files_operation = m_factory->createMacroOperation(operations);
    connect(m_collect_files_operation.data(), &IOperation::progress,
            this, &FrameworkTest::onOperationProgress);
    connect(m_collect_files_operation.data(), &IOperation::finished,
            this, &FrameworkTest::onCollectFilesOperationFinished);

    m_invoker = m_factory->createAsyncOperationInvoker();
    if (!m_invoker)
    {
        qWarning() << tr("Memory allocation failure: %1").arg("IOperationInvoker");
        emit finished();
        return false;
    }
    m_invoker->start(m_collect_files_operation);
    return true;
}

void FrameworkTest::onOperationProgress(qint8 value, const QString &text)
{
    qInfo() << value << text;
}

void FrameworkTest::onCollectFilesOperationFinished(bool ok, const QString &msg)
{
    Q_ASSERT_X(m_collect_files_operation, "check slot method", "started operation is null");
    Q_ASSERT_X(m_factory, "check slot method", "factory is null");
    Q_ASSERT_X(m_invoker, "check slot method", "invoker is null");
    if (!m_collect_files_operation
            || !m_factory
            || !m_invoker)
    {
        qWarning() << tr("Incorrect using FrameworkTest");
        emit finished();
        return;
    }

    disconnect(m_collect_files_operation.data(), &IOperation::progress,
               this, &FrameworkTest::onOperationProgress);
    disconnect(m_collect_files_operation.data(), &IOperation::finished,
               this, &FrameworkTest::onCollectFilesOperationFinished);
    m_collect_files_operation.clear();

    QString target_zip_dir =
            QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    if (target_zip_dir.isEmpty()
            || !QDir(target_zip_dir).exists())
    {
        qWarning() << tr("Can not determine the desktop directory");
        emit finished();
        return;
    }

    if (!ok)
    {
        qWarning() << tr("An error occurred when collecting reqired files: %1").arg(msg);
        emit finished();
        return;
    }

    QDir target_dir(m_target_dir.path());
    QStringList target_zip_files = target_dir.entryList(QDir::Files);
    if (target_zip_files.isEmpty())
    {
        qWarning() << tr("There are no collected files to create zip-archive");
        emit finished();
        return;
    }
    for(auto i = target_zip_files.begin(); i != target_zip_files.end(); ++i)
    {
        QString &file = *i;
        file = m_target_dir.path() + '/' + file;
    }

    QString unique_file_zip_path = generateUniqueFileZipPath(
                target_zip_dir,
                "collected-log-files");
    if (unique_file_zip_path.isEmpty())
    {
        qWarning() << tr("Can not generate unique zip file name");
        emit finished();
        return;
    }

    m_zip_operation = m_factory->createZipOperation(
                target_zip_files,
                unique_file_zip_path);
    connect(m_zip_operation.data(), &IOperation::progress,
            this, &FrameworkTest::onOperationProgress);
    connect(m_zip_operation.data(), &IOperation::finished,
            this, &FrameworkTest::onZipOperationFinished);

    m_invoker->start(m_zip_operation);
}

void FrameworkTest::onZipOperationFinished(bool ok, const QString &msg)
{
    Q_ASSERT_X(m_zip_operation, "check slot method", "started operation is null");
    if (!m_zip_operation)
    {
        qWarning() << tr("Incorrect using FrameworkTest");
        emit finished();
        return;
    }

    disconnect(m_zip_operation.data(), &IOperation::progress,
               this, &FrameworkTest::onOperationProgress);
    disconnect(m_zip_operation.data(), &IOperation::finished,
               this, &FrameworkTest::onCollectFilesOperationFinished);
    m_zip_operation.clear();

    if (!ok)
    {
        qWarning() << tr("An error occurred when collecting reqired files: %1").arg(msg);
    }
    emit finished();
}

QString FrameworkTest::generateUniqueFileZipPath(
        const QString &target_dir,
        const QString &file_name_prefix) noexcept
{
    QDir td(target_dir);
    Q_ASSERT_X(td.exists(), "trying to get unique zip name", "target dir is not exist");
    if (!td.exists())
    {
        return QString();
    }

    qint32 id = 1;
    QString result = target_dir + '/' + file_name_prefix + ".zip";
    while (QFile::exists(result))
    {
        result = target_dir
                + '/'
                + file_name_prefix
                + QString::number(id)
                + ".zip";
        id++;
    }
    return result;
}

void FrameworkTest::clearFilesInDirectory(const QString &target_dir) noexcept
{
    bool ok;
    QDir td(target_dir);
    QStringList files = td.entryList(QDir::Files);
    for(const auto &file : files)
    {
        ok = td.remove(file);
        if (!ok)
        {
            qWarning() << tr("Can not remove file %1 in directory %2").arg(file).arg(target_dir);
        }
    }
}
