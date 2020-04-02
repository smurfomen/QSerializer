# This repo for Qt/C++ Json Marshaling based on QtCore
## Workflow on _Qt/C++_
## **Mark serialization fields**
#### You can inherit from QJsonMarshaler and gain access to property setting functions
```C++
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
  bool epmloyed{false};
  std::vector<QString> skills; 
}
```
#### You can mark serializable fields of object using macro Q_PROPERTY and inheriting from QOject
 * Q_PROPERTY should include atribute USER with value equal true
 * Q_OBJECT macro should be included on your class to declare for moc-generator this type as a QObject
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
  bool epmloyed{false};
  std::vector<QString> skills; 
}
```
## **Marshaling**
#### In case with inherit QJsonMarshaler
```C++
...
User u;
u.name = "Bob";
u.age = 20;
u.employed = true;
skills.push_back("Computer Scince");
skills.push_back("Foreign languages");
QJsonObject userJson = u.Marshal();
```
#### In case with Q_PROPERTY
```C++
QJsonObject userJson = QJsonMarshaler::Marshal(&u);
```

## **Unmarshaling**
#### In case with inherit QJsonMarshaler
```C++
...
QJsonObject userJson;
User u;
u.Unmarshal(userJson);
```
#### In case with Q_PROPERTY if you need to get a new serialized object
```C++
...
QJsonObject userJson;
User * u = QJsonObject::Unmarshal<User>(userJson);
```
#### In case with Q_PROPERTY if you need to modify an existing object
```C++
...
QJsonObject userJson;
QJsonObject::Unmarshal(&u, userJson);
```


