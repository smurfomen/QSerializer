#include "qjsonmarshaler.h"
#include <map>
QJsonMarshaler::QJsonMarshaler(){

}

QJsonMarshaler::~QJsonMarshaler()
{
    for(auto property : propertyes)
        delete property;
}


std::map<QString,QJsonMarshaler*> marshalers;
void QJsonMarshaler::setJsonObjectAnotation(QString annotation)
{
    this->objAnnotation = annotation;
    marshalers.insert(std::make_pair(objAnnotation, this));
}

void QJsonMarshaler::UnmarshalDocument(QByteArray jsonArr)
{
    QJsonDocument doc = QJsonDocument::fromBinaryData(jsonArr);
    QJsonMarshaler::UnmarshalDocument(doc);
}

void QJsonMarshaler::UnmarshalDocument(QString jsonStr)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
    QJsonMarshaler::UnmarshalDocument(doc);
}

void QJsonMarshaler::UnmarshalDocument(QJsonDocument doc)
{
    QJsonObject document = doc.object();
    foreach(auto marshaler, marshalers)
    {
        QString objectAnnotate = marshaler.first;
        QJsonObject obj = document.value(objectAnnotate).toObject();
        if(!obj.isEmpty())
        {
            QJsonMarshaler * m = marshaler.second;
            m->Unmarshal(obj);
        }
    }
}

void QJsonMarshaler::Unmarshal(QObject *obj, QJsonObject json)
{
    KeepersFactory factory;
    // точка для рекурсии
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(obj);

    QStringList keys = json.keys();
    for(QString &key : keys)
    {
        for(auto keeper : keepers)
        {
            if(keeper->getValue().first == key)
            {
                keeper->setValue(json.value(key));
            }
        }
    }
}

QJsonObject QJsonMarshaler::Marshal(QObject *obj)
{
    KeepersFactory factory;
    std::vector<PropertyKeeper*> keepers = factory.getKeepersForObject(obj);

    QJsonObject json;
    for(auto keeper : keepers)
    {
        std::pair<QString, QJsonValue> keeperValue = keeper->getValue();
        json.insert(keeperValue.first, keeperValue.second);
    }

    return json;
}

QJsonObject QJsonMarshaler::Marshal()
{
    QVariantMap map;
    for(PropertyKeeper * item : propertyes)
    {
        map.insert(item->getValue().first, item->getValue().second);
    }
    return QJsonObject::fromVariantMap(map);
}


void QJsonMarshaler::Unmarshal(QJsonObject json)
{
    for(PropertyKeeper * item : propertyes)
    {
        QString key = item->getValue().first;
        QJsonValue val = json.value(key);
        if(val.isNull())
        {
            continue;
        }
        item->setValue(val);
    }
}
