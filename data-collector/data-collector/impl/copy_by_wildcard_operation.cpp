#include "copy_by_wildcard_operation.h"

#include <QDir>
#include <QDebug>
#include <QFileInfo>

CopyByWildcardOperation::CopyByWildcardOperation(
        const QString &source_dir,
        const QStringList &wildcards,
        const QString &target_dir) noexcept :
    IOperation(),
    m_source_dir(source_dir),
    m_wildcards(wildcards),
    m_target_dir(target_dir)
{
}

void CopyByWildcardOperation::start() noexcept
{
    QDir source_dir(m_source_dir);

    if (!source_dir.exists())
    {
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(false, tr("Source directory is not exists. %1").arg(m_source_dir));
        return;
    }
    if (!QDir(m_target_dir).exists())
    {
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(false, tr("Target directory is not exists. %1").arg(m_target_dir));
        return;
    }

    qint8 p;
    const QStringList &files = source_dir.entryList(m_wildcards, QDir::Files);
    qint32 i = 1;
    qint32 total_files = files.size();
    for(const QString &file : files)
    {
        p = static_cast<qint8>(
                    static_cast<qreal>(i)/static_cast<qreal>(total_files)*MAX_OPERATION_PROGRESS);
        emit progress(p, tr("Copy file %1").arg(file));
        QFileInfo fi(file);
        const bool ok = QFile::copy(file, m_target_dir + '/' + fi.fileName());
        if (!ok)
        {
            qWarning() << "Can not copy file " << file << " to target directory " << m_target_dir;
        }
        i++;
    }
    emit finished(true, QString());
}
