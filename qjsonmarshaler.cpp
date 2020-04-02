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
    std::vector<PropertyKeeper*> keepers = getMetaKeepersCollection(obj);

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
    std::vector<PropertyKeeper*> keepers = getMetaKeepersCollection(obj);

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

PropertyKeeper *QJsonMarshaler::getMetaKeeper(QObject *obj, QMetaProperty prop)
{
    int t_id = QMetaType::type(prop.typeName());
    if( t_id == qMetaTypeId<std::vector<int>>())
        return new QMetaArrayKeeper<int>(obj, prop);

    else if(t_id == qMetaTypeId<std::vector<QString>>())
        return new QMetaArrayKeeper<QString>(obj, prop);

    else if(t_id == qMetaTypeId<std::vector<double>>())
        return new QMetaArrayKeeper<double>(obj, prop);

    else if(t_id == qMetaTypeId<std::vector<bool>>())
        return new QMetaArrayKeeper<bool>(obj, prop);

    return new QMetaSimpleKeeper(obj,prop);
}

std::vector<PropertyKeeper *> QJsonMarshaler::getMetaKeepersCollection(QObject *obj)
{
    std::vector<PropertyKeeper*> keepers;
    for(int i = 0; i < obj->metaObject()->propertyCount(); i++)
    {
        if(obj->metaObject()->property(i).isUser(obj))
            keepers.push_back(getMetaKeeper(obj, obj->metaObject()->property(i)));
    }
    return keepers;
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
