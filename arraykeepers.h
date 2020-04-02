#ifndef ARRAYKEEPERS_H
#define ARRAYKEEPERS_H
#include <propertykeeper.h>
#include <vector>
#include <QMetaProperty>
#include <QMetaType>
#include <QVariant>

template<typename A>
class SimpleArrayKeeper : public PropertyKeeper
{
public:
    SimpleArrayKeeper(std::vector<A> * parr, QString pName)
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


template<typename T>
class QMetaArrayKeeper : public PropertyKeeper
{
public:
    QMetaArrayKeeper(QObject * obj, QMetaProperty prop)
    {
        this->obj = obj;
        this->prop = prop;
    }

    std::pair<QString, QJsonValue> getValue()
    {
        QJsonValue result;
        QJsonArray resultArr;
        QVariant var = prop.read(obj);
        std::vector<T> values = var.value<std::vector<T>>();
        for(auto item : values)
        {
            resultArr.push_back(QJsonValue::fromVariant(QVariant(item)));
        }

        result = QJsonValue(resultArr);

        return std::make_pair(QString(prop.name()), result);
    }

    void setValue(QJsonValue val)
    {
        QJsonArray arr = val.toArray();
        std::vector<T> v;
        for(auto item :arr)
        {
            QVariant var(item);
            v.push_back(var.value<T>());
        }
        prop.write(obj, QVariant::fromValue(v));
    }

private:
    QObject * obj;
    QMetaProperty prop;
};


#endif // ARRAYKEEPERS_H
