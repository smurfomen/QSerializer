#include <QCoreApplication>
#include "field.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Field f;
    f.array.append(1);
    f.message = "12";
    f.ob.i = 33;
    f.ob.s = "OBJECT";

    f.vob.append(Obj());
    f.vob[0].i = 10;
    f.vob[0].s = "first";

    f.vob.append(Obj());
    f.vob[0].i = 20;
    f.vob[0].s = "second";

    qDebug()<<QJsonDocument(f.toJson()).toJson().toStdString().c_str();
    return a.exec();
}
