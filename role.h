#ifndef ROLE_H
#define ROLE_H

#include <QMap>

enum class RoleEnum
{
    Administrator,
    Dealer
};

class Role
{
    static QMap<QString, RoleEnum> _roleMap;
    static QString _roleString;
    static RoleEnum _roleEnum;

public:
    Role(QString roleString);

    static QString getRoleString();
    static RoleEnum getRoleEnum();

private:
    static void assigningValues();

};

#endif // ROLE_H
