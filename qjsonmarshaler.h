#ifndef QJSONMARSHALER_H
#define QJSONMARSHALER_H
#include <propertykeeper.h>
#include <simplekeepers.h>
#include <arraykeepers.h>
#include <qjsonmarshalerlib_global.h>

class QJSONMARSHALERLIBSHARED_EXPORT QJsonMarshaler
{
public:
    QJsonMarshaler ();
    ~QJsonMarshaler();

    // задает аннотацию для объекта маршалинга - анмаршалинга.
    void setJsonObjectAnotation(QString annotation);

    // производит анмаршалинг по массиву, строке или JSON документу
    static void UnmarshalDocument(QByteArray jsonArr);
    static void UnmarshalDocument(QString jsonStr);
    static void UnmarshalDocument(QJsonDocument doc);


    // задает простой член типа Т в propertyes
    template<typename T>
    void setJsonProperty(T &pMember, QString annotation)
    {
        propertyes.push_back(new SimpleKeeper<T>(&pMember, annotation));
    }

    // задает вектор типа А в propertyes
    template<typename A>
    void setJsonProperty(std::vector<A> &pArray, QString annotation)
    {
        propertyes.push_back(new SimpleArrayKeeper<A>(&pArray, annotation));
    }

    // шаблонная функция, конвертирует JSON в объект типа и возвращает указатель на него
    template<typename T>
    static T * Unmarshal(QJsonObject json)
    {
        QObject * qobj = new T();
        T * targetObj = qobject_cast<T*>(qobj);
        if(targetObj == nullptr)
            throw (MarshalExeption(MarshalExeption::FailQObjectCast));
        QJsonMarshaler::Unmarshal(targetObj, json);
        return targetObj;
    }

    // производит анмаршалинг объекта JSON и устанавливает подходящие поля объекту QObject
    static void Unmarshal(QObject * obj, QJsonObject json);

    // производит анмаргалинг объекта JSON и выставляет связанные с ним опции из propertyes
    void Unmarshal(QJsonObject json);


    // производит маршалинг объекта QObject в JSON по метаинформации
    static QJsonObject Marshal(QObject * obj);

    // производит маршалинг propertyes в JSON
    QJsonObject Marshal();


private:

    // получить нужный хранитель мета информации
    static PropertyKeeper * getMetaKeeper(QObject * obj, QMetaProperty prop);

    // получить вектор из мета-хранителей для всех свойств объекта
    static std::vector<PropertyKeeper*> getMetaKeepersCollection(QObject * obj);

    QString objAnnotation;
    std::vector<PropertyKeeper*> propertyes;

};





#endif // QJSONMARSHALER_H

