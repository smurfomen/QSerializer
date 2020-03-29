#include "qjsonkeeper.h"

IntPropertyKeeper::IntPropertyKeeper(QString name, int *member)
{
    this->pName = name;
    this->pMember = member;
}

std::pair<QString, QJsonValue> IntPropertyKeeper::getValue()
{
    return std::make_pair(pName, QJsonValue(*pMember));
}

void IntPropertyKeeper::setValue(QJsonValue val)
{
    *pMember = val.toInt();
}







BoolPropertyKeeper::BoolPropertyKeeper(QString name, bool *member)
{
    this->pName = name;
    this->pMember = member;
}

std::pair<QString, QJsonValue> BoolPropertyKeeper::getValue()
{
    return std::make_pair(pName, QJsonValue(*pMember));
}

void BoolPropertyKeeper::setValue(QJsonValue val)
{

    *pMember = val.toBool();
}







StringPropertyKeeper::StringPropertyKeeper(QString name, QString *member)
{
    this->pName = name;
    this->pMember = member;
}

std::pair<QString, QJsonValue> StringPropertyKeeper::getValue()
{
    return std::make_pair(pName, QJsonValue(*pMember));

}

void StringPropertyKeeper::setValue(QJsonValue val)
{
    *pMember = val.toString();
}
