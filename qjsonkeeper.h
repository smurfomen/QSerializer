#ifndef QJSONKEEPER_H
#define QJSONKEEPER_H

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
class PropertyKeeper
{
public:
    virtual ~PropertyKeeper() = default;
    virtual std::pair<QString, QJsonValue> getValue() = 0;
    virtual void setValue(QJsonValue) = 0;
};



class MarshalExeption
{
public:
    enum Type
    {
        NotFoundMarshallingElement,
        UndefinedType
    };
    MarshalExeption(Type t) : err(t){

    }

    QString what()
    {
        return  "";
    }

private:
    Type err;
};






#endif // QJSONKEEPER_H
