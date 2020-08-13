#include <QCoreApplication>
#include "../classes.h"

#include <QFile>
#include <QDebug>

void json_example() {
    qDebug()<<"\n\nSERIALIZE JSON_EXAMPLE";
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


void xml_example(){
    qDebug()<<"\n\nSERIALIZE XML_EXAMPLE";
    TestXml src;
    src.field = 10;
    for(int i = 0; i < src.field; i ++)
        src.collection.append(i);
    src.object.digit = 666;
    src.object.string.append("it's first note in string array");
    src.object.string.append("it's second note in string array");

    // make hat if need this
    QDomDocument node = QSerializer::appendXmlHat(src.toXml(), "UTF-8");
    qDebug()<<QSerializer::toByteArray(node).constData();

    TestXml dst;
    dst.fromXml(node);
    qDebug()<<QSerializer::toByteArray(dst.toXml()).constData();
}


void serialize_to_file() {
    qDebug()<<"\n\nSERIALIZE_TO_FILE_EXAMPLE";
    qDebug()<<"\nfield example";
    Field field;
    field.flag = false;
    field.digit = 10;
    field.string = "some string";
    field.d_digit = 88.99;
    qDebug()<<QSerializer::toByteArray(field.toJson()).constData();
    qDebug()<<QSerializer::toByteArray(field.toXml()).constData();

    qDebug()<<"\ncollection example";
    Collection collection;
    collection.list.append("first");
    collection.list.append("second");
    collection.list.append("third");
    for(int i = 0; i < 20 ; i ++)
        collection.vector.append(i);
    collection.stack.append(2.44);
    collection.stack.append(4.42);
    collection.stack.append(77);
    qDebug()<<QSerializer::toByteArray(collection.toJson()).constData();
    qDebug()<<QSerializer::toByteArray(collection.toXml()).constData();


    qDebug()<<"\nobject example";
    CustomObject object;
    object.digit = 8;
    object.string.append("third");
    object.string.append("second");
    object.string.append("first");
    qDebug()<<QSerializer::toByteArray(object.toJson()).constData();
    qDebug()<<QSerializer::toByteArray(object.toXml()).constData();


    qDebug()<<"\ncollection of objects example";
    CollectionOfObjects collectionObjects;
    for(int i = 0; i < 5 ; i ++)
    {
        CustomObject o;
        o.digit = i;
        for(int k = 0; k < 5; k ++)
            o.string.append(QString("so it's just index number in binary %1").arg(QString::number(k+i,2)));
        collectionObjects.objects.append(o);
    }
    qDebug()<<QSerializer::toByteArray(collectionObjects.toJson()).constData();
    qDebug()<<QSerializer::toByteArray(collectionObjects.toXml()).constData();


    qDebug()<<"\ngeneral document example";
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
}

#define SERIALIZE_TO_FILE_EX
#define XML_EX
#define JSON_EX
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"QSSerialize";
#ifdef XML_EX
    xml_example();
#endif

#ifdef JSON_EX
    json_example();
#endif

#ifdef SERIALIZE_TO_FILE_EX
    serialize_to_file();
#endif
    return a.exec();
}
