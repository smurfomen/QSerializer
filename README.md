# QSerializer 
This project is designed to convert data from an object view to JSON or XML and opposite in the Qt/C++ ecosystem. C ++ classes by default do not have the required meta-object information for serializing class fields, but Qt is equipped with its own highly efficient meta-object system.
An important feature of the QSerializer is the ability to specify serializable fields of the class without having to serialize the entire class. QSerilaizer generate code and declare Q_PROPERTY for every declared member of class. This is convenient because you do not need to create separate structures or classes for serialization of write some code to serialize every class, it's just included to QSerializer.

## Installation
Download repository
```bash
$ git clone https://github.com/smurfomen/QSerializer.git
```
Just include qserializer.h in your project and enjoy simple serialization. qserializer.h located in src folder.

Demo-projects for using QSerializer locate are in the examples folder.

## Workflow
## Create serialization class
To get started, include qserializer.h in your code.
Next step, inherit from QSerializer and declare some serializable fields. QSerializer is not QObject-based class, but used Q_GADJET for greater performance. You may not inherit from QSerializer, for this case you need to define the methods in the class:
 - QJsonValue toJson()
 - void fromJson(const QJsonValue &)
 - QDomNode toXml()
 - void fromXml(const QDomNode &)
 - const QMetaObject * metaObject() const

For create serializable member of class and generate propertyes, use macro:
- QS_FIELD
- QS_COLLECTION
- QS_OBJECT
- QS_COLLECTION_OBJECTS

If you want only declare exists fields - use macro QS_JSON_FIELD, QS_XML_FIELD, QS_JSON_COLLECTION and other (look at qserializer.h)
```C++
class User : public QSerializer
{
QS_CLASS
// Create data members to be serialized - you can use this members in code
QS_FIELD(int, age)
QS_COLLECTION(QVector, QString, parents)
public:
  // Make constructor, where provide staticMetaObject in base QSerializer class
  User() QS_PROVIDE { }
};
```
Macro QS_CLASS is redefined macro Q_GADGET.
Macro QS_PROVIDE is define your constructor for provide staticMetaObject of Q_GADGET classes to QSerializer.

You may not use QS_CLASS and QS_PROVIDE if you not use QSerializer, but use QObject based class, because the QObjest already contains the metaObject method and stores it in itself, however QObject is a heavier parent.

### Alternative case without inherit QSerializer
```C++
class User : public QObject
{
Q_OBJECT
// Create data members to be serialized - you can use this members in code
QS_FIELD(int, age)
QS_COLLECTION(QVector, QString, parents)
public:
  // Make constructor, where provide staticMetaObject in base QSerializer class
  User() { }
  QJsonValue toJson();
  void fromJson(const QJsonValue &);
  QDomNode toXml();
  void fromXml(const QDomNode &);
};
```

## **Serialize**
Now you can serialize object of this class to JSON or XML.
For example:
```C++
User u;
u.age = 20;
u.parents.append("Mary");
u.parents.append("Jeff");
QJsonObject jsonUser = u.toJson();
// or
QDomNode xmlUser = u.toXml();
```

## **Deserialize**
Opposite of the serialization procedure is the deserialization procedure.
You can deserialize object from JSON or XML, declared fields will be modified or resets to default for this type values.
For example:
```C++
...
User u;
QJsonObject userJson;
u.fromJson(userJson);
//or
QDomNode userXml;
u.fromXml(userXml);
```
## Detailed description
| Macro | Description | Restrictions | For example |
|-|-|-|-|
| QS_FIELD | Create simple field, generate methods and propertyes | Available types: <li>int</li> <li>QString</li> <li>double</li> <li>bool</li> <li>short</li> <li>unsigned char</li> | create field named "Digit" of int type</br> QS_FIELD(int, Digit) |
| QS_COLLECTION | Create collection of simple fields, generate methods and propertyes | Available types of collection: <li>QVector</li> <li>QList</li> <li>QStack</li> <li>QQueue</li> | create collection named "MyCollection" of QVector\<int\> type</br> QS_COLLECTION(QVector, int, MyCollection) |
| QS_OBJECT | Create some class object field, generate methods and propertyes | You may create some object field, if class of this object provide methods: <li>QJsonValue toJson() const</li> <li>void fromJson(const QJsonValue &)</li> <li>QDomNode toXml() const</li> <li>void fromXml(const QDomNode &)</li> <li>const QMetaObject * metaObject() const</li> | create some object "MyObject"  of MyClass type</br> QS_OBJECT(MyClass, MyObject) |
| QS_COLLECTION_OBJECTS | Create collection of some objects, generate methods and propertyes | You may create collection of objects satisfying restrictions of QS_OBJECT | create collection of some objects</br> named "MyObjectsCollection" of QList\<MyClass\> type QS_COLLECTION_OBJECTS(QList, MyClass, MyObjectsCollection) |
| QS_CLASS | Added in class QMetaObject System | Use this macro if you don't use QObject based class | class ClassName : public QSerializer { </br> QS_CLASS</br>  ...</br> }; |
| QS_PROVIDE | Provide metaObject to QSerializer with you constructor | Use this macro only with QS_CLASS and inherit of QSerializer. You already have QMetaObject in QObject-based classes. | class ClassName : public QSerializer { </br> QS_CLASS</br> public:</br> ClassName() QS_PROVIDE {}</br> }; |
| QS_DECLARE_VARRIABLE | Created new public member of class | Use exists types | create public member class [int Digit]</br> QS_DECLARE_VARIABLE(int, Digit) |
