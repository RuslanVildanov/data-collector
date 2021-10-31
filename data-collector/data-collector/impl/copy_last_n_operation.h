#ifndef COPYLASTNOPERATION_H
#define COPYLASTNOPERATION_H

#include <QString>
#include "../i_operation.h"

class CopyLastNOperation : public IOperation
{
    Q_OBJECT

    const QString m_source_dir;
    const qint32 m_number;
    const QString m_target_dir;

public:
    CopyLastNOperation(
            const QString &source_dir,
            qint32 number,
            const QString &target_dir) noexcept;

    // IOperation interface
    void start() noexcept final;

};

#endif // COPYLASTNOPERATION_H
