# This repo for Qt/C++ Json Marshaling based on QtCore
## Workflow on _Qt/C++_
## **Make class for _serialization_**
```C++
// In first make the class serializable
class User : public QJsonMarshaler
{
public:
  User()
  {
    // Define data members to be serialized
    setJsonProperty(name, "name");
    setJsonProperty(age, "age");
    setJsonProperty(employed, "employed");
    setJsonProperty(skills, "skills");
  }
  
private:
  QString name;
  int age{0};
  bool epmloyed{false};
  std::vector<QString> skills; 
}
```
### or
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
  User() { }
  void operator=(const User& t)
  {
    name = t.name;
    age = t.age;
    employed = t.employed;
    skills = t.skills;
  }
  User(const User& t)
  {
    operator=(t);
  }
  
  QString name;
  int age{0};
  bool epmloyed{false};
  std::vector<QString> skills; 
}
```


## **_Marshaling_ procedure based on _defined_ object properties**
```C++
...
User u;
u.name = "Bob";
u.age = 20;
u.employed = true;
skills.push_back("Computer Scince");
skills.push_back("Foreign languages");
QJsonObject userJson = u.Marshal();
// json in userJson object => {"name": "Bob","age": 20,"employed": true, "skills": ["Computer Science", "Foreign languages"]}
```
### or
```C++
...
QJsonObject userJson = QJsonMarshaler::Marshal(&u);
// json in userJson object => {"name": "Bob","age": 20,"employed": true, "skills": ["Computer Science", "Foreign languages"]}
```

## **_Unmarshaling_ procedure based on _defined_ object properties**
```C++
...
QJsonObject userJson; // {"name": "Bob","age": 20,"employed": true, "skills": ["Computer Science", "Foreign languages"]} <= json in userJson object
User u;
u.Unmarshal(userJson);
// variables of object now contains => name = "Bob", age = 20, employed = true, skills[0] = "Computer Science", skills[1] = "Foreign languages"
```
### or

```C++
...
QJsonObject userJson; // {"name": "Bob","age": 20,"employed": true, "skills": ["Computer Science", "Foreign languages"]} <= json in userJson object
User * u = QJsonObject::Unmarshal<User>(userJson) ;
// variables of object now contains => name = "Bob", age = 20, employed = true, skills[0] = "Computer Science", skills[1] = "Foreign languages"
```
### or

```C++
...
QJsonObject userJson; // {"name": "Bob","age": 20,"employed": true, "skills": ["Computer Science", "Foreign languages"]} <= json in userJson object
QJsonObject::Unmarshal(&u, userJson) ;
// variables of object now contains => name = "Bob", age = 20, employed = true, skills[0] = "Computer Science", skills[1] = "Foreign languages"
```


