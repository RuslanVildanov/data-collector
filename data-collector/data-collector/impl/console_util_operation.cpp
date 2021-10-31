#include "console_util_operation.h"

#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QTextStream>
#include <QSharedPointer>

ConsoleUtilOperation::ConsoleUtilOperation(
        const QString &prog,
        const QString &working_dir,
        const QStringList &args,
        const QString &output_file_path) noexcept :
    IOperation(),
    m_prog(prog),
    m_working_dir(working_dir),
    m_args(args),
    m_output_file_path(output_file_path)
{
}

void ConsoleUtilOperation::start() noexcept
{
    if (!QDir(m_working_dir).exists())
    {
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(false, tr("Working directory is not exists. %1").arg(m_working_dir));
        return;
    }

    QFile f(m_output_file_path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(false, tr("Can not create output file. %1").arg(m_output_file_path));
        return;
    }
    QTextStream s(&f);

    QSharedPointer<QProcess> process(
                new QProcess(),
                &QObject::deleteLater);
    connect(process.data(), &QProcess::readyReadStandardError, [&s, &process]()
    {
        s << process->readAllStandardError() << endl;
    });
    connect(process.data(), &QProcess::readyReadStandardOutput, [&s, &process]()
    {
        s << process->readAllStandardOutput() << endl;
    });

    process->setWorkingDirectory(m_working_dir);
    process->start(m_prog, m_args);
    bool ok = process->waitForStarted();
    if (!ok)
    {
        emit progress(MAX_OPERATION_PROGRESS, QString());
        emit finished(false, tr("An error occurred when starting programm %1").arg(m_prog));
        f.close();
        return;
    }
    ok = process->waitForFinished();
    if (!ok)
    {
        qWarning() << tr("An error occurred when starting programm %1").arg(m_prog);
    }
    f.close();
    process->close();

    emit progress(MAX_OPERATION_PROGRESS, QString());
    emit finished(true, QString());
}
