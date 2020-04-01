#ifndef ARRAYKEEPER_H
#define ARRAYKEEPER_H
#include <qjsonkeeper.h>
#include <vector>
#include <QJsonArray>
#include <QVariant>

template<typename A>
class ArrayPropertyKeeper : public PropertyKeeper
{
public:
    ArrayPropertyKeeper(std::vector<A> * parr, QString pName)
    {
        this->pName = pName;
        this->pArray = parr;
    }

    std::pair<QString, QJsonValue> getValue() override
    {
        QJsonArray arr;
        for(int i = 0; i< pArray->size(); i++)
        {
            QVariant var;
            var.setValue(pArray->at(i));
            arr.push_back(QJsonValue::fromVariant(var));
        }
        return std::make_pair(pName,QJsonValue(arr));
    }

    void setValue(QJsonValue val)
    {
        QJsonArray array = val.toArray();
        for(QJsonValue item : array)
        {
            pArray->push_back(item.toVariant().value<A>());
        }
    }

private:
    QString pName;
    std::vector<A> * pArray;
};



#endif // ARRAYKEEPER_H
