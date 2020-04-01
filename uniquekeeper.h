#ifndef UNIQUEKEEPER_H
#define UNIQUEKEEPER_H

#include <qjsonkeeper.h>
#include <QVariant>
#include <QJsonArray>
template<typename T>
class UniquePropertyKeeper : public PropertyKeeper
{
public:
    UniquePropertyKeeper(T * member, QString name)
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



#endif // UNIQUEKEEPER_H
