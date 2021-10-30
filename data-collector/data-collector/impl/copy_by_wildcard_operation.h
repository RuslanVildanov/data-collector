#ifndef COPYBYWILDCARDOPERATION_H
#define COPYBYWILDCARDOPERATION_H

#include <QString>
#include <QStringList>
#include "../i_operation.h"

class CopyByWildcardOperation : public IOperation
{
    Q_OBJECT

    const QString m_source_dir;
    const QStringList m_wildcards;
    const QString m_target_dir;

public:
    CopyByWildcardOperation(
            const QString &source_dir,
            const QStringList &wildcards,
            const QString &target_dir) noexcept;

    // IOperation interface
    void start() noexcept final;

};

#endif // COPYBYWILDCARDOPERATION_H
