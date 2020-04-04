#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <qjsonmarshalerlib_global.h>
#include <qjsonmarshaler.h>
#include "user.h"
const QString USER_JSON_FILE_NAME = "user_data.json";

void writeUser(User * u);
User * readUser();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    User user;
    user.name = "Bob";
    user.email = "example@exmail.com";
    user.age = 18;
    user.score = 82.23;
    user.phoneNumbers.push_back("+12345678989");
    user.phoneNumbers.push_back("+98765432121");

    writeUser(&user);

    User * newUser = readUser();

    return a.exec();
}

void writeUser(User * u)
{
    QJsonObject jsonUser =  QJsonMarshaler::Marshal(u);
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

User * readUser()
{
    QFile file (USER_JSON_FILE_NAME);
    if(file.open(QIODevice::ReadOnly))
    {
        QJsonObject jsonObj = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
        return QJsonMarshaler::Unmarshal<User>(jsonObj);
    }
    throw -1;
}

