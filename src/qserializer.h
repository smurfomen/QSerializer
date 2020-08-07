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

    void fromJson(const QJsonObject json) {
        for(int i = 0; i < mo.propertyCount(); i++)
        {
            for(auto key : json.keys())
            {
                if(key == mo.property(i).name())
                    mo.property(i).writeOnGadget(this, json.value(key));
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
    type name;                                                                      \

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
#define QS_BIND_ARRAY(type, name)                                                   \
    Q_PROPERTY(QJsonValue name READ get_##name WRITE set_##name)                    \
    private:                                                                        \
    QJsonValue get_##name() const {                                                 \
        QJsonArray val;                                                             \
        for(int i = 0; i < name.size(); i++)                                        \
            val.append(name.at(i));                                 \
        return QJsonValue::fromVariant(val);                                        \
    }                                                                               \
    void set_##name(QJsonValue varname){                                            \
        if(!varname.isArray())                                                      \
            return;                                                                 \
        name.clear();\
        QJsonArray val = varname.toArray();                                         \
        for(int i = 0; i < val.size(); i++)                                        \
        name.append(val.at(i).toVariant().value<type>());                           \
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
    QJsonObject val = varname.toObject();\
    name.fromJson(val);\
    }\



#define QS_BIND_ARRAY_OBJECTS(type, name) \
    Q_PROPERTY(QJsonValue name READ get_##name WRITE set_##name)                    \
    private: \
    QJsonValue get_##name() const {                                                 \
        QJsonArray val;                                                                 \
        for(int i = 0; i < name.size(); i++)\
            val.append(name.at(i).toJson());\
        return QJsonValue::fromVariant(val); \
    }\
    void set_##name(QJsonValue varname) {\
    if(!varname.isArray()) \
        return; \
    name.clear(); \
    QJsonArray val = varname.toArray();\
    for(int i = 0; i < val.size(); i++) { \
        QGadget * gad = (QGadget*)(&name.at(i));\
        if(gad)\
            gad->fromJson(val.at(i).toObject()); \
    }\
    }\



#define QS_MAKE_FIELD(type, name)                                                   \
    QS_DECLARE(type, name)                                                          \
    QS_BIND_FIELD(type, name)                                                       \

// работает с любым шаблонным контейнером, у которого есть метод append
#define QS_MAKE_ARRAY(type, name)                                                   \
    QS_DECLARE(type, name)                                                          \
    QS_BIND_ARRAY(type, name)                                                       \

#define QS_MAKE_OBJECT(type,name)\
    QS_DECLARE(type, name)\
    QS_BIND_OBJECT(type, name)\

#define QS_MAKE_ARRAY_OBJECTS(type, name) \
    QS_DECLARE(type, name) \
    QS_BIND_ARRAY_OBJECTS(type,name) \


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

