#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>

#include <qserializer.h>

#include "employee.h"
#include <QDebug>
const QString EMPLOYEE_FILE = "employeeOutput.xml";

void writeEmployeeToXmlFile(const QDomDocument &doc);
QDomDocument readEmployeeFromJsonFile();

int main(int argc, char *argv[])
{
    QS_REGISTER(Skill)
    QCoreApplication a(argc, argv);
    Employee employee;
    employee.age = 100;
    employee.name = "Mike";

    QDomDocument xml = QSerializer::toXml(&employee);
    writeEmployeeToXmlFile(xml);
    qDebug()<<"EMPLOYEE 1"<<xml.toString().toStdString().c_str();

    xml = readEmployeeFromJsonFile();
    Employee * employee2 = QSerializer::fromXml<Employee>(xml);
    xml = QSerializer::toXml(employee2);
    qDebug()<<"EMPLOYEE 2"<<xml.toString().toStdString().c_str();

    return 0;
}

void writeEmployeeToXmlFile(const QDomDocument &doc)
{
    QFile file(EMPLOYEE_FILE);
    if(file.exists())
        file.remove();
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toString().toStdString().c_str());
        file.close();
    }
}

QDomDocument readEmployeeFromJsonFile()
{
    QFile file (EMPLOYEE_FILE);
    QDomDocument xml;
    if(file.open(QIODevice::ReadOnly))
    {
        xml.setContent(&file);
        file.close();
    }
    return xml;
}

