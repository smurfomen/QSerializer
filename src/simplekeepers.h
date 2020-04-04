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


/// \brief хранитель обычного поля (не массива) QMetaProperty у указанного QObject
class QMetaSimpleKeeper : public PropertyKeeper
{
public:
    QMetaSimpleKeeper(QObject * obj, QMetaProperty prop)
    {
        this->linkedObj = obj;
        this->prop = prop;
    }

    /// \brief вернуть пару из ключа и JSON значения из указанной QMetaProperty связанного объекта
    std::pair<QString, QJsonValue> getValue()
    {
        QJsonValue result = QJsonValue::fromVariant(prop.read(linkedObj));
        return std::make_pair(QString(prop.name()), result);
    }

    /// \brief задать новое значение для связанной с хранимой QMetaProperty поля связанного объекта из JSON
    void setValue(QJsonValue val)
    {
        prop.write(linkedObj, QVariant(val));
    }

private:
    QObject * linkedObj;
    QMetaProperty prop;
};



#endif // SIMPLEKEEPERS_H
