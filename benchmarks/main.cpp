#include <QCoreApplication>
#include <QtTest/QTest>
#include "bench.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Bench b;
    QTest::qExec(&b);

    return a.exec();
}
