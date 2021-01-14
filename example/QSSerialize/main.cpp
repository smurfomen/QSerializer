#include <QCoreApplication>
#include "../classes.h"

#include <QFile>
#include <QDebug>

void json_example() {
    qDebug()<<"====================================SERIALIZATION JSON====================================";
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
    qDebug()<<"====================================SERIALIZATION XML====================================";
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
    qDebug()<<"====================================SERIALIZE TO FILE====================================";
    /* OBJECT */
    Field field;
    field.flag = false;
    field.digit = 34;
    field.string = "some string";
    field.d_digit = 88.99;
    /*.........................................................................................................................*/

    /* COLLECTION*/
    Collection collection;
    collection.list.append("first");
    collection.list.append("second");
    collection.list.append("third");
    for(int i = 0; i < 6; i ++)
        collection.vector.append(i);
    collection.stack.append(2.44);
    collection.stack.append(4.42);
    collection.stack.append(77);
    /*.........................................................................................................................*/


    /* OBJECT */
    CustomObject object;
    object.digit = 8;
    object.string.append("third");
    object.string.append("second");
    object.string.append("first");
    /*.........................................................................................................................*/


    /* COLLECTION OF OBJECTS */
    CollectionOfObjects collectionObjects;
    for(int i = 0; i < 5 ; i ++)
    {
        CustomObject o;
        o.digit = i;
        for(int k = 0; k < 5; k ++)
            o.string.append(QString("so it's just index number in binary %1").arg(QString::number(k+i,2)));
        collectionObjects.objects.append(o);
    }
    /*.........................................................................................................................*/



    /* DICTIONARIES */
    Dictionaries dict;
    dict.std_map.insert(std::pair<int, QString>(1,"first"));
    dict.std_map.insert(std::pair<int, QString>(2,"second"));
    dict.std_map_objects.insert(std::pair<QString, Student>("+7(909)001-00-00", Student(22,
                                                                                "Ken",
                                                                                QStringList("http://github.com/smurfomen"),
                                                                                Parent(44, "Olga", false),
                                                                                Parent(48, "Alex", true))));
    dict.std_map_objects.insert(std::pair<QString, Student>("+7(909)000-10-00", Student(21,
                                                                                "Jane",
                                                                                QStringList("http://somelink.com"),
                                                                                Parent(38, "Elie", false),
                                                                                Parent(48, "John", true))));
    // fill QMap<QString, Student>
    dict.qt_map_objects.insert("+7(909)000-01-00", Student(22,
                                                    "Kate",
                                                    QStringList("http://katelink.com"),
                                                    Parent(44, "Marlin", false),
                                                    Parent(48, "Jake", true)));
    dict.qt_map_objects.insert("+7(909)100-00-10", Student(22,
                                                    "Bob",
                                                    QStringList("http://bobsite.com"),
                                                    Parent(44, "Mary", false),
                                                    Parent(48, "Koul", true)));
    dict.qt_map.insert("ping","pong");
    dict.qt_map.insert("abra","kadabra");
    /*.........................................................................................................................*/

    General general;
    general.field = field;
    general.object = object;
    general.collection = collection;
    general.collectionObjects = collectionObjects;
    general.dictionaries = dict;

    qDebug()<<"====================================GENERAL JSON====================================";
    qDebug()<<QSerializer::toByteArray(general.toJson()).constData();

    qDebug()<<"====================================GENERAL XML====================================";
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


// use case json dictionaries
// 1. create some statement local Dictionaries object (fill this)
// 2. emulate receive other json from server for this object (receive new data)
// 3. update local Dictionaries object (update statement from server)
void json_dict_example()
{
    // local object
    Dictionaries myDict;

    // fill std::map<int, QString>
    myDict.std_map.insert(std::pair<int, QString>(1,"first"));
    myDict.std_map.insert(std::pair<int, QString>(2,"second"));

    // create couple students, fill std::map<QString, Student>
    myDict.std_map_objects.insert(std::pair<QString, Student>("+7(909)001-00-00", Student(22,
                                                                                "Ken",
                                                                                QStringList("http://github.com/smurfomen"),
                                                                                Parent(44, "Olga", false),
                                                                                Parent(48, "Alex", true))));
    myDict.std_map_objects.insert(std::pair<QString, Student>("+7(909)000-10-00", Student(21,
                                                                                "Jane",
                                                                                QStringList("http://somelink.com"),
                                                                                Parent(38, "Elie", false),
                                                                                Parent(48, "John", true))));
    // fill QMap<QString, QString> with some phrases
    myDict.qt_map.insert("ping","pong");
    myDict.qt_map.insert("abra","kadabra");

    qDebug()<<QSerializer::toByteArray(myDict.toJson()).toStdString().c_str();

    // emulate receive JsonFromServer
    auto JsonFromServer = []() -> QByteArray {
        Dictionaries dict;
        // fill std::map<int, QString>
        dict.std_map.insert(std::pair<int, QString>(3, "third"));

        // fill QMap<QString, Student>
        dict.qt_map_objects.insert("+7(909)000-01-00", Student(22,
                                                        "Kate",
                                                        QStringList("http://katelink.com"),
                                                        Parent(44, "Marlin", false),
                                                        Parent(48, "Jake", true)));
        dict.qt_map_objects.insert("+7(909)100-00-10", Student(22,
                                                        "Bob",
                                                        QStringList("http://bobsite.com"),
                                                        Parent(44, "Mary", false),
                                                        Parent(48, "Koul", true)));
        // fill QMap<QString,QString> with some phrases
        dict.qt_map.insert("fokus","pokus");

        // serialize dict to QByteArray
        return QSerializer::toByteArray(dict.toJson());
    };


    // update original local object to equial received json
    myDict.fromJson(JsonFromServer());
    qDebug()<<QSerializer::toByteArray(myDict.toJson()).toStdString().c_str();

}


#define SERIALIZE_TO_FILE_EX
#define XML_EX
#define JSON_EX
#define JSON_DICT_EX
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"====================================SERIALIZE EXAMPLES====================================";
#ifdef XML_EX
    xml_example();
#endif

#ifdef JSON_EX
    json_example();
#endif

#ifdef SERIALIZE_TO_FILE_EX
    serialize_to_file();
#endif

#ifdef JSON_DICT_EX
    json_dict_example();
#endif
    return a.exec();
}



