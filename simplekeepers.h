#ifndef SIMPLEKEEPERS_H
#define SIMPLEKEEPERS_H

#include <propertykeeper.h>
#include <QVariant>

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include <QMetaProperty>
#include <QMetaType>

template<typename T>
class SimpleKeeper : public PropertyKeeper
{
public:
    SimpleKeeper(T * member, QString name)
    {
        this->pName = name;
        this->pMember = member;
    }
    std::pair<QString, QJsonValue> getValue()
    {
        QVariant var;
        var.setValue(*pMember);
        return std::make_pair(pName, QJsonValue::fromVariant(var));
    }

    void setValue(QJsonValue val)
    {
        *pMember = val.toVariant().value<T>();
    }

protected:
    QString pName;
    T * pMember;
};


class QMetaSimpleKeeper : public PropertyKeeper
{
public:
    QMetaSimpleKeeper(QObject * obj, QMetaProperty prop)
    {
        this->obj = obj;
        this->prop = prop;
    }
    std::pair<QString, QJsonValue> getValue()
    {
        QJsonValue result = QJsonValue::fromVariant(prop.read(obj));
        return std::make_pair(QString(prop.name()), result);
    }

    void setValue(QJsonValue val)
    {
        prop.write(obj, QVariant(val));
    }

private:
    QObject * obj;
    QMetaProperty prop;
};



#endif // SIMPLEKEEPERS_H
