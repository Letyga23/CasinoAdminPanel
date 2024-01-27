#include "role.h"

QMap<QString, RoleEnum> Role::_roleMap;
QString Role::_roleString;
RoleEnum Role::_roleEnum;

Role::Role(QString roleString)
{
    _roleString = roleString;

    assigningValues();

    _roleEnum = _roleMap[_roleString];
}

void Role::assigningValues()
{
    _roleMap =
    {
        {"Администратор", RoleEnum::Administrator},
        {"Диллер", RoleEnum::Dealer},
    };
}

QString Role::getRoleString()
{
    return _roleString;
}

RoleEnum Role::getRoleEnum()
{
    return _roleEnum;
}
