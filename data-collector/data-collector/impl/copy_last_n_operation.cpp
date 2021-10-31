#include "copy_last_n_operation.h"

#include <QDir>
#include <QFile>
#include <QtMath>
#include <QDebug>
#include <QStringList>

CopyLastNOperation::CopyLastNOperation(
        const QString &source_dir,
        qint32 number,
        const QStringList &wildcards,
        const QString &target_dir) noexcept :
    IOperation(),
    m_source_dir(source_dir),
    m_number(number),
    m_wildcards(wildcards),
    m_target_dir(target_dir)
{
}

void CopyLastNOperation::start() noexcept
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
    if (m_number == 0)
    {
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(true, QString());
        return;
    }

    qint8 p;
    const QStringList &file_names =
            m_wildcards.isEmpty() ?
                source_dir.entryList(QDir::Files, QDir::Time) :
                source_dir.entryList(m_wildcards, QDir::Files, QDir::Time);
    qint32 i = 1;
    qint32 total_files = qMin(m_number, file_names.size());
    for(const QString &file_name : file_names)
    {
        p = static_cast<qint8>(
                    static_cast<qreal>(MAX_OPERATION_PROGRESS*i)/static_cast<qreal>(total_files));
        emit progress(p, tr("Copy file %1").arg(file_name));
        const bool ok = QFile::copy(m_source_dir + '/' + file_name, m_target_dir + '/' + file_name);
        if (!ok)
        {
            qWarning() << "Can not copy file " << file_name << " to target directory " << m_target_dir;
        }
        if (i >= m_number)
        {
            break;
        }
        i++;
    }
    emit finished(true, QString());
}
