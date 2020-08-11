#ifndef TESTCLASSES_H
#define TESTCLASSES_H

#include "../src/qserializer.h"
#include <QObject>

QS_BEGIN_CLASS(TestField_int)
QS_FIELD(int, field_int)
QS_END_CLASS

QS_BEGIN_CLASS(TestField_string)
QS_FIELD(QString, field_string)
QS_END_CLASS




QS_BEGIN_CLASS(TestCollection_vector_int)
QS_COLLECTION(QVector, int, vector_int)
QS_END_CLASS

QS_BEGIN_CLASS(TestCollection_vector_string)
QS_COLLECTION(QVector, QString, vector_string)
QS_END_CLASS


class Object : public QSerializer
{
  QS_CLASS
  QS_FIELD(int, f_int)
  QS_FIELD(QString, f_string)
  QS_COLLECTION(QVector, int, v_int)
  QS_COLLECTION(QVector, QString, v_string)
  public:
    Object() QS_PROVIDE { }
};


QS_BEGIN_CLASS(TestObject_field)
QS_OBJECT(Object, f_object)
QS_END_CLASS


QS_BEGIN_CLASS(TestObject_collection)
QS_COLLECTION_OBJECTS(QVector, Object, vector_object)
QS_END_CLASS



#endif // TESTCLASSES_H
