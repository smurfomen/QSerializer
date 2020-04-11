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
    QSERIALIZER_EXPORT void fromJson(QObject * obj, QJsonObject json);

    /// \brief конвертирует JSON в новый объект указанного типа и возвращает указатель на него
    template<typename T> QSERIALIZER_EXPORT T * fromJson(QJsonObject json)
    {
        QObject * qobj = new T();
        T * targetObj = qobject_cast<T*>(qobj);
        if(targetObj == nullptr)
            throw QSException(InvalidQObject);
        fromJson(targetObj, json);
        return targetObj;
    }

    /// \brief производит сериализацию QObject в JSON по имеющейся у объекта метаинформации
    QSERIALIZER_EXPORT QJsonObject toJson(QObject * obj);

    QSERIALIZER_EXPORT QDomDocument toXml(QObject * obj);

    QSERIALIZER_EXPORT void fromXml(QObject * obj, QDomNode &xml);
}






#endif // QJSONMARSHALER_H

