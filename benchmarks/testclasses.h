#ifndef TESTCLASSES_H
#define TESTCLASSES_H

#include "../src/qserializer.h"
#include <QObject>

class TestField_int {
    Q_GADGET
    QS_CLASS
    QS_FIELD(int, field_int)
};

class TestField_string{
    Q_GADGET
    QS_CLASS
    QS_FIELD(QString, field_string)
};

class TestCollection_vector_int {
    Q_GADGET
    QS_CLASS
    QS_COLLECTION(QVector, int, vector_int)
};

class TestCollection_vector_string {
    Q_GADGET
    QS_CLASS
    QS_COLLECTION(QVector, QString, vector_string)
};



class Object
{
    Q_GADGET
    QS_CLASS
    QS_FIELD(int, f_int)
    QS_FIELD(QString, f_string)
    QS_COLLECTION(QVector, int, v_int)
    QS_COLLECTION(QVector, QString, v_string)
};


class TestObject_field {
    Q_GADGET
    QS_CLASS
    QS_OBJECT(Object, f_object)
};


class TestObject_collection{
    Q_GADGET
    QS_CLASS
    QS_COLLECTION_OBJECTS(QVector, Object, vector_object)
};


#endif // TESTCLASSES_H
