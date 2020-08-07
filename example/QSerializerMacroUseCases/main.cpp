#include <QCoreApplication>
#include "field.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Field f;
    f.array.append(1);
    f.array.append(2);
    f.message = "12";
    f.ob.i = 33;
    f.ob.s = "OBJECT";
    f.ob.names.names.append("qwerty");
    f.ob.names.names.append("ytrewq");
    f.ob.names.count = 123123;

    f.vob.append(Obj());
    f.vob[0].i = 10;
    f.vob[0].s = "first";
    f.vob[0].names.count = 1000;
    f.vob[0].names.names.append("Kate");
    f.vob[0].names.names.append("Jane");


    f.vob.append(Obj());
    f.vob[1].i = 20;
    f.vob[1].s = "second";
    f.vob[1].names.count = 2000;
    f.vob[1].names.names.append("John");
    f.vob[1].names.names.append("Mark");


    Field filling;
    filling.fromJson(f.toJson());

    qDebug()<<QJsonDocument(filling.toJson()).toJson().toStdString().c_str();
    return a.exec();
}
