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
    qDebug()<<QSerializer::toByteArray(field.toJson()).toStdString().c_str();
    qDebug()<<QSerializer::toByteArray(field.toXml()).toStdString().c_str();


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
    qDebug()<<QSerializer::toByteArray(collection.toJson()).toStdString().c_str();
    qDebug()<<QSerializer::toByteArray(collection.toXml()).toStdString().c_str();


    qDebug()<<"\nCUSTOM OBJECT";
    CustomObject object;
    object.digit = 8;
    object.string.append("third");
    object.string.append("second");
    object.string.append("first");
    qDebug()<<QSerializer::toByteArray(object.toJson()).toStdString().c_str();
    qDebug()<<QSerializer::toByteArray(object.toXml()).toStdString().c_str();


    qDebug()<<"\nCOLLECTION OF CUSTOM OBJECTS";
    CollectionOfObjects collectionObjects;
    collectionObjects.objects.append(object);
    collectionObjects.objects.append(object);
    collectionObjects.objects.append(object);
    qDebug()<<QSerializer::toByteArray(collectionObjects.toJson()).toStdString().c_str();
    qDebug()<<QSerializer::toByteArray(collectionObjects.toXml()).toStdString().c_str();


    qDebug()<<"\nGENERAL";
    General general;
    general.field = field;
    general.object = object;
    general.collection = collection;
    general.collectionObjects = collectionObjects;
    qDebug()<<QSerializer::toByteArray(general.toJson()).toStdString().c_str();
    qDebug()<<QSerializer::toByteArray(general.toXml()).toStdString().c_str();


    QFile json("../general.json");
    if(json.exists())
        json.remove();
    if(json.open(QIODevice::WriteOnly))
    {
        json.write(QSerializer::toByteArray(general.toJson()));
        json.close();
    }

    QFile xml("../general.xml");
    if(xml.exists())
        xml.remove();
    if(xml.open(QIODevice::WriteOnly))
    {
        xml.write(QSerializer::toByteArray(general.toXml()));
        xml.close();
    }


    return a.exec();
}
