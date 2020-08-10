#ifndef CLASSES_H
#define CLASSES_H
#include "../../src/qserializer.h"

class Field : public QSerializer {
    Q_GADGET
    QS_FIELD(int, digit)
    QS_FIELD(QString, string)
    QS_FIELD(bool, flag)

    public:
        Field() : QSerializer(staticMetaObject) {}
};

class Collection : public QSerializer {
    Q_GADGET
    QS_COLLECTION(QVector, int, vector)
    QS_COLLECTION(QList, QString, list)

    public:
        Collection() : QSerializer(staticMetaObject) {}
};


class CustomObject : public QSerializer {
    Q_GADGET
    QS_FIELD(int, digit)
    QS_COLLECTION(QVector, QString, string)

    public:
        CustomObject() : QSerializer(staticMetaObject) {}
};


class CollectionOfObjects : public QSerializer {
    Q_GADGET
    QS_COLLECTION_OBJECTS(QVector, CustomObject, objects)

    public:
        CollectionOfObjects() : QSerializer(staticMetaObject) {}
};


class General : public QSerializer {
    Q_GADGET
    QS_OBJECT(Field, field)
    QS_OBJECT(Collection, collection)
    QS_OBJECT(CustomObject, object)
    QS_OBJECT(CollectionOfObjects, collectionObjects)
public:
    General() : QSerializer(staticMetaObject){}
};


#endif // CLASSES_H
