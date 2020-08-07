#include "qserializer.h"
#include <map>
#include "keepersfactory.h"

void QSerializer::fromJson(QObject *qo, const QJsonObject & json)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(qo);

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

void QSerializer::fromJson(QObject *qo, const QJsonDocument &json)
{
    QJsonObject jObject = json.object();
    return fromJson(qo, jObject);
}

void QSerializer::fromJson(QObject *qo, const QByteArray &json)
{
    QJsonObject jObject = QJsonDocument::fromJson(json).object();
    return fromJson(qo, jObject);
}

void QSerializer::fromJson(QObject *qo, const QString &json)
{
    const char * raw = json.toStdString().c_str();
    QJsonObject jObject = QJsonDocument::fromRawData(raw, static_cast<int>(strlen(raw))).object();
    return fromJson(qo, jObject);
}




QJsonObject QSerializer::toJson(QObject *qo)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(qo);

    QJsonObject json;
    for(auto keeper : keepers)
    {
        std::pair<QString, QJsonValue> keeperValue = keeper->toJson();
        json.insert(keeperValue.first, keeperValue.second);
    }

    return json;
}






void QSerializer::fromXml(QObject * qo, const QDomNode & doc)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(qo);

    if(qo->metaObject()->className() == doc.firstChildElement().tagName())
    {
        QDomNode fieldNode = doc.firstChild().firstChild();
        while(!fieldNode.isNull())
        {
            QDomElement domElement = fieldNode.toElement();
            for(auto keeper : keepers)
            {
                if(keeper->toXml().first == domElement.tagName())
                {
                    keeper->fromXml(domElement);
                    break;
                }
            }
            fieldNode = fieldNode.nextSibling();
        }
    }
}


QDomDocument QSerializer::toXml(QObject *qo)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(qo);

    QDomDocument doc;
    QDomElement el = doc.createElement(qo->metaObject()->className());
    for(auto keeper : keepers)
    {
        std::pair<QString, QDomNode> keeperValue = keeper->toXml();
        el.appendChild(keeperValue.second);
    }
    doc.appendChild(el);
    return doc;
}
