#include <QCoreApplication>
#include "classes.h"

#include <QFile>
#include <QDebug>

#ifdef QS_HAS_JSON
void json_example() {
    qDebug() << "==================================== json_example ====================================";
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

    qDebug() << "json from object:";
    QByteArray d = stud.toRawJson();
    qDebug() << d.constData();

    qDebug() << "creation new object using raw json:";
    Student copy;
    copy.fromJson(d);
    qDebug() << copy.toRawJson().constData();
    qDebug() << "=============================================================================";
}
#endif

#ifdef QS_HAS_XML
void xml_example(){
    qDebug() << "==================================== xml_example ====================================";
    TestXml src;
    src.field = 10;
    for(int i = 0; i < src.field; i ++)
        src.collection.append(i);
    src.object.digit = 666;
    src.object.string.append("it's first note in string array");
    src.object.string.append("it's second note in string array");

    // make hat if need this
    QDomDocument node = QSerializer::appendXmlHat(src.toXml(), "UTF-8");
    qDebug() << "xml from object:";
    qDebug() << QSerializer::toByteArray(node).constData();

    qDebug() << "creation new object using raw xml:";
    TestXml copy;
    copy.fromXml(node);
    qDebug() << copy.toRawXml().constData();
    qDebug() << "=============================================================================";
}
#endif

void serialize_to_file() {
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

#ifdef QS_HAS_JSON
    {
        qDebug() << "==================================== serialize_to_file : general.json ====================================";
        qDebug() << general.toRawJson().constData();

        QFile json("general.json");
        if(json.exists())
            json.remove();
        if(json.open(QIODevice::WriteOnly))
        {
            json.write(general.toRawJson());
            json.close();
        }
        qDebug() << "=============================================================================";
    }
#endif

#ifdef QS_HAS_XML
    {
        qDebug() << "==================================== serialize_to_file : general.xml ====================================";
        qDebug() << general.toRawXml().constData();

        QFile xml("general.xml");
        if(xml.exists())
            xml.remove();
        if(xml.open(QIODevice::WriteOnly))
        {
            xml.write(general.toRawXml());
            xml.close();
        }
        qDebug() << "=============================================================================";
    }
#endif
}

void deserialize_from_file()
{
#ifdef QS_HAS_JSON
    {
        QFile json ("general.json");
        if(!json.exists())
            qWarning() << "ERROR: general.json is not exist";
        if(json.open(QIODevice::ReadOnly))
        {
            // empty object
            General general;
            qDebug() << "empty json object:";
            qDebug() << general.toRawJson().constData();


            general.fromJson(json.readAll());
            qDebug() << "serialized json object from file: general.json";
            qDebug() << general.toRawJson().constData();
            json.close();
        }
    }
#endif

#ifdef QS_HAS_XML
    {
        QFile xml ("general.xml");
        if(!xml.exists())
            qWarning() << "ERROR: general.xml is not exist";
        if(xml.open(QIODevice::ReadOnly))
        {
            // empty object
            General general;
            qDebug() << "empty xml object:";
            qDebug() << general.toRawXml().constData();

            qDebug() << "contains in file: general.xml";
            QByteArray d = xml.readAll();
            qDebug() << d << "\n";

            general.fromXml(d);
            qDebug() << "serialized json object from file: general.xml";
            qDebug() << general.toRawXml().constData();

            xml.close();
        }
    }
#endif
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "==================================== SERIALIZE EXAMPLES ====================================";
#ifdef QS_HAS_XML
    xml_example();
#endif

#ifdef QS_HAS_JSON
    json_example();
#endif

    serialize_to_file();
    deserialize_from_file();
    qDebug() << "=============================================================================";

	return 0;
}



