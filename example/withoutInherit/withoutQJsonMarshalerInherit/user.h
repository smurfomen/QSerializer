#ifndef USER_H
#define USER_H
#include <QObject>
#include <vector>
class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER name USER true)
    Q_PROPERTY(QString email MEMBER email USER true)
    Q_PROPERTY(int age MEMBER age USER true)
    Q_PROPERTY(double score MEMBER score USER true)
    Q_PROPERTY(std::vector<QString> phoneNumbers MEMBER phoneNumbers USER true)
public:
    User(){}

    QString name;
    QString email;
    int age;
    double score;
    std::vector<QString> phoneNumbers;
};

#endif // USERDATA_H
