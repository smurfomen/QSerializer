#ifndef FIELD_H
#define FIELD_H
#include <QObject>
#include <vector>
#include <QVector>
#include <QVariant>
#include <QMetaProperty>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QVariantList>
#include <string.h>

class QGadget {
    Q_GADGET
public:
    QGadget(QMetaObject mo) : mo(mo){}
    QJsonObject toJson(){
        QJsonObject json;
        for(int i = 0; i < mo.propertyCount(); i++)
        {
            json.insert(mo.property(i).name(), mo.property(i).readOnGadget(this).toJsonValue());
        }
        return json;
    }

private:
    QMetaObject mo;
};

#define QS_METATYPE(Type) qRegisterMetaType<Type>(#Type) ;

#define QS_REGISTER(Type)       \
QS_METATYPE(Type)               \
QS_METATYPE(Type*)              \
QS_METATYPE(std::vector<Type*>) \
QS_METATYPE(std::vector<Type>)  \


#define __QS_FIELD             "field"
#define __QS_NAME_PERFIX       "qs_info::"
#define QS_INFO(type, name, value)                                                  \
    Q_CLASSINFO(__QS_NAME_PERFIX type #name #value,                                 \
                type "\n" #name "\n" #value)                                        \


#define QS_DECLARE(type, name)                                                      \
    public :                                                                        \
    type name;                                                                      \

#define QS_BIND_FIELD(type, name)                                                   \
    Q_PROPERTY(QJsonValue name READ get_##name WRITE set_##name)                    \
    private:                                                                        \
    QJsonValue get_##name() const {                                                 \
        QJsonValue val = QJsonValue::fromVariant(QVariant::fromValue(name));        \
        return val;                                                                 \
    }                                                                               \
    void set_##name(QJsonValue varname){                                            \
        name = varname.toVariant().value<type>();                                   \
    }                                                                               \
                                                                                    \
// работает с любым шаблонным контейнером,
// у которого есть метод append
#define QS_BIND_ARRAY(type, name)                                                   \
    Q_PROPERTY(QJsonValue name READ get_##name WRITE set_##name)                    \
    private:                                                                        \
    QJsonValue get_##name() const {                                                 \
        QJsonArray val;                                                             \
        for(int i = 0; i < name.size(); i++)                                        \
            val.append(name.at(i));                                                 \
        return QJsonValue::fromVariant(val);                                        \
    }                                                                               \
    void set_##name(QJsonValue varname){                                            \
        if(!varname.isArray())                                                      \
            return;                                                                 \
        QJsonArray val = varname.toArray();                                         \
        for(int i = 0; i < val.size(); i++)                                         \
        name.append(val.at(i).toVariant().value<type>());                           \
    }                                                                               \

#define QS_MAKE_FIELD(type, name)                                                   \
    QS_DECLARE(type, name)                                                          \
    QS_BIND_FIELD(type, name)                                                       \

// работает с любым шаблонным контейнером, у которого есть метод append
#define QS_MAKE_ARRAY(type, name)                                                   \
    QS_DECLARE(type, name)                                                          \
    QS_BIND_ARRAY(type, name)                                                       \




class Field : public QGadget {
    Q_GADGET
    // связать существующее поле
    QS_BIND_FIELD(QString, bindOnly)

    // создать и связать поле
    QS_MAKE_FIELD(QString, message)
    QS_MAKE_FIELD(int, digit)

    // создать и связать массив (Это может быть что угодно - главное, чтобы контейнер имел метод append)
    QS_MAKE_ARRAY(QList<int>, array)

public:
    Field() : QGadget(staticMetaObject){}
    QString bindOnly;

};

#endif // FIELD_H
