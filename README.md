## This repo for Qt/C++ Json Marshaling based on QtCore
# Workflow on _Qt/C++_
### **Make class for _serialization_**
```C++
// In first make the class serializable
class Data : public QJsonMarshaler
{
public:
  Data()
  {
    // Define data members to be serialized
    setJsonProperty(&count, "count");
    setJsonProperty(&nameOjCluster, "name");
    setJsonProperty(&isActual, "actual");
  }
  
private:
  int count{0};
  QString nameOfCluster;
  bool isActual{0};
}
```
### **_Marshaling_ procedure based on _defined_ object properties**
```C++
...
Data cluster;
cluster.count = 0;
cluster.nameOfCluster = "bigger";
cluster.isActual = true;
QJsonObject clusterJson = cluster.Marshal();
// json in clusterJson object => {"count": 0,"name": "bigger","actual": true}
```

### **_Unmarshaling_ procedure based on _defined_ object properties**
```C++
...
QJsonObject clusterJson; // {"count": 0,"name": "bigger","actual": true} <= json in clusterJson object
Data cluster;
cluster.Unmarshal(obj)
// cluster values is count = 0, nameOfCluster = "bigger", isActual = true.
```


