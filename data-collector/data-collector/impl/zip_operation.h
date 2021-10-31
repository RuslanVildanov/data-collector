#ifndef ZIPOPERATION_H
#define ZIPOPERATION_H

#include <QString>
#include <QStringList>
#include "../i_operation.h"

class ZipOperation : public IOperation
{
    Q_OBJECT

    const QStringList m_source_files;
    const QString m_target_zip_file_path;

public:
    ZipOperation(
            const QStringList &source_files,
            const QString &target_zip_file_path) noexcept;

    // IOperation interface
    void start() noexcept final;

};

#endif // ZIPOPERATION_H
