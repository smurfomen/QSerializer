#ifndef QJSONMARSHALER_H
#define QJSONMARSHALER_H
#include <qserializerlib_global.h>
#include <serializerexception.h>

#include <QObject>
#include <QJsonObject>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>


namespace QSerializer {
    /// \brief производит сериализацию JSON в переданный QObject согласно имеющейся у объекта метаинформации
    QSERIALIZER_EXPORT void fromJson(QObject * obj, const QJsonObject &json);

    /// \brief конвертирует JSON в новый объект указанного типа и возвращает указатель на него
    template<typename T> QSERIALIZER_EXPORT T * fromJson(const QJsonObject &json)
    {
        QObject * qobj = new T();
        T * targetObj = qobject_cast<T*>(qobj);
        if(targetObj == nullptr)
            throw QSException(InvalidQObject);
        fromJson(targetObj, json);
        return targetObj;
    }

    template <typename T> QSERIALIZER_EXPORT T * fromXml(const QDomNode &xml)
    {
        QObject * qobj = new T();
        T * targetObj = qobject_cast<T*>(qobj);
        if(targetObj == nullptr)
            throw QSException(InvalidQObject);
        fromJson(targetObj, xml);
        return targetObj;
    }

    /// \brief производит сериализацию QObject в JSON по имеющейся у объекта метаинформации
    QSERIALIZER_EXPORT QJsonObject toJson(QObject * obj);

    QSERIALIZER_EXPORT QDomDocument toXml(QObject * obj);

    QSERIALIZER_EXPORT void fromXml(QObject * obj, const QDomNode &xml);
}






#endif // QJSONMARSHALER_H

