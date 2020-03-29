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

QJsonObject QJsonMarshaler::Marshal()
{
    QJsonObject obj;
    for(PropertyKeeper * item : propertyes)
    {
        obj.insert(item->getValue().first, item->getValue().second);
    }
    return obj;
}

void QJsonMarshaler::Unmarshal(QJsonObject obj)
{
    for(PropertyKeeper * item : propertyes)
    {
        QString key = item->getValue().first;
        QJsonValue val = obj.value(key);
        if(val.isNull())
            continue;
        item->setValue(val);
    }
}
