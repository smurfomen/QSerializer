#ifndef FIELD_H
#define FIELD_H
#include <QObject>
#include <vector>
#include "../../src/qserializer.h"


class Obj2 : public QGadget{
    Q_GADGET
    QS_MAKE_ARRAY(QList<QString>, names)
    QS_MAKE_FIELD(int, count)
public:
    Obj2() : QGadget(staticMetaObject){}
};

class Obj : public QGadget {
    Q_GADGET
    QS_MAKE_FIELD(int, i)
    QS_MAKE_FIELD(QString, s)
    QS_MAKE_OBJECT(Obj2, names)
public:
    Obj() : QGadget(staticMetaObject) {}
};


class Field : public QGadget {
    Q_GADGET
    // связать существующее поле
    QS_BIND_FIELD(QString, bindOnly)

    // создать и связать поле
    QS_MAKE_FIELD(QString, message)
    QS_MAKE_FIELD(int, digit)

    QS_MAKE_ARRAY(QVector<int>, array)
    // создать и связать массив (Это может быть что угодно - главное, чтобы контейнер имел метод append)
    QS_MAKE_OBJECT(Obj, ob)

    QS_MAKE_ARRAY_OBJECTS(QVector<Obj>, vob)

public:
    Field() : QGadget(staticMetaObject){}
    QString bindOnly;

};

#endif // FIELD_H
