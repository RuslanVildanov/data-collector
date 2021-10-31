#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QObject>
#include <QSharedPointer>
#include "framework_test.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qInfo() << "Started framework test. Main thread: " << QThread::currentThread();

    QSharedPointer<FrameworkTest> ft(
                new FrameworkTest(),
                &QObject::deleteLater);
    QObject::connect(ft.data(), &FrameworkTest::finished,
                     &a, &QCoreApplication::quit);
    // windows 7 console line: dxdiag /dontskip /whql:off /64bit /t .\report.txt
    const bool ok = ft->run(
                "cmd",
                a.applicationDirPath(),
                QStringList() << "/c" << "dxdiag /dontskip /whql:off /64bit /t .\\log\\report.txt",
                ".\\log\\report-console.out",
                "log",
                QStringList() << "*.txt",
                5);

    return ok ? a.exec() : -1;
}
