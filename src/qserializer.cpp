#include "qserializer.h"
#include <map>
#include <keepersfactory.h>

void QSerializer::fromJson(QObject *obj, QJsonObject json)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(obj);

    QStringList keys = json.keys();
    for(QString &key : keys)
    {
        for(auto keeper : keepers)
        {
            if(keeper->toJson().first == key)
            {
                keeper->fromJson(json.value(key));
            }
        }
    }
}

QJsonObject QSerializer::toJson(QObject *obj)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(obj);

    QJsonObject json;
    for(auto keeper : keepers)
    {
        std::pair<QString, QJsonValue> keeperValue = keeper->toJson();
        json.insert(keeperValue.first, keeperValue.second);
    }

    return json;
}

QDomDocument QSerializer::toXml(QObject *obj)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(obj);

    QDomDocument doc;
    QDomElement el = doc.createElement(obj->metaObject()->className());
    for(auto keeper : keepers)
    {
        std::pair<QString, QDomNode> keeperValue = keeper->toXml();
        el.appendChild(keeperValue.second);
    }
    doc.appendChild(el);
    return doc;
}


void QSerializer::fromXml(QObject * obj, QDomNode & doc)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(obj);

    QDomNode xml = doc.firstChild();
    if(obj->metaObject()->className() == doc.firstChildElement().tagName())
    {
        QDomNode fieldNode = doc.firstChild().firstChild();
        while(!fieldNode.isNull())
        {
            if(fieldNode.isDocument())
            {
                QDomElement domElement = fieldNode.toDocument().documentElement();
                for(auto keeper : keepers)
                {
                    if(keeper->toXml().first == domElement.tagName())
                    {
                        keeper->fromXml(domElement);
                        break;
                    }
                }
            }
            fieldNode = fieldNode.nextSibling();
        }
    }
}























