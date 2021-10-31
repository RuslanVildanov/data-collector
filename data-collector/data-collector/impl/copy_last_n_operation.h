#ifndef COPYLASTNOPERATION_H
#define COPYLASTNOPERATION_H

#include <QString>
#include <QStringList>
#include "../i_operation.h"

class CopyLastNOperation : public IOperation
{
    Q_OBJECT

    const QString m_source_dir;
    const qint32 m_number;
    const QStringList m_wildcards;
    const QString m_target_dir;

public:
    CopyLastNOperation(
            const QString &source_dir,
            qint32 number,
            const QStringList &wildcards,
            const QString &target_dir) noexcept;

    // IOperation interface
    void start() noexcept final;

    Q_DISABLE_COPY(CopyLastNOperation)
};

#endif // COPYLASTNOPERATION_H
