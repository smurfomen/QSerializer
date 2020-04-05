#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QObject>
#include <vector>
class Employee : public QObject
{
    Q_OBJECT

    // set Json propertyes (mark ' USER true ' necessarily !!!)
    Q_PROPERTY(QString name MEMBER name USER true)
    Q_PROPERTY(QString email MEMBER email USER true)
    Q_PROPERTY(int age MEMBER age USER true)
    Q_PROPERTY(bool vacation MEMBER vacation USER true)
    Q_PROPERTY(double score MEMBER score USER true)
    Q_PROPERTY(std::vector<QString> phone MEMBER phone USER true)
    Q_PROPERTY(std::vector<int> shedule MEMBER shedule USER true)

public:
    Employee()
    {
        // set fields value default for example
        name = "Bob";
        email = "example@exmail.com";
        age = 18;
        score = 82.23;
        phone.push_back("+12345678989");
        phone.push_back("+98765432121");
        for(int i = 0; i < 30; i++)
            shedule.push_back(i);
        vacation = true;
    }

    QString name;
    QString email;
    int age{0};
    bool vacation{false};
    double score{0.0};
    std::vector<QString> phone;
    std::vector<int> shedule;
};

#endif // EMPLOYEE_H
