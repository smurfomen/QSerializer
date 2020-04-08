#ifndef PROPERTYKEEPER_H
#define PROPERTYKEEPER_H
#include <qserializerlib_global.h>

#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>

// предоставляет интерфейс для доступа к полю и связанной с ним JSON аннотацией
class PropertyKeeper
{
public:
    virtual ~PropertyKeeper() = default;
    virtual std::pair<QString, QJsonValue> getValue() = 0;
    virtual void setValue(QJsonValue) = 0;
};

#endif // PROPERTYKEEPER_H
