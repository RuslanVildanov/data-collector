#include "zip_operation.h"

#include <QDir>
#include <QDebug>
#include <QFileInfo>

ZipOperation::ZipOperation(
        const QStringList &source_files,
        const QString &target_zip_file_path) noexcept :
    IOperation(),
    m_source_files(source_files),
    m_target_zip_file_path(target_zip_file_path)
{
}

void ZipOperation::start() noexcept
{
    if (m_source_files.isEmpty())
    {
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(true, QString());
        return;
    }

    // TODO: нужна нормальная реализация, сейчас пока сделано копирование в отдельный каталог

    QFileInfo target_fi(m_target_zip_file_path);
    QString target_file_name = target_fi.fileName();
    QDir target_dir = target_fi.dir();
    QString target_dir_str = target_dir.path();

    if (!target_dir.exists())
    {
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(false, tr("Target directory is not exists. %1").arg(target_dir_str));
        return;
    }

    QString archive_dir_str = target_dir_str + '/' + target_file_name;
    bool ok = target_dir.mkdir(target_file_name);
    if (!ok)
    {
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(false, tr("Can not create target directory for archive files. %1").arg(archive_dir_str));
        return;
    }

    qint8 p;
    qint32 i = 1;
    qint32 total_files = m_source_files.size();
    for(const auto &source_file : m_source_files)
    {
        p = static_cast<qint8>(
                    static_cast<qreal>(MAX_OPERATION_PROGRESS*i)/static_cast<qreal>(total_files));
        emit progress(p, tr("Copy file %1").arg(source_file));

        QFileInfo fi(source_file);
        if (!fi.exists())
        {
            i++;
            continue;
        }

        const bool ok = QFile::copy(source_file, archive_dir_str + '/' + fi.fileName());
        if (!ok)
        {
            qWarning() << "Can not copy file " << source_file << " to target directory " << archive_dir_str;
        }
        i++;
    }

    emit finished(true, QString());
}
