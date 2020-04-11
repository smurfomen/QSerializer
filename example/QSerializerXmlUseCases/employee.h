#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QObject>

class SPECIAL : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double score MEMBER score USER true)
    Q_PROPERTY(std::vector<int> numbers MEMBER numbers USER true)
public:
    SPECIAL() {

    }

    double score{0.0};
    std::vector<int> numbers;

};


class Employee : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name MEMBER name USER true)
    Q_PROPERTY(int age MEMBER age USER true)
    Q_PROPERTY(std::vector<int> nums MEMBER nums USER true)

    Q_PROPERTY(std::vector<SPECIAL *> special MEMBER special USER true)

public:
    Employee() {
        age = 20;
        name = "Bob";
        special.push_back(new SPECIAL());
        special.back()->score = 99.99;
        special.push_back(new SPECIAL());
        special.back()->score = 77.77;
    }

    QString name;
    int age{0};
    std::vector<SPECIAL *> special;
    std::vector<int> nums;


};
Q_DECLARE_METATYPE(std::vector<SPECIAL*>)

#endif // EMPLOYEE_H
