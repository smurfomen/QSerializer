#ifndef CLASSES_H
#define CLASSES_H
#include "../../src/qserializer.h"
#include <QQueue>
#include <QStack>

QS_BEGIN_CLASS(Parent)
    QS_FIELD(int, age)
    QS_FIELD(QString, name)
    QS_FIELD(bool, male)
QS_END_CLASS

QS_BEGIN_CLASS(Student)
    QS_FIELD(int, age)
    QS_FIELD(QString, name)
    QS_COLLECTION(QList, QString, links)
    QS_COLLECTION_OBJECTS(QList, Parent, parents)
QS_END_CLASS

class Field : public QSerializer {
    QS_CLASS
    QS_FIELD(int, digit)
    QS_FIELD(QString, string)
    QS_FIELD(bool, flag)
    QS_FIELD(double, d_digit)

    public:
        Field() QS_PROVIDE { }
};

class Collection : public QSerializer {
    QS_CLASS
    QS_COLLECTION(QVector, int, vector)
    QS_COLLECTION(QList, QString, list)
    QS_COLLECTION(QStack, double, stack)
    public:
        Collection() QS_PROVIDE {}
};


class CustomObject : public QSerializer {
    QS_CLASS
    QS_FIELD(int, digit)
    QS_COLLECTION(QVector, QString, string)

    public:
        CustomObject() QS_PROVIDE {}
};


class CollectionOfObjects : public QSerializer {
    QS_CLASS
    QS_COLLECTION_OBJECTS(QVector, CustomObject, objects)

    public:
        CollectionOfObjects() QS_PROVIDE {}
};


class General : public QSerializer {
    QS_CLASS
    QS_OBJECT(Field, field)
    QS_OBJECT(Collection, collection)
    QS_OBJECT(CustomObject, object)
    QS_OBJECT(CollectionOfObjects, collectionObjects)
public:
    General() QS_PROVIDE {}
};


#endif // CLASSES_H
