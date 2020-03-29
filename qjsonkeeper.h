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


class BoolPropertyKeeper : public PropertyKeeper
{
public:
    BoolPropertyKeeper(QString name, bool * member);
    std::pair<QString, QJsonValue> getValue();

    void setValue(QJsonValue val);

private:
    QString pName;
    bool * pMember;
};


class IntPropertyKeeper : public PropertyKeeper
{
public:
    IntPropertyKeeper(QString name, int * member);
    std::pair<QString, QJsonValue> getValue();

    void setValue(QJsonValue val);

private:
    QString pName;
    int * pMember;
};


class StringPropertyKeeper : public PropertyKeeper
{
public:
    StringPropertyKeeper(QString name, QString * member);
    std::pair<QString, QJsonValue> getValue();

    void setValue(QJsonValue val);

private:
    QString pName;
    QString * pMember;
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



template<typename T>
PropertyKeeper * getIJ(T * pMember, QString pName)
{
    if (typeid(T) == typeid(bool))
        return new BoolPropertyKeeper(pName, (bool*)pMember);

    else if (typeid (T) == typeid (int))
        return new IntPropertyKeeper(pName, (int*)pMember);

    else if (typeid (T) == typeid (QString))
        return new StringPropertyKeeper(pName, (QString*)pMember);

    else
        throw MarshalExeption(MarshalExeption::UndefinedType);
}



#endif // QJSONKEEPER_H
