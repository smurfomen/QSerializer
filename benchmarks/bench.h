#ifndef BENCH_H
#define BENCH_H
#include <QObject>
#include <QTest>
#include "testclasses.h"

class Bench : public QObject {
Q_OBJECT
private Q_SLOTS:
    //========================================================================================================================================
    void bench_field_int_toJson();

    void bench_field_int_toXml();

    void bench_field_int_fromJson();

    void bench_field_int_fromXml();
    //========================================================================================================================================



    //========================================================================================================================================
    void bench_field_string_toJson();

    void bench_field_string_toXml();

    void bench_field_string_fromJson();

    void bench_field_string_fromXml();
    //========================================================================================================================================



    //========================================================================================================================================
    void bench_collection_vector_int_toJson();

    void bench_collection_vector_int_toXml();

    void bench_collection_vector_int_fromJson();

    void bench_collection_vector_int_fromXml();
    //========================================================================================================================================



    //========================================================================================================================================
    void bench_collection_vector_string_toJson();

    void bench_collection_vector_string_toXml();

    void bench_collection_vector_string_fromJson();

    void bench_collection_vector_string_fromXml();
    //========================================================================================================================================


    //========================================================================================================================================
    void bench_object_field_toJson();

    void bench_object_field_toXml();

    void bench_object_field_fromJson();

    void bench_object_field_fromXml();
    //========================================================================================================================================



    //========================================================================================================================================
    void bench_collection_objects_toJson();

    void bench_collection_objects_toXml();

    void bench_collection_objects_fromJson();
    void bench_collection_objects_fromXml();
    //========================================================================================================================================

};

#endif // BENCH_H
