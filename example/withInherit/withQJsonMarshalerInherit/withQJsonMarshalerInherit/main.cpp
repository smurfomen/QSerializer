#include <QCoreApplication>
#include "employee.h"
#include <QFile>

const QString USER_JSON_FILE_NAME = "user_data.json";

void writeEmployeeToJsonFile(Employee &e);
void readEmployeeFromJsonFile(Employee &e);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Employee employee;

    writeEmployeeToJsonFile(employee);

    Employee employee2;
    readEmployeeFromJsonFile(employee);

    return a.exec();
}

void writeEmployeeToJsonFile(Employee &e)
{
    QJsonObject jsonUser = e.Marshal();
    QJsonDocument document(jsonUser);
    QFile file(USER_JSON_FILE_NAME);
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
    QFile file (USER_JSON_FILE_NAME);
    if(file.open(QIODevice::ReadOnly))
    {
        QJsonObject jsonObj = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
        e.Unmarshal(jsonObj);
    }
}
