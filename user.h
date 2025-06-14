#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:
    QString name;
    QString id;
public:
    User(QString name_ = "", QString id_ = "");
    ~User() = default;

    QString getName();
    QString getID();
};

#endif // USER_H
