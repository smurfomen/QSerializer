## This repo for Qt/C++ Json Marshaling based on QtCore
# Workflow on _Qt/C++_
### **Make class for _serialization_**
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
    setArrayJsonProperty(skills, "skills");
  }
  
private:
  QString name;
  int age{0};
  bool epmloyed{false};
  std::vector<QString> skills; 
}
```
### **_Marshaling_ procedure based on _defined_ object properties**
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

### **_Unmarshaling_ procedure based on _defined_ object properties**
```C++
...
QJsonObject userJson; // {"name": "Bob","age": 20,"employed": true, "skills": ["Computer Science", "Foreign languages"]} <= json in userJson object
User u;
u.Unmarshal(userJson)
// variables of object now contains => name = "Bob", age = 20, employed = true, skills[0] = "Computer Science", skills[1] = "Foreign languages"
```


