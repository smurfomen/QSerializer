#ifndef PROPERTYKEEPER_H
#define PROPERTYKEEPER_H
#include <qserializerlib_global.h>

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

#include <QDomElement>
#include <QDomNode>

// предоставляет интерфейс для доступа к полю и связанной с ним JSON аннотацией
class PropertyKeeper
{
public:
    virtual ~PropertyKeeper() = default;
    virtual std::pair<QString, QJsonValue> toJson() = 0;
    virtual void fromJson(QJsonValue) = 0;

    virtual std::pair<QString, QDomNode> toXml() = 0;
    virtual void fromXml(const QDomNode &node) = 0;
};

#endif // PROPERTYKEEPER_H
