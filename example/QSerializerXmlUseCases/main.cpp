#include <QCoreApplication>
#include <QDebug>
#include <qserializer.h>
#include <employee.h>


#include <keepersfactory.h>
#include <QDomNode>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Employee emp;

    emp.special.at(0)->numbers.push_back(2);
    emp.special.at(0)->numbers.push_back(3);
    emp.special.at(0)->numbers.push_back(4);
    emp.special.at(0)->numbers.push_back(5);

    emp.nums.push_back(2);
    emp.nums.push_back(3);
    emp.nums.push_back(4);
    emp.nums.push_back(5);


    QDomDocument doc = QSerializer::toXml(&emp);
    qDebug()<<doc.toString().toStdString().c_str();

    emp.age = 100;
    emp.special.at(0)->numbers.clear();
    emp.special.at(1)->numbers.push_back(999);
    emp.nums.push_back(555);

    Employee cloneEmp;
    doc = QSerializer::toXml(&emp);
    QSerializer::fromXml(&cloneEmp, doc);

    QDomDocument cloneDoc = QSerializer::toXml(&cloneEmp);
    qDebug()<<cloneDoc.toString().toStdString().c_str();

    return a.exec();
}


