#ifndef ARRAYKEEPERS_H
#define ARRAYKEEPERS_H
#include <propertykeeper.h>
#include <vector>
#include <QMetaProperty>
#include <QMetaType>
#include <QVariant>
#include <type_traits>

/// \brief возвращает один из Meta хранителей, подходящих для переданного свойства связанного объекта, обернутый в базовый интерфейс хранителя
PropertyKeeper * getMetaKeeper(QObject *obj, QMetaProperty prop);

/// \brief возвраащет коллекцию Meta хранителей для объекта обернутых в базовый интерфейс хранителя
std::vector<PropertyKeeper*> getMetaKeepers(QObject * obj);

/// \brief абстрактный класс для Meta хранителей
class MetaPropertyKeeper : public PropertyKeeper
{
public:
    MetaPropertyKeeper(QObject * obj, QMetaProperty prop)
    {
        this->linkedObj = obj;
        this->prop = prop;
    }

protected:
    QObject * linkedObj;
    QMetaProperty prop;
};

/// \brief хранитель обычного поля (не массива) QMetaProperty у указанного QObject
class QMetaSimpleKeeper : public MetaPropertyKeeper
{
public:
    QMetaSimpleKeeper(QObject * obj, QMetaProperty prop): MetaPropertyKeeper(obj, prop){ }
    /// \brief вернуть пару из ключа и JSON значения из указанной QMetaProperty связанного объекта
    std::pair<QString, QJsonValue> getValue() override
    {
        QJsonValue result = QJsonValue::fromVariant(prop.read(linkedObj));
        return std::make_pair(QString(prop.name()), result);
    }

    /// \brief задать новое значение для связанной с хранимой QMetaProperty поля связанного объекта из JSON
    void setValue(QJsonValue val) override
    {
        prop.write(linkedObj, QVariant(val));
    }
};



/// \brief хранитель массивов типа А для поля QMetaProperty у указанного QObject
template<typename A>
class QMetaArrayKeeper : public MetaPropertyKeeper
{
public:
    QMetaArrayKeeper(QObject * obj, QMetaProperty prop): MetaPropertyKeeper(obj, prop) { }
    /// \brief возвращает пару из имени хранимого поля и cформированного в массив QJsonValue
    std::pair<QString, QJsonValue> getValue() override
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

    /// \brief изменение значения поля из JSON по переданному QJsonValue
    void setValue(QJsonValue val) override
    {
        /// переводим значение в массив и наполняем вектор типа А, с которым был создан хранитель значениями из JSON массива
        /// в конце записываем в хранимое свойство связанного объекта этот вектор
        QJsonArray arr = val.toArray();
        std::vector<A> v;
        for(auto item :arr)
        {
            QVariant itemVariant(item);
            v.push_back(itemVariant.value<A>());
        }
        prop.write(linkedObj, QVariant::fromValue(v));
    }
};


/// \brief хранитель полей класса, типы которых унаследованны от QObject
class QMetaObjectKeeper : public MetaPropertyKeeper
{
    /// такой тип хранителя предназначен для хранения вложенных объектов, т.е. объектов, унаследованных от QObject
    /// вместо изменения свойства напрямую, он разбирает хранимый объект на элементарные составляющие или такие же хранители объектов
    /// и служит "маршрутизатором" JSON значений внутрь хранимых объектов и обратно.
    /// QMetaObjectKeeper, фактически, является воплощением QMetaObject для связанного с ним вложенного объекта.
public:
    QMetaObjectKeeper(QObject * obj, QMetaProperty prop): MetaPropertyKeeper(obj, prop) { }
    /// \brief возвращает пару из имени хранимого поля и упакованного в QJsonValue объекта JSON
    std::pair<QString, QJsonValue> getValue() override
    {
        /// взять коллекцию простых хранителей, хранящих в себе элементарные данные и взять у каждого хранителя его ключ и JSON значение
        /// составить из этих значений объект.
        /// внутри коллекции хранителей может быть точно такой же хранитель (QMetaObjectKeeper), которых хранит в себе еще один вложенный объект и т.д.
        /// сбор информации будет продолжаться до тех пор, пока не упрется в последние элементарные хранители самых глубоковложенных объектов
        /// и так же JSON значения (QJsonValue) будут возвращаться и сливаться в объекты до тех пор, пока не вернутся в корень для возврата вернут сформированного JSON объекта
        std::vector<PropertyKeeper*> keepers = getMetaKeepers(linkedObj);
        QJsonObject result;
        for(auto keeper : keepers)
        {
            std::pair<QString, QJsonValue> keeperValue = keeper->getValue();
            result.insert(keeperValue.first, keeperValue.second);
        }
        return std::make_pair(prop.name(),QJsonValue(result));
    }

    /// \brief изменение значения хранимого объекта по переданному QJsonValue, в который упакован объект
    void setValue(QJsonValue val) override
    {
        /// перебор всех ключей и всех хранителей, если нашли хранителя с подходящим ключом - отдаем значение по ключу в найденный хранитель
        /// дальше он сам разберется что ему с ним делать, в зависимости от типа хранителя за прослойкой интерфейса
        QJsonObject jsonObject = val.toObject();
        QStringList keys = jsonObject.keys();
        std::vector<PropertyKeeper*> keepers = getMetaKeepers(linkedObj);

        for(auto jsonKey : keys)
        {
            for(auto keeper : keepers)
            {
                QString keeperKey = keeper->getValue().first;
                if(jsonKey == keeperKey)
                {
                    keeper->setValue(jsonObject.value(jsonKey));
                }
            }
        }
    }
};

#endif // ARRAYKEEPERS_H
