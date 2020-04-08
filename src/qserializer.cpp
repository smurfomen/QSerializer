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
            if(keeper->getValue().first == key)
            {
                keeper->setValue(json.value(key));
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
        std::pair<QString, QJsonValue> keeperValue = keeper->getValue();
        json.insert(keeperValue.first, keeperValue.second);
    }

    return json;
}
