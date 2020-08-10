#ifndef CLASSES_H
#define CLASSES_H
#include "../../src/qserializer.h"

class Field : public QGadget {
    Q_GADGET
    QS_FIELD(int, digit)
    QS_FIELD(QString, string)
    QS_FIELD(bool, flag)

    public:
        Field() : QGadget(staticMetaObject) {}
};

class Collection : public QGadget {
    Q_GADGET
    QS_COLLECTION(QVector, int, vector)
    QS_COLLECTION(QList, QString, list)

    public:
        Collection() : QGadget(staticMetaObject) {}
};


class CustomObject : public QGadget {
    Q_GADGET
    QS_FIELD(int, digit)
    QS_COLLECTION(QVector, QString, string)

    public:
        CustomObject() : QGadget(staticMetaObject) {}
};


class CollectionOfObjects : public QGadget {
    Q_GADGET
    QS_COLLECTION_OBJECTS(QVector, CustomObject, objects)

    public:
        CollectionOfObjects() : QGadget(staticMetaObject) {}
};


class General : public QGadget {
    Q_GADGET
    QS_OBJECT(Field, field)
    QS_OBJECT(Collection, collection)
    QS_OBJECT(CustomObject, object)
    QS_OBJECT(CollectionOfObjects, collectionObjects)
public:
    General() : QGadget(staticMetaObject){}
};


#endif // CLASSES_H
