#ifndef ARRAYKEEPERS_H
#define ARRAYKEEPERS_H
#include <qserializerlib_global.h>
#include <serializerexception.h>
#include <propertykeeper.h>

#include <vector>

#include <QMetaProperty>
#include <QMetaType>
#include <QVariant>

#include <QDomElement>
#include <QDomAttr>
#include <QDomNode>
#include <QDomDocument>


/// \brief возвращает один из Meta хранителей, подходящих для переданного свойства связанного объекта, обернутый в базовый интерфейс хранителя
PropertyKeeper * getMetaKeeper(QObject *obj, QMetaProperty prop);

/// \brief возвраащет коллекцию Meta хранителей для объекта обернутых в базовый интерфейс хранителя
std::vector<PropertyKeeper*> getMetaKeepers(QObject * obj);

/// \brief абстрактный класс для Meta хранителей
class MetaKeeper : public PropertyKeeper
{
public:
    MetaKeeper(QObject * obj, QMetaProperty prop)
    {
        this->linkedObj = obj;
        this->prop = prop;
    }

    QObject * linkedObj;
    QMetaProperty prop;
};


class MetaObjectKeeper : public MetaKeeper
{
protected:
    MetaObjectKeeper(QObject * obj, QMetaProperty prop) : MetaKeeper(obj, prop) {}

    /// \brief накачивает переданный объект переданным JSONом
    void fillObjectFromJson(QObject * qo, const QJsonValue &json);

    /// \brief накачивает переданный объект переданным XML
    void fillObjectFromXml(QObject * qo, const QDomNode &xml);

    /// \brief выкачивает JSON из переданного объекта
    QJsonObject getJsonFromObject(QObject * qo);


    /// \brief выкачивает XML из переданного объекта
    QDomNode getXmlFromObject(QObject * qo);

};


/// \brief хранитель обычного поля (не массива) QMetaProperty у указанного QObject
class QMetaSimpleKeeper : public MetaKeeper
{
public:
    QMetaSimpleKeeper(QObject * obj, QMetaProperty prop): MetaKeeper(obj, prop){ }
    /// \brief вернуть пару из ключа и JSON значения из указанной QMetaProperty связанного объекта
    std::pair<QString, QJsonValue> toJson() override
    {
        QJsonValue result = QJsonValue::fromVariant(prop.read(linkedObj));
        return std::make_pair(QString(prop.name()), result);
    }

    /// \brief задать новое значение для связанной с хранимой QMetaProperty поля связанного объекта из JSON
    void fromJson(QJsonValue val) override
    {
        prop.write(linkedObj, QVariant(val));
    }

    std::pair<QString, QDomNode> toXml() override
    {
        QDomDocument doc;
        QDomElement element = doc.createElement(prop.name());
        element.setAttribute("type",prop.typeName());
        QDomText valueOfProp = doc.createTextNode(prop.read(linkedObj).toString());
        element.appendChild(valueOfProp);
        doc.appendChild(element);
        return  std::make_pair(QString(prop.name()), QDomNode(doc));
    }

    void fromXml(const QDomNode &node) override
    {
        if(!node.isNull() && node.isElement())
        {
            QDomElement domElement = node.toElement();
            if(domElement.tagName() == prop.name())
            {
                prop.write(linkedObj, QVariant(domElement.text()));
            }
        }
    }

};




/// \brief хранитель массивов типа А для поля QMetaProperty у указанного QObject
template<typename A>
class QMetaArrayKeeper : public MetaKeeper
{
public:
    QMetaArrayKeeper(QObject * obj, QMetaProperty prop): MetaKeeper(obj, prop) { }
    /// \brief возвращает пару из имени хранимого поля и cформированного в массив QJsonValue
    std::pair<QString, QJsonValue> toJson() override
    {
        QVariant property = prop.read(linkedObj);
        std::vector<A> values = property.value<std::vector<A>>();

        QJsonArray result;
        for(auto val : values)
        {
            result.push_back(QJsonValue::fromVariant(QVariant(val)));
        }

        return std::make_pair(QString(prop.name()), QJsonValue(result));
    }

    /// \brief изменение значения поля из JSON по переданному QJsonValue
    void fromJson(QJsonValue json) override
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

    std::pair<QString, QDomNode> toXml() override
    {
        /// берем массив объектов А и наполняем документ дочерними элементами типа с атрибутами 'type' и 'index'
        /// Возвращаем имя опции и ноду в виде документа.
        QVariant property = prop.read(linkedObj);
        std::vector<A> array = property.value<std::vector<A>>();
        QDomDocument doc;
        QDomElement arrayXml = doc.createElement(prop.name());
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
class QMetaObjectKeeper : public MetaObjectKeeper
{
    /// такой тип хранителя предназначен для хранения вложенных объектов, т.е. объектов, унаследованных от QObject
    /// вместо изменения свойства напрямую, он разбирает хранимый объект на элементарные составляющие или такие же хранители объектов
    /// и служит "маршрутизатором" JSON значений внутрь хранимых объектов и обратно.
    /// QMetaObjectKeeper, фактически, является воплощением QMetaObject для связанного с ним вложенного объекта.
public:
    QMetaObjectKeeper(QObject * obj, QMetaProperty prop): MetaObjectKeeper(obj, prop) { }
    /// \brief возвращает пару из имени хранимого поля и упакованного в QJsonValue объекта JSON
    std::pair<QString, QJsonValue> toJson() override
    {
        QJsonObject result = getJsonFromObject(linkedObj);
        return std::make_pair(prop.name(),QJsonValue(result));
    }

    /// \brief изменение значения хранимого объекта по переданному QJsonValue, в который упакован объект
    void fromJson(QJsonValue json) override
    {
        fillObjectFromJson(linkedObj, json);
    }

    std::pair<QString, QDomNode> toXml() override
    {
        return std::make_pair(QString(prop.name()), getXmlFromObject(linkedObj));
    }

    void fromXml(const QDomNode &node) override
    {
        fillObjectFromXml(linkedObj, node);
    }
};




/// \brief хранитель массивов типа QObject или базового от него
class QMetaObjectArrayKeeper : public MetaObjectKeeper
{
    /// Стоит оговориться, на данный момент возможна работа только с коллекциями указателей на объекты, а не с непосредственно самими объектами
    /// Объекты должны саморазрушаться
public:
    QMetaObjectArrayKeeper(QObject * obj, QMetaProperty prop) : MetaObjectKeeper(obj, prop){ }
    /// \brief возвращает пару из имени хранимого поля и упакованного в QJsonValue массива объектов
    std::pair<QString, QJsonValue> toJson() override
    {
        return std::make_pair(QString(prop.name()), makeJson());
    }

    /// \brief изменение значения хранимого массива объектов по переданному JSON массиву обернутому в QJsonValue
    void fromJson(QJsonValue json) override
    {
        if(!json.isArray())
            throw QSException(JsonArrayExpected);

        QJsonArray jsonArray = json.toArray();
        fillArray(jsonArray);
    }

    std::pair<QString, QDomNode> toXml() override
    {
        return  std::make_pair(QString(prop.name()), makeXml());
    }

    void fromXml(const QDomNode &node) override
    {
        QDomNodeList nodesList = node.childNodes();
        fillArray(nodesList);
    }



private:

    void fillArray(const QJsonArray &jsonArray)
    {
        QVariant property = prop.read(linkedObj);
        std::vector<QObject*> * objects = static_cast<std::vector<QObject*>*>(property.data());

        if(objects != nullptr && (objects->size() == 0 || qobject_cast<QObject*>(objects->at(0)) != nullptr))
        {
            for(int i = 0; i < jsonArray.size() && i < objects->size(); i ++)
                fillObjectFromJson(objects->at(i),jsonArray.at(i));
        }
        else
            throw QSException(InvalidQObject);
    }

    void fillArray(const QDomNodeList & nodeList)
    {
        QVariant property = prop.read(linkedObj);
        std::vector<QObject*> * arrayObjects = static_cast<std::vector<QObject*>*>(property.data());

        if(arrayObjects != nullptr && (arrayObjects->size() == 0 || qobject_cast<QObject*>(arrayObjects->at(0)) != nullptr))
        {
            for(int i = 0; i < nodeList.size() && i < arrayObjects->size(); i++)
                fillObjectFromXml(arrayObjects->at(i), nodeList.at(i));
        }
        else
            throw QSException(InvalidQObject);;
    }


    QJsonValue makeJson()
    {
        QJsonArray result;
        QVariant property = prop.read(linkedObj);
        std::vector<QObject*> * objects = static_cast<std::vector<QObject*>*>(property.data());

        if(objects != nullptr && (objects->size() == 0 || qobject_cast<QObject*>(objects->at(0)) != nullptr))
        {
            for(QObject * qo : *objects)
                result.push_back(getJsonFromObject(qo));
        } else throw QSException(InvalidQObject);
        return QJsonValue(result);
    }

    QDomNode makeXml()
    {
        QDomDocument doc;
        QDomElement element = doc.createElement(prop.name());
        QVariant property = prop.read(linkedObj);
        std::vector<QObject*> * arrayObjects = static_cast<std::vector<QObject*>*>(property.data());

        if(arrayObjects != nullptr && (arrayObjects->size() == 0 || qobject_cast<QObject*>(arrayObjects->at(0)) != nullptr))
        {
            for(QObject * qo : *arrayObjects)
                element.appendChild(getXmlFromObject(qo));
        }
        else
            throw QSException(InvalidQObject);

        doc.appendChild(element);
        return QDomNode(doc);
    }
};

#endif // ARRAYKEEPERS_H
