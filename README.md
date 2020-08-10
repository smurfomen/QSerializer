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
To get started, include qserializer.h in your code, inherit from QSerializer and declare some serializable fields. 
For create serializable member of class and generate propertyes, use macro:
- QS_FIELD
- QS_COLLECTION
- QS_OBJECT
- QS_COLLECTION_OBJECTS

If you want only declare exists fields - use macro QS_JSON_FIELD, QS_XML_FIELD, QS_JSON_COLLECTION and other (look at qserializer.h)
```C++
class User : public QSerializer
{
Q_GADGET
// Create data members to be serialized - you can use this members in code
QS_FIELD(int, age)
QS_COLLECTION(QVector, QString, parents)
public:
  // Make constructor, where provide staticMetaObject in base QSerializer class
  User() : QSerializer(staticMetaObject) { }
};
```

## **Serialize**
Now you can serialize object of this class to JSON or XML.
For example:
```C++
User u;
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

