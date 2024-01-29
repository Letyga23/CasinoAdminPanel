#include "welcome.h"
#include <QTime>

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
    setTextToLabel();
    //    _label->setText("Добро пожаловать, " + _user);
    _label->setFont(font);
    _label->setAlignment(Qt::AlignCenter);
    _verticalLayout->addWidget(_label);
}

void Welcome::setTextToLabel()
{
    QTime currentTime = QTime::currentTime();
    QString goodTime;

    int hour = currentTime.hour();

    if (hour >= 6 && hour < 12)
        goodTime = "Доброе утро, ";
    else if (hour >= 12 && hour < 17)
        goodTime = "Добрый день, ";
    else if (hour >= 17 && hour < 22)
        goodTime = "Добрый вечер, ";
    else
        goodTime = "Доброй ночи, ";

    _label->setText(goodTime + _user);
}
