#include "welcome.h"

Welcome::Welcome(QString user, QWidget *parent) : QWidget(parent), _user(user)
{
    renderingInterface();
}

Welcome::~Welcome()
{
    delete _verticalLayout;
}

void Welcome::renderingInterface()
{
    _verticalLayout = new QVBoxLayout(this);

    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(30);
    font.setBold(true);

    _label = new QLabel();
    _label->setText("Добро пожаловать, " + _user);
    _label->setFont(font);
    _label->setAlignment(Qt::AlignCenter);
    _verticalLayout->addWidget(_label);
}
