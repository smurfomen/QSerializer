#ifndef QJSONMARSHALER_H
#define QJSONMARSHALER_H
#include <qserializerlib_global.h>
#include <keepersfactory.h>

class QSERIALIZERLIBSHARED_EXPORT QSerializer
{
public:
    QSerializer ();
    ~QSerializer();

    /// \brief конвертирует JSON в новый объект указанного типа и возвращает указатель на него
    template<typename T>
    static T * fromJson(QJsonObject json)
    {
        QObject * qobj = new T();
        T * targetObj = qobject_cast<T*>(qobj);
        if(targetObj == nullptr)
            throw (MarshalExeption(MarshalExeption::FailQObjectCast));
        QSerializer::fromJson(targetObj, json);
        return targetObj;
    }

    /// \brief производит сериализацию JSON в переданный QObject согласно имеющейся у объекта метаинформации
    static void fromJson(QObject * obj, QJsonObject json);

    /// \brief производит сериализацию QObject в JSON по имеющейся у объекта метаинформации
    static QJsonObject toJson(QObject * obj);
};





#endif // QJSONMARSHALER_H

