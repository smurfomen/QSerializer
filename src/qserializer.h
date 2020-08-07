#ifndef QJSONMARSHALER_H
#define QJSONMARSHALER_H
#include "qserializerlib_global.h"
#include "serializerexception.h"

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <typeinfo>
#include <type_traits>

#include <QVector>
#include <QVariant>
#include <QMetaProperty>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariantList>
#include <string.h>


class QGadget {
    Q_GADGET
public:
    QGadget(QMetaObject mo) : mo(mo){}
    QJsonObject toJson() const {
        QJsonObject json;
        for(int i = 0; i < mo.propertyCount(); i++)
        {
            json.insert(mo.property(i).name(), mo.property(i).readOnGadget(this).toJsonValue());
        }
        return json;
    }

    void fromJson(const QJsonValue & val) {

        if(val.isObject())
        {
            QJsonObject json = val.toObject();
            QStringList keys = json.keys();
            int propCount = mo.propertyCount();
            for(int i = 0; i < propCount; i++)
            {
                for(auto key : json.keys())
                {
                    if(key == mo.property(i).name())
                        mo.property(i).writeOnGadget(this, json.value(key));
                }
            }
        }
    }

private:
    QMetaObject mo;
};

#define __QS_FIELD             "field"
#define __QS_NAME_PERFIX       "qs_info::"
#define QS_INFO(type, name, value)                                                  \
    Q_CLASSINFO(__QS_NAME_PERFIX type #name #value,                                 \
                type "\n" #name "\n" #value)                                        \


#define QS_DECLARE(type, name)                                                      \
    public :                                                                        \
    type name = type();                                                                      \

#define QS_BIND_FIELD(type, name)                                                   \
    Q_PROPERTY(QJsonValue name READ get_##name WRITE set_##name)                    \
    private:                                                                        \
    QJsonValue get_##name() const {                                                 \
        QJsonValue val = QJsonValue::fromVariant(QVariant::fromValue(name));        \
        return val;                                                                 \
    }                                                                               \
    void set_##name(QJsonValue varname){                                            \
        name = varname.toVariant().value<type>();                                   \
    }                                                                               \

// работает с любым шаблонным контейнером,
// у которого есть метод append
#define QS_BIND_ARRAY(type, contains, name)                                                   \
    Q_PROPERTY(QJsonValue name READ get_##name WRITE set_##name)                    \
    private: \
    QJsonValue get_##name() const {                                                 \
        QJsonArray val;                                                             \
        for(int i = 0; i < name.size(); i++)                                        \
            val.push_back(name.at(i));                                 \
        return QJsonValue::fromVariant(val);                                        \
    }                                                                               \
    void set_##name(QJsonValue varname){                                            \
        if(!varname.isArray())                                                      \
            return;                                                                 \
        name.clear();                                                               \
        QJsonArray val = varname.toArray();                                         \
        for(auto item : val){                                        \
            QVariant tmp(item); \
            name.append(tmp.value<contains>());                           \
        }\
    }                                                                               \

#define QS_BIND_OBJECT(type, name) \
    Q_PROPERTY(QJsonValue name READ get_##name WRITE set_##name)\
    private: \
    QJsonValue get_##name() const {\
    QJsonObject val = name.toJson();\
    return QJsonValue(val);\
    }\
    void set_##name(QJsonValue varname) {\
    if(!varname.isObject())\
    return;\
    name.fromJson(varname);\
    }\



#define QS_BIND_ARRAY_OBJECTS(type, contains, name) \
    Q_PROPERTY(QJsonValue name READ get_##name WRITE set_##name)                    \
    private: \
    QJsonValue get_##name() const {                                                 \
        QJsonArray val;                                                                 \
        for(int i = 0; i < name.size(); i++)\
            val.push_back(name.at(i).toJson());\
        return QJsonValue::fromVariant(val); \
    }\
    void set_##name(QJsonValue varname) {\
    if(!varname.isArray() || !std::is_base_of<QGadget, contains>()) \
        return; \
    name.clear(); \
    QJsonArray val = varname.toArray();\
    for(int i = 0; i < val.size(); i++) { \
        contains tmp;\
        tmp.fromJson(val.at(i)); \
        name.append(tmp);\
    }\
    }\



#define QS_FIELD(type, name)                                                   \
    QS_DECLARE(type, name)                                                          \
    QS_BIND_FIELD(type, name)                                                       \

// работает с любым шаблонным контейнером, у которого есть метод append
#define QS_ARRAY(type, contains, name)                                                   \
    QS_DECLARE(type, name)                                                          \
    QS_BIND_ARRAY(type, contains, name)                                                       \

#define QS_OBJECT(type,name)\
    QS_DECLARE(type, name)\
    QS_BIND_OBJECT(type, name)\

#define QS_ARRAY_OBJECTS(type, contains, name) \
    QS_DECLARE(type, name) \
    QS_BIND_ARRAY_OBJECTS(type, contains, name) \







namespace QSerializer
{
    /// \brief производит сериализацию QObject в JSON по имеющейся у объекта метаинформации
    QSERIALIZER_EXPORT QJsonObject toJson(QObject * qo);

    /// \brief производит сериализацию JSON в переданный QObject согласно имеющейся у объекта метаинформации
    QSERIALIZER_EXPORT void fromJson(QObject * qo, const QJsonObject & json);

    /// \brief производит сериализацию JSON в переданный QObject согласно имеющейся у объекта метаинформации
    QSERIALIZER_EXPORT void fromJson(QObject * qo, const QJsonDocument & json);

    /// \brief производит сериализацию JSON в переданный QObject согласно имеющейся у объекта метаинформации
    QSERIALIZER_EXPORT void fromJson(QObject * qo, const QByteArray & json);

    /// \brief производит JSON в переданный QObject согласно имеющейся у объекта метаинформации
    QSERIALIZER_EXPORT void fromJson(QObject * qo, const QString & json);


    /// \brief конвертирует JSON в новый объект указанного типа и возвращает указатель на него
    template <class T, typename = std::enable_if<std::is_base_of<QObject, T>::value>>
    QSERIALIZER_EXPORT T * fromJson(const QJsonObject &json)
    {
        T * targetObj = new T();
        fromJson(targetObj, json);
        return targetObj;
    }
    /// \brief конвертирует JSON в новый объект указанного типа и возвращает указатель на него
    template <class T, typename = std::enable_if<std::is_base_of<QObject, T>::value>>
    QSERIALIZER_EXPORT T * fromJson(const QJsonDocument & json)
    {
        QJsonObject jObject = json.object();
        return fromJson<T>(jObject);
    }

    /// \brief конвертирует JSON в новый объект указанного типа и возвращает указатель на него
    template <class T, typename = std::enable_if<std::is_base_of<QObject, T>::value>>
    QSERIALIZER_EXPORT T * fromJson(const QByteArray & json)
    {
        QJsonObject jObject = QJsonDocument::fromJson(json).object();
        return fromJson<T>(jObject);
    }

    /// \brief конвертирует JSON в новый объект указанного типа и возвращает указатель на него
    template <class T, typename = std::enable_if<std::is_base_of<QObject, T>::value>>
    QSERIALIZER_EXPORT T * fromJson(const QString &json)
    {
        const char * raw = json.toStdString().c_str();
        QJsonObject jObject = QJsonDocument::fromRawData(raw, static_cast<int>(strlen(raw))).object();
        return fromJson<T>(jObject);
    }

    /// \brief производит сериализацию QObject в XML по имеющейся у объекта метаинформации
    QSERIALIZER_EXPORT QDomDocument toXml(QObject * qo);

    /// \brief модифицирует существующий объект из XML
    QSERIALIZER_EXPORT void fromXml(QObject * qo, const QDomNode &xml);

    /// \brief конвертирует XML в новый объект указанного типа и возвращает указатель на него
    template <class T, typename = std::enable_if<std::is_base_of<QObject, T>::value>>
    QSERIALIZER_EXPORT T * fromXml(const QDomNode &xml)
    {
        T * targetObj = new T();
        fromXml(targetObj, xml);
        return targetObj;
    }
}






#endif // QJSONMARSHALER_H

