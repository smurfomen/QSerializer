#ifndef SIMPLEKEEPERS_H
#define SIMPLEKEEPERS_H
#include <propertykeeper.h>
#include <QVariant>

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include <QMetaProperty>
#include <QMetaType>

/// \brief хранитель обычного поля с жесткой привязкой по указателю на поле
template<typename T>
class SimpleKeeper : public PropertyKeeper
{
public:
    SimpleKeeper(T * member, QString name)
    {
        this->key = name;
        this->linkedField = member;
    }

    /// \brief вернуть пару из ключа и JSON значения по связанному указателю
    std::pair<QString, QJsonValue> getValue()
    {
        QVariant var;
        var.setValue(*linkedField);
        return std::make_pair(key, QJsonValue::fromVariant(var));
    }

    /// \brief задать значение связанному указателю через JSON значение
    void setValue(QJsonValue val)
    {
        *linkedField = val.toVariant().value<T>();
    }

protected:
    QString key;
    T * linkedField;
};


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



#endif // SIMPLEKEEPERS_H
