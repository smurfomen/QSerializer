#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <QObject>
#include <vector>

class Skill : public QObject
{
    Q_OBJECT

    // set Json propertyes (mark ' USER true ' necessarily !!!)
    Q_PROPERTY(double score MEMBER score USER true)
    Q_PROPERTY(QString description MEMBER description USER true)
public:
    Skill(QString describtion, double score)
    {
        this->score = score;
        this->description = describtion;
    }

private:
    double score;
    QString description;
};

class SPECIAL : public QObject
{
    Q_OBJECT

    // set Json propertyes (mark ' USER true ' necessarily !!!)
    Q_PROPERTY(std::vector<Skill*> skills MEMBER skills USER true)
    Q_PROPERTY(double total_score MEMBER total_score USER true)

public:

    SPECIAL() { }

    std::vector<Skill*> skills;
    double total_score{0.0};
};
Q_DECLARE_METATYPE(std::vector<Skill*>)


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
    Q_PROPERTY(SPECIAL* special MEMBER special USER true)

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
        for(int i = 0; i < 5; i++)
            shedule.push_back(i);
        vacation = true;
        special = new SPECIAL();

        special->skills.push_back(new Skill("STRONG", 8.88));
        special->total_score += 8.88;
        special->skills.push_back(new Skill("AGILITY", 5.67));
        special->total_score += 5.67;
        special->skills.push_back(new Skill("LUCK", 3.78));
        special->total_score += 3.78;
        special->skills.push_back(new Skill("INTELLIGENCE", 7.09));
        special->total_score += 7.09;
    }

private:
    QString name;
    QString email;
    int age{0};
    bool vacation{false};
    double score{0.0};
    std::vector<QString> phone;
    std::vector<int> shedule;
    SPECIAL* special;
};


#endif // EMPLOYEE_H
