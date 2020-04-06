#include <QCoreApplication>
#include "employee.h"
#include <QFile>
#include <QDebug>
const QString EMPLOYEE_FILE = "employeeOutput.json";

void writeEmployeeToJsonFile(Employee &e);
void readEmployeeFromJsonFile(Employee &e);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Employee employee;

    writeEmployeeToJsonFile(employee);

    Employee employee2;
    readEmployeeFromJsonFile(employee2);

    qDebug()<<QString(QJsonDocument(employee2.Marshal()).toJson()).toStdString().c_str();

    return a.exec();
}

void writeEmployeeToJsonFile(Employee &e)
{
    QJsonObject jsonUser = e.Marshal();
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

void readEmployeeFromJsonFile(Employee &e)
{
    QFile file (EMPLOYEE_FILE);
    if(file.open(QIODevice::ReadOnly))
    {
        QJsonObject jsonObj = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
        e.Unmarshal(jsonObj);
    }
}
