#include <metakeepers.h>
#include <keepersfactory.h>
#include <qserializer.h>



std::pair<QString, QJsonValue> QMetaSimpleKeeper::toJson()
{
    QJsonValue result = QJsonValue::fromVariant(prop.read(linkedObj));
    return std::make_pair(QString(prop.name()), result);
}

void QMetaSimpleKeeper::fromJson(const QJsonValue &val)
{
    prop.write(linkedObj, QVariant(val));
}

std::pair<QString, QDomNode> QMetaSimpleKeeper::toXml()
{
    QDomDocument doc;
    QDomElement element = doc.createElement(prop.name());
    element.setAttribute("type",prop.typeName());
    QDomText valueOfProp = doc.createTextNode(prop.read(linkedObj).toString());
    element.appendChild(valueOfProp);
    doc.appendChild(element);
    return  std::make_pair(QString(prop.name()), QDomNode(doc));
}

void QMetaSimpleKeeper::fromXml(const QDomNode &node)
{
    if(!node.isNull() && node.isElement())
    {
        QDomElement domElement = node.toElement();
        if(domElement.tagName() == prop.name())
            prop.write(linkedObj, QVariant(domElement.text()));
    }
}











std::pair<QString, QJsonValue> QMetaObjectKeeper::toJson()
{
    QJsonObject result = QSerializer::toJson(linkedObj);;
    return std::make_pair(prop.name(),QJsonValue(result));
}

void QMetaObjectKeeper::fromJson(const QJsonValue &json)
{
    if(!json.isObject())
        throw QSException(JsonObjectExpected);

    QSerializer::fromJson(linkedObj, json.toObject());
}

std::pair<QString, QDomNode> QMetaObjectKeeper::toXml()
{
    return std::make_pair(QString(prop.name()), QSerializer::toXml(linkedObj));
}

void QMetaObjectKeeper::fromXml(const QDomNode &node)
{
    QSerializer::fromXml(linkedObj, node);
}
















std::pair<QString, QJsonValue> QMetaObjectArrayKeeper::toJson()
{
    return std::make_pair(QString(prop.name()), makeJson());
}

void QMetaObjectArrayKeeper::fromJson(const QJsonValue &json)
{
    if(!json.isArray())
        throw QSException(JsonArrayExpected);

    QJsonArray jsonArray = json.toArray();
    fillArray(jsonArray);
}

std::pair<QString, QDomNode> QMetaObjectArrayKeeper::toXml()
{
    return std::make_pair(QString(prop.name()), makeXml());
}

void QMetaObjectArrayKeeper::fromXml(const QDomNode &node)
{
    QDomNodeList nodesList = node.childNodes();
    fillArray(nodesList);
}

void QMetaObjectArrayKeeper::fillArray(const QJsonArray &jsonArray)
{
    QVariant property = prop.read(linkedObj);
    std::vector<QObject*> * objects = static_cast<std::vector<QObject*>*>(property.data());

    if(objects != nullptr && (objects->size() == 0 || qobject_cast<QObject*>(objects->at(0)) != nullptr))
    {
        for(int i = 0; i < jsonArray.size(); i ++)
        {
            if(!jsonArray.at(i).isObject())
                throw QSException(JsonObjectExpected);

            QSerializer::fromJson(objects->at(i), jsonArray.at(i).toObject());
        }
    }
    else
        throw QSException(InvalidQObject);
}

void QMetaObjectArrayKeeper::fillArray(const QDomNodeList &nodeList)
{
    QVariant property = prop.read(linkedObj);
    std::vector<QObject*> * objects = static_cast<std::vector<QObject*>*>(property.data());

    if(objects != nullptr && (objects->size() == 0 || qobject_cast<QObject*>(objects->at(0)) != nullptr))
    {
        for(int i = 0; i < nodeList.size() && i < objects->size(); i++)
            QSerializer::fromXml(objects->at(i), nodeList.at(i));
    }
    else
        throw QSException(InvalidQObject);;
}

QJsonValue QMetaObjectArrayKeeper::makeJson()
{
    QJsonArray result;
    QVariant property = prop.read(linkedObj);
    std::vector<QObject*> * objects = static_cast<std::vector<QObject*>*>(property.data());

    if(objects != nullptr && (objects->size() == 0 || qobject_cast<QObject*>(objects->at(0)) != nullptr))
    {
        for(QObject * qo : *objects)
            result.push_back(QSerializer::toJson(qo));
    }
    else
        throw QSException(InvalidQObject);
    return QJsonValue(result);
}

QDomNode QMetaObjectArrayKeeper::makeXml()
{
    QDomDocument doc;
    QDomElement element = doc.createElement(prop.name());
    element.setAttribute("type", "array");
    QVariant property = prop.read(linkedObj);
    std::vector<QObject*> * arrayObjects = static_cast<std::vector<QObject*>*>(property.data());

    if(arrayObjects != nullptr && (arrayObjects->size() == 0 || qobject_cast<QObject*>(arrayObjects->at(0)) != nullptr))
    {
        for(QObject * qo : *arrayObjects)
            element.appendChild(QSerializer::toXml(qo));
    }
    else
        throw QSException(InvalidQObject);

    doc.appendChild(element);
    return QDomNode(doc);
}
