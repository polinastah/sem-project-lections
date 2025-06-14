#include "user.h"

User::User(QString name_, QString id_) :
    name(name_),
    id(id_)
{}

QString User::getName() {
    return name;
}

QString User::getID() {
    return id;
}
