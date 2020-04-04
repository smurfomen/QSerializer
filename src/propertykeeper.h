#ifndef PROPERTYKEEPER_H
#define PROPERTYKEEPER_H

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

// предоставляет интерфейс для доступа к полю и связанной с ним JSON аннотацией
class PropertyKeeper
{
public:
    virtual ~PropertyKeeper() = default;
    virtual std::pair<QString, QJsonValue> getValue() = 0;
    virtual void setValue(QJsonValue) = 0;
};


// класс исключения
class MarshalExeption
{
public:
    enum Type
    {
        NotFoundElement,
        FailQObjectCast,
        UndefinedType
    };
    MarshalExeption(Type t) : err(t) { }

    QString what()
    {
        return  "";
    }

private:
    Type err;
};



#endif // PROPERTYKEEPER_H
