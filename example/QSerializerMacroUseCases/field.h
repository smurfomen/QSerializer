#ifndef FIELD_H
#define FIELD_H
#include <QObject>
#include <vector>
#include "../../src/qserializer.h"


class Obj2 : public QGadget{
    Q_GADGET
    QS_ARRAY(QList<QString>, QString, names)
    QS_FIELD(int, count)
public:
    Obj2() : QGadget(staticMetaObject){}
};

class Obj : public QGadget {
    Q_GADGET
    QS_FIELD(int, i)
    QS_FIELD(QString, s)
    QS_OBJECT(Obj2, names)
public:
    Obj() : QGadget(staticMetaObject) {}
};


class Field : public QGadget {
    Q_GADGET
    // создать и связать поле
    QS_FIELD(QString, message)
    QS_FIELD(int, digit)
    QS_ARRAY(QVector<int>, int, array)
    QS_OBJECT(Obj, ob)
    QS_ARRAY_OBJECTS(QVector<Obj>, Obj, vob)

public:
    Field() : QGadget(staticMetaObject){}
};

#endif // FIELD_H
