#ifndef METAKEEPERS_H
#define METAKEEPERS_H
#include "qserializerlib_global.h"
#include "serializerexception.h"
#include "propertykeeper.h"

#include <vector>

#include <QMetaProperty>
#include <QMetaType>
#include <QVariant>

#include <QDomElement>
#include <QDomAttr>
#include <QDomNode>
#include <QDomDocument>



/// \brief абстрактный класс для Meta хранителей, не реализует интерфейс PropertyKeeper
/// хранит свойство объекта и указатель на сам объект для доступа к свойству.
/// предназначен для наследования другими конкретными реализациями интерфейса PropertyKeeper в угоду меньшего дублирования кода
class MetaKeeper : public PropertyKeeper
{
public:
    MetaKeeper(QObject * obj, QMetaProperty prop)
    {
        this->linkedObj = obj;
        this->prop = prop;
    }

protected:
    QObject * linkedObj;
    QMetaProperty prop;
};




/// \brief хранитель для примитивных значений (integer, boolean, double, text)
class QMetaSimpleKeeper : public MetaKeeper
{
public:
    QMetaSimpleKeeper(QObject * obj, QMetaProperty prop): MetaKeeper(obj, prop){ }
    /// \brief вернуть пару из ключа и JSON значения из указанной QMetaProperty связанного объекта
    std::pair<QString, QJsonValue> toJson() override;

    /// \brief задать новое значение для связанной с хранимой QMetaProperty поля связанного объекта из JSON
    void fromJson(const QJsonValue &val) override;

    /// \brief возвращает пару из имени и XML хранимого примитивного значения
    std::pair<QString, QDomNode> toXml() override;

    /// \brief изменяет хранимое примитивное значение согласно переданному XML
    void fromXml(const QDomNode &node) override;
};






/// \brief хранитель массивов шаблонного типа для примитивных значений (integer, boolean, double, text)
template<typename A>
class QMetaArrayKeeper : public MetaKeeper
{
public:
    QMetaArrayKeeper(QObject * obj, QMetaProperty prop): MetaKeeper(obj, prop) { }

    /// \brief возвращает пару из имени и JSON хранимого массива примитивных значений
    std::pair<QString, QJsonValue> toJson() override
    {
        QVariant property = prop.read(linkedObj);
        std::vector<A> values = property.value<std::vector<A>>();

        QJsonArray result;
        for(auto val : values)
            result.push_back(QJsonValue::fromVariant(QVariant(val)));

        return std::make_pair(QString(prop.name()), QJsonValue(result));
    }

    /// \brief изменяет хранимый массив примитивных значений согласно переданному JSON
    void fromJson(const QJsonValue & json) override
    {
        /// переводим значение в массив и наполняем вектор типа А, с которым был создан хранитель значениями из JSON массива
        /// в конце записываем в хранимое свойство связанного объекта этот вектор
        if(!json.isArray())
            throw QSException(JsonArrayExpected);

        QJsonArray arr = json.toArray();
        std::vector<A> v;
        for(auto item :arr)
        {
            QVariant itemVariant(item);
            v.push_back(itemVariant.value<A>());
        }
        prop.write(linkedObj, QVariant::fromValue(v));
    }

    /// \brief возвращает пару из имени и XML хранимого массива примитивных значений
    std::pair<QString, QDomNode> toXml() override
    {
        /// берем массив объектов А и наполняем документ дочерними элементами типа с атрибутами 'type' и 'index'
        /// Возвращаем имя опции и ноду в виде документа.
        QVariant property = prop.read(linkedObj);
        std::vector<A> array = property.value<std::vector<A>>();
        QDomDocument doc;
        QDomElement arrayXml = doc.createElement(prop.name());
        arrayXml.setAttribute("type", "array");
        for(int i = 0; i < array.size(); i++)
        {
            A item = array.at(i);
            QDomElement itemXml = doc.createElement("item");
            itemXml.setAttribute("type", QMetaType::typeName(qMetaTypeId<A>()));
            itemXml.setAttribute("index", i);

            QVariant variantItem = QVariant::fromValue(item);
            itemXml.appendChild(doc.createTextNode(variantItem.toString()));
            arrayXml.appendChild(itemXml);
        }
        doc.appendChild(arrayXml);
        return  std::make_pair(QString(prop.name()), QDomNode(doc));
    }

    /// \brief изменяет хранимый массив примитивных значений согласно переданному XML
    void fromXml(const QDomNode &node) override
    {
        std::vector<A> v;
        QDomNode domNode = node.firstChild();
        while(!domNode.isNull())
        {
            if(domNode.isElement())
            {
                QDomElement domElement = domNode.toElement();
                v.push_back(QVariant(domElement.text()).value<A>());
            }
            domNode = domNode.nextSibling();
        }
        prop.write(linkedObj, QVariant::fromValue(v));
    }
};






/// \brief хранитель полей класса, типы которых унаследованны от QObject
class QMetaObjectKeeper : public MetaKeeper
{
    /// такой тип хранителя предназначен для хранения вложенных объектов, т.е. объектов, унаследованных от QObject
    /// вместо изменения свойства напрямую, он разбирает хранимый объект на элементарные составляющие или такие же хранители объектов
    /// и служит "маршрутизатором" JSON значений внутрь хранимых объектов и обратно.
    /// QMetaObjectKeeper, фактически, является воплощением QMetaObject для связанного с ним вложенного объекта.
public:
    QMetaObjectKeeper(QObject * obj, QMetaProperty prop): MetaKeeper(obj, prop) { }
    /// \brief возвращает пару из имени и JSON хранимого объекта
    std::pair<QString, QJsonValue> toJson() override;

    /// \brief изменяет объект согласно переданному JSON
    void fromJson(const QJsonValue & json) override;

    /// \brief возвращает пару из имени и XML хранимого объекта
    std::pair<QString, QDomNode> toXml() override;

    /// \brief изменяет объект согласно переданному XML
    void fromXml(const QDomNode &node) override;
};







/// \brief хранитель массивов типа QObject или базового от него
class QMetaObjectArrayKeeper : public MetaKeeper
{
    /// Стоит оговориться, на данный момент возможна работа только с коллекциями указателей на объекты, а не с непосредственно самими объектами.
    /// Если есть нужда работать с объектом напрямую - в Q_PROPERTY необходимо явно указать методы доступа и записи вместо флага MEMBER
public:
    QMetaObjectArrayKeeper(QObject * obj, QMetaProperty prop) : MetaKeeper(obj, prop){ }

    /// \brief возвращает пару из имени хранимого поля и упакованного в QJsonValue массива объектов
    std::pair<QString, QJsonValue> toJson() override;

    /// \brief изменяет хранимый массив объектов согласно переданному JSON
    void fromJson(const QJsonValue & json) override;

    /// \brief возвращает пару из имени и XML хранимого массива объектов
    std::pair<QString, QDomNode> toXml() override;

    /// \brief изменяет хранимый массив объектов согласно переданному XML
    /// ВАЖНО: v.1.0.2. может только изменять существующие объекты, но не добавлять новые.
    /// Это распространяется только на непремитивные типы наследованные от QObject
    void fromXml(const QDomNode &node) override;

private:

    void fillArray(const QJsonArray &jsonArray);

    void fillArray(const QDomNodeList & nodeList);

    QJsonValue makeJson();

    QDomNode makeXml();

};

#endif // METAKEEPERS_H
