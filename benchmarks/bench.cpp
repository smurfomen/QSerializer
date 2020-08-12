#include "bench.h"
#include <QDebug>

void Bench::bench_field_int_toJson() {
    TestField_int test;
    test.field_int = 999;
    QBENCHMARK{
        test.toJson();
    }
}

void Bench::bench_field_int_toXml() {
    TestField_int test;
    test.field_int = 999;
    QBENCHMARK{
        test.toXml();
    }
}

void Bench::bench_field_int_fromJson() {
    TestField_int test;
    test.field_int = 999;
    QJsonObject json = test.toJson();
    TestField_int dest;
    QBENCHMARK{
        dest.fromJson(json);
    }
}

void Bench::bench_field_int_fromXml() {
    TestField_int test;
    test.field_int = 999;
    QDomNode xml = test.toXml();
    TestField_int dest;
    QBENCHMARK{
        dest.fromXml(xml);
    }
}

void Bench::bench_field_string_toJson() {
    TestField_string test;
    test.field_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
    QBENCHMARK{
        test.toJson();
    }
}

void Bench::bench_field_string_toXml() {
    TestField_string test;
    test.field_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
    QBENCHMARK{
        test.toXml();
    }
}

void Bench::bench_field_string_fromJson() {
    TestField_string test;
    test.field_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
    QJsonObject json = test.toJson();
    TestField_string dest;
    QBENCHMARK{
        dest.fromJson(json);
    }
}

void Bench::bench_field_string_fromXml() {
    TestField_string test;
    test.field_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
    QDomNode xml = test.toXml();
    TestField_string dest;
    QBENCHMARK{
        dest.fromXml(xml);
    }
}

void Bench::bench_collection_vector_int_toJson() {
    TestCollection_vector_int test;
    for(int i = 0; i < 100; i++)
        test.vector_int.append(i);
    QBENCHMARK{
        test.toJson();
    }
}

void Bench::bench_collection_vector_int_toXml() {
    TestCollection_vector_int test;
    for(int i = 0; i < 100; i++)
        test.vector_int.append(i);
    QBENCHMARK{
        test.toXml();
    }
}

void Bench::bench_collection_vector_int_fromJson() {
    TestCollection_vector_int test;
    for(int i = 0; i < 100; i++)
        test.vector_int.append(i);
    QJsonObject json = test.toJson();
    TestCollection_vector_int dest;
    QBENCHMARK{
        dest.fromJson(json);
    }
}

void Bench::bench_collection_vector_int_fromXml() {
    TestCollection_vector_int test;
    for(int i = 0; i < 100; i++)
        test.vector_int.append(i);
    QDomNode xml = test.toXml();
    TestCollection_vector_int dest;
    QBENCHMARK{
        dest.fromXml(xml);
    }
}

void Bench::bench_collection_vector_string_toJson() {
    TestCollection_vector_string test;
    for(int i = 0; i < 100; i++)
        test.vector_string.append(QString::number(i));
    QBENCHMARK{
        test.toJson();
    }
}

void Bench::bench_collection_vector_string_toXml() {
    TestCollection_vector_string test;
    for(int i = 0; i < 100; i++)
        test.vector_string.append(QString::number(i));
    QBENCHMARK{
        test.toXml();
    }
}

void Bench::bench_collection_vector_string_fromJson() {
    TestCollection_vector_string test;
    for(int i = 0; i < 100; i++)
        test.vector_string.append(QString::number(i));
    QJsonObject json = test.toJson();
    TestCollection_vector_string dest;
    QBENCHMARK{
        dest.fromJson(json);
    }
}

void Bench::bench_collection_vector_string_fromXml() {
    TestCollection_vector_string test;
    for(int i = 0; i < 100; i++)
        test.vector_string.append(QString::number(i));
    QDomNode xml = test.toXml();
    TestCollection_vector_string dest;
    QBENCHMARK{
        dest.fromXml(xml);
    }
}

void Bench::bench_object_field_toJson() {
    TestObject_field test;

    test.f_object.f_int = 999;
    test.f_object.f_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
    for(int i = 0; i< 100; i++)
    {
        test.f_object.v_int.append(i);
        test.f_object.v_string.append(QString::number(i));
    }
    QBENCHMARK{
        test.toJson();
    }
}

void Bench::bench_object_field_toXml() {
    TestObject_field test;

    test.f_object.f_int = 999;
    test.f_object.f_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
    for(int i = 0; i< 100; i++)
    {
        test.f_object.v_int.append(i);
        test.f_object.v_string.append(QString::number(i));
    }
    QBENCHMARK{
        test.toXml();
    }
}

void Bench::bench_object_field_fromJson() {
    TestObject_field test;

    test.f_object.f_int = 999;
    test.f_object.f_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
    for(int i = 0; i< 100; i++)
    {
        test.f_object.v_int.append(i);
        test.f_object.v_string.append(QString::number(i));
    }
    QJsonObject json = test.toJson();
    TestObject_field dest;
    QBENCHMARK{
        dest.fromJson(json);
    }
}

void Bench::bench_object_field_fromXml() {
    TestObject_field test;

    test.f_object.f_int = 999;
    test.f_object.f_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
    for(int i = 0; i< 100; i++)
    {
        test.f_object.v_int.append(i);
        test.f_object.v_string.append(QString::number(i));
    }

    QDomNode xml = test.toXml();
//    qDebug()<<"\n"<<QSerializer::toByteArray(test.toXml()).constData();
    TestObject_field dest;
    QBENCHMARK{
        dest.fromXml(xml);
    }
    qDebug()<<"\n"<<QSerializer::toByteArray(dest.toXml()).constData();
}

void Bench::bench_collection_objects_toJson() {
    TestObject_collection test;
    for(int i = 0; i < 100; i++)
    {
        Object obj;
        obj.f_int = 999;
        obj.f_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
        for(int i = 0; i< 100; i++)
        {
            obj.v_int.append(i);
            obj.v_string.append(QString::number(i));
        }
        test.vector_object.append(obj);
    }
    QBENCHMARK{
        test.toJson();
    }
}

void Bench::bench_collection_objects_toXml() {
    TestObject_collection test;
    for(int i = 0; i < 100; i++)
    {
        Object obj;
        obj.f_int = 999;
        obj.f_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
        for(int i = 0; i< 100; i++)
        {
            obj.v_int.append(i);
            obj.v_string.append(QString::number(i));
        }
        test.vector_object.append(obj);
    }
    QBENCHMARK{
        test.toXml();
    }
}

void Bench::bench_collection_objects_fromJson() {
    TestObject_collection test;
    for(int i = 0; i < 100; i++)
    {
        Object obj;
        obj.f_int = 999;
        obj.f_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
        for(int i = 0; i< 100; i++)
        {
            obj.v_int.append(i);
            obj.v_string.append(QString::number(i));
        }
        test.vector_object.append(obj);
    }
    QJsonObject json = test.toJson();
    TestObject_collection dest;
    QBENCHMARK{
        dest.fromJson(json);
    }
}
void Bench::bench_collection_objects_fromXml() {
    TestObject_collection test;
    for(int i = 0; i < 100; i++)
    {
        Object obj;
        obj.f_int = 999;
        obj.f_string = "QWERTYUIOP{ASDFGHJKL:ZXCVBNM<>?";
        for(int i = 0; i< 5; i++)
        {
            obj.v_int.append(i);
            obj.v_string.append(QString::number(i));
        }
        test.vector_object.append(obj);
    }
    QDomNode xml = test.toXml();
    TestObject_collection dest;
    QBENCHMARK{
        dest.fromXml(xml);
    }
}



QTEST_MAIN(Bench);
