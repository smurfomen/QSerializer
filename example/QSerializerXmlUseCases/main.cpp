#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>

#include <qserializer.h>

#include "employee.h"
#include <QDebug>
const QString EMPLOYEE_FILE = "employeeOutput.xml";

void writeEmployeeToJsonFile(Employee * e);
Employee * readEmployeeFromJsonFile();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Employee employee;

    writeEmployeeToJsonFile(&employee);

    Employee * employee2 = readEmployeeFromJsonFile();

    qDebug()<<"EMPLOYEE 2"<<QSerializer::toXml(employee2).toString().toStdString().c_str();
    return 0;
}

void writeEmployeeToJsonFile(Employee * e)
{
    QDomDocument xmlUser =  QSerializer::toXml(e);
    qDebug()<<"EMPLOYEE 1"<<xmlUser.toString().toStdString().c_str();

    QFile file(EMPLOYEE_FILE);
    if(file.exists())
        file.remove();
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(xmlUser.toString().toStdString().c_str());
        file.close();
    }
}

Employee * readEmployeeFromJsonFile()
{
    QFile file (EMPLOYEE_FILE);
    if(file.open(QIODevice::ReadOnly))
    {
        QJsonObject jsonObj = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
        return QSerializer::fromJson<Employee>(jsonObj);
    }
    throw std::exception();
}

