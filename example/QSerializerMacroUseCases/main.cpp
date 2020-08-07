#include <QCoreApplication>
#include "field.h"
#include <QDebug>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Field f;
    f.array.append(1);
    f.message = "12";

    qDebug()<<QJsonDocument(f.toJson()).toJson().toStdString().c_str();
    return a.exec();
}
