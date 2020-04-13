#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>

#include <qserializer.h>

#include "employee.h"
#include <QDebug>
const QString EMPLOYEE_FILE = "employeeOutput.json";

void writeEmployeeToJsonFile(QJsonObject);
QJsonObject readEmployeeFromJsonFile();

int main(int argc, char *argv[])
{
    QS_REGISTER(Skill);

    QCoreApplication a(argc, argv);
    Employee employee;
    employee.age = 200;
    employee.name = "Mike";

    // get the first employee json and write it to file
    QJsonObject json = QSerializer::toJson(&employee);
    qDebug()<<"EMPLOYEE 1"<<QString(QJsonDocument(json).toJson()).toStdString().c_str();
    writeEmployeeToJsonFile(json);

    // read the file and create new object Employee class from first employee json
    json = readEmployeeFromJsonFile();
    Employee * employee2 = QSerializer::fromJson<Employee>(json);
    json = QSerializer::toJson(employee2);
    qDebug()<<"EMPLOYEE 2"<<QString(QJsonDocument(json).toJson()).toStdString().c_str();

    return 0;
}

void writeEmployeeToJsonFile(QJsonObject jsonUser)
{
    QJsonDocument document(jsonUser);
    QFile file(EMPLOYEE_FILE);
    if(file.exists())
        file.remove();
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(QString(document.toJson()).toStdString().c_str());
        file.close();
    }
}

QJsonObject readEmployeeFromJsonFile()
{
    QJsonObject json;
    QFile file (EMPLOYEE_FILE);
    if(file.open(QIODevice::ReadOnly))
    {
        json = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }
    return json;
}

