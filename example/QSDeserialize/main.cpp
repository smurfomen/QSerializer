#include <QCoreApplication>
#include "../classes.h"

#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"QSDeserialize";

    // empty object
    General general;
    qDebug()<<QJsonDocument(general.toJson()).toJson().toStdString().c_str();
    qDebug()<<general.toXml().toDocument().toString().toStdString().c_str();


    qDebug()<<"\nDESERIALIZATION";
    QFile json ("../general.json");
    if(!json.exists())
        qWarning()<<"ERROR: general.json is not exist";
    if(json.open(QIODevice::ReadOnly))
    {
        general.fromJson(json.readAll());
        qDebug()<<QSerializer::toByteArray(general.toJson()).toStdString().c_str();
        json.close();
    }


    QFile xml ("../general.xml");
    if(!xml.exists())
        qWarning()<<"ERROR: general.xml is not exist";
    if(xml.open(QIODevice::ReadOnly))
    {
        general.fromXml(xml.readAll());
        qDebug()<<QSerializer::toByteArray(general.toXml()).toStdString().c_str();
        xml.close();
    }

    return a.exec();
}
