#include <QCoreApplication>
#include "../classes.h"

#include <QFile>
#include <QDebug>

void exampleStudent() {
    Student stud;
    stud.age = 23;
    stud.name = "Vlad";
    stud.links.append("https:/github.com/smurfomen");

    Parent mother;
    mother.age = 43;
    mother.male = false;
    mother.name = "Olga";

    Parent father;
    father.age = 48;
    father.male = true;
    father.name = "Alex";
    stud.parents.append(mother);
    stud.parents.append(father);

    qDebug()<<QSerializer::toByteArray(stud.toJson()).constData();
}

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
    qDebug()<<QSerializer::toByteArray(field.toJson()).constData();
    qDebug()<<QSerializer::toByteArray(field.toXml()).constData();

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
    qDebug()<<QSerializer::toByteArray(collection.toJson()).constData();
    qDebug()<<QSerializer::toByteArray(collection.toXml()).constData();


    qDebug()<<"\nCUSTOM OBJECT";
    CustomObject object;
    object.digit = 8;
    object.string.append("third");
    object.string.append("second");
    object.string.append("first");
    qDebug()<<QSerializer::toByteArray(object.toJson()).constData();
    qDebug()<<QSerializer::toByteArray(object.toXml()).constData();


    qDebug()<<"\nCOLLECTION OF CUSTOM OBJECTS";
    CollectionOfObjects collectionObjects;
    collectionObjects.objects.append(object);
    collectionObjects.objects.append(object);
    collectionObjects.objects.append(object);
    qDebug()<<QSerializer::toByteArray(collectionObjects.toJson()).constData();
    qDebug()<<QSerializer::toByteArray(collectionObjects.toXml()).constData();


    qDebug()<<"\nGENERAL";
    General general;
    general.field = field;
    general.object = object;
    general.collection = collection;
    general.collectionObjects = collectionObjects;
    qDebug()<<QSerializer::toByteArray(general.toJson()).constData();
    qDebug()<<QSerializer::toByteArray(general.toXml()).constData();


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
