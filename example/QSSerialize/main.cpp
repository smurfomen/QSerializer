#include <QCoreApplication>
#include "../classes.h"

#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"QSSerialize";

    qDebug()<<"\nFIELDS";
    Field field;
    field.flag = false;
    field.digit = 10;
    field.string = "some string";
    field.d_digit = 88.99;
    qDebug()<<QJsonDocument(field.toJson()).toJson().toStdString().c_str();
    qDebug()<<field.toXml().toDocument().toString().toStdString().c_str();


    qDebug()<<"\nCOLLECTIONS";
    Collection collection;
    collection.list.append("first");
    collection.list.append("second");
    collection.list.append("third");
    collection.vector.append(1);
    collection.vector.append(2);
    collection.vector.append(3);
    collection.stack.append(2.44);
    collection.stack.append(4.42);
    collection.stack.append(77);
    qDebug()<<QJsonDocument(collection.toJson()).toJson().toStdString().c_str();
    qDebug()<<collection.toXml().toDocument().toString().toStdString().c_str();


    qDebug()<<"\nCUSTOM OBJECT";
    CustomObject object;
    object.digit = 8;
    object.string.append("third");
    object.string.append("second");
    object.string.append("first");
    qDebug()<<QJsonDocument(object.toJson()).toJson().toStdString().c_str();
    qDebug()<<object.toXml().toDocument().toString().toStdString().c_str();


    qDebug()<<"\nCOLLECTION OF CUSTOM OBJECTS";
    CollectionOfObjects collectionObjects;
    collectionObjects.objects.append(object);
    collectionObjects.objects.append(object);
    collectionObjects.objects.append(object);
    qDebug()<<QJsonDocument(collectionObjects.toJson()).toJson().toStdString().c_str();
    qDebug()<<collectionObjects.toXml().toDocument().toString().toStdString().c_str();


    qDebug()<<"\nGENERAL";
    General general;
    general.field = field;
    general.object = object;
    general.collection = collection;
    general.collectionObjects = collectionObjects;
    qDebug()<<QJsonDocument(general.toJson()).toJson().toStdString().c_str();
    qDebug()<<general.toXml().toDocument().toString().toStdString().c_str();

    QFile json("../general.json");
    if(json.exists())
        json.remove();
    if(json.open(QIODevice::WriteOnly))
    {
        json.write(QJsonDocument(general.toJson()).toJson().toStdString().c_str());
        json.close();
    }

    QFile xml("../general.xml");
    if(xml.exists())
        xml.remove();
    if(xml.open(QIODevice::WriteOnly))
    {
        xml.write(general.toXml().toDocument().toString().toStdString().c_str());
        xml.close();
    }


    return a.exec();
}
