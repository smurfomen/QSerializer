#ifndef ARRAYKEEPERS_H
#define ARRAYKEEPERS_H
#include <propertykeeper.h>
#include <vector>
#include <QMetaProperty>
#include <QMetaType>
#include <QVariant>

/// \brief простой хранитель массивов типа А с жесткой привязкой к массиву объекта по указателю
template<typename A>
class SimpleArrayKeeper : public PropertyKeeper
{
public:
    SimpleArrayKeeper(std::vector<A> * parr, QString pName)
    {
        this->key = pName;
        this->linkedArray = parr;
    }

    /// \brief вернуть пару из ключа и JSON массива по связанному указателю
    std::pair<QString, QJsonValue> getValue()
    {
        QJsonArray arr;
        for(int i = 0; i< linkedArray->size(); i++)
        {
            QVariant var;
            var.setValue(linkedArray->at(i));
            arr.push_back(QJsonValue::fromVariant(var));
        }
        return std::make_pair(key,QJsonValue(arr));
    }

    /// \brief перенаполнение связанного указателем массива из JSON
    void setValue(QJsonValue val)
    {
        QJsonArray array = val.toArray();
        if(array.isEmpty())
            return;

        linkedArray->clear();
        for(QJsonValue item : array)
        {
            linkedArray->push_back(item.toVariant().value<A>());
        }
    }

private:
    QString key;
    std::vector<A> * linkedArray;

};


/// \brief хранитель массивов типа А для поля QMetaProperty у указанного QObject
template<typename A>
class QMetaArrayKeeper : public PropertyKeeper
{
public:
    QMetaArrayKeeper(QObject * obj, QMetaProperty prop)
    {
        this->linkedObj = obj;
        this->prop = prop;
    }

    /// \brief вернуть пару из ключа и JSON массива из указанной QMetaProperty
    std::pair<QString, QJsonValue> getValue()
    {
        QJsonArray result;
        QVariant property = prop.read(linkedObj);
        std::vector<A> values = property.value<std::vector<A>>();
        for(auto val : values)
        {
            result.push_back(QJsonValue::fromVariant(QVariant(val)));
        }

        return std::make_pair(QString(prop.name()), QJsonValue(result));
    }

    /// \brief изменение значения связанной QMetaProperty у связанного объекта из переданного JSON
    void setValue(QJsonValue val)
    {
        QJsonArray arr = val.toArray();
        std::vector<A> v;
        for(auto item :arr)
        {
            QVariant var(item);
            v.push_back(var.value<A>());
        }
        prop.write(linkedObj, QVariant::fromValue(v));
    }

private:    
    QObject * linkedObj;
    QMetaProperty prop;
};


#endif // ARRAYKEEPERS_H
