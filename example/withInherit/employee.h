#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <vector>
#include <qjsonmarshaler.h>
class Employee : public QJsonMarshaler
{
    // set Json propertyes and annotations
    void initPropertyes()
    {
        setJsonProperty(name, "name");
        setJsonProperty(email, "email");
        setJsonProperty(age, "age");
        setJsonProperty(onVacation, "vacation");
        setJsonProperty(score, "score");
        setJsonProperty(phoneNumbers, "phone");
        setJsonProperty(workShedule, "shedule");
    }

public:
    Employee()
    {
        initPropertyes();

        // set fields value default for example
        name = "Bob";
        email = "example@exmail.com";
        age = 20;
        score = 82.23;
        phoneNumbers.push_back("+12345678989");
        phoneNumbers.push_back("+98765432121");
        for(int i = 0; i < 5; i++)
            workShedule.push_back(i);
        onVacation = true;
    }

    QString name;
    QString email;
    int age{0};
    bool onVacation{false};
    double score{0.0};
    std::vector<QString> phoneNumbers;
    std::vector<int> workShedule;
};

#endif // EMPLOYEE_H
