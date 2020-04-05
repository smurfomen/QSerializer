# QJsonMarshaler library 
This project is designed to convert data from an object view to JSON and from JSON to an object view in the Qt / C ++ ecosystem. C ++ classes by default do not have the required meta-object information for serializing class fields, but Qt is equipped with its own highly efficient meta-object system.
An important feature of the library is the ability to specify serializable fields of the class without having to serialize the entire class. This is convenient because you do not need to create separate structures or classes for serialization.

## Installation
Download repository
```bash
$ git clone https://github.com/smurfomen/QJsonMarshalerLib
$ cd QJsonMarshalerLib
```
Open file QJsonMarshalerLib.pro with QtCreator and build project. Binaries will be in the build folder after building, divided into folders "debug" and "release". To connect the library to your project, right-click on your project> select "Add Library"> "External Library" and in the window that opens, specify the path to the .so file from the "release" folder. Header files are located in the src folder from the root of the repository.

Demo-projects for using QJsonMarshalerLib locate are in the examples folder. There are also collected binaries of the examples/build library. Examples of output JSON files can be found in the root of the examples.

## Workflow
## Mark serialization fields
### You can inherit from QJsonMarshaler and gain access to property setting functions
```C++
#include <qjsonmarshaler.h>

// In first make the class serializable
class User : public QJsonMarshaler
{
public:
  User()
  {
    // Mark variables to be serialized
    setJsonProperty(name, "name");
    setJsonProperty(age, "age");
    setJsonProperty(employed, "employed");
    setJsonProperty(skills, "skills");
  }
  
  QString name;
  int age{0};
  bool employed{false};
  std::vector<QString> skills; 
};
```
### You can mark serializable fields of object using macro Q_PROPERTY and inheriting from QObject
 Q_PROPERTY should include atribute USER with value equal true
 Q_OBJECT macro should be included on your class to declare for moc-generator this type as a QObject
```C++
// In first make the class serializable
class User : public QObject
{
Q_OBJECT
// Define data members to be serialized
Q_PROPERTY(QString name MEMBER name USER true)
Q_PROPERTY(int age MEMBER age USER true)
Q_PROPERTY(bool employed MEMBER employed USER true)
Q_PROPERTY(std::vector<QString> skills MEMBER skills USER true)
public:
  // Make base constructor
  User() { }
 
  QString name;
  int age{0};
  bool employed{false};
  std::vector<QString> skills; 
};
```
## **Marshaling**
### In case with inherit QJsonMarshaler
```C++
...
User u;
QJsonObject userJson = u.Marshal();
```
### In case with Q_PROPERTY
```C++
QJsonObject userJson = QJsonMarshaler::Marshal(&u);
```

## **Unmarshaling**
### In case with inherit QJsonMarshaler
```C++
...
QJsonObject userJson;
User u;
u.Unmarshal(userJson);
```
### In case with Q_PROPERTY if you need to get a new serialized object
```C++
...
QJsonObject userJson;
User * u = QJsonMarshaler::Unmarshal<User>(userJson);
```
### In case with Q_PROPERTY if you need to modify an existing object
```C++
...
QJsonObject userJson;
QJsonMarshaler::Unmarshal(&u, userJson);
```


