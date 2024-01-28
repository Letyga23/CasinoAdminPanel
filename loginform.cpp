#include "loginform.h"
#include "adminpanelwindow.h"
#include "role.h"
#include <QDebug>
#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent)
    : QMainWindow(parent)
{
    assigningValues();
    renderingInterface();
    connects();
    creatingObjects();
}

LoginForm::~LoginForm()
{
    delete _widget;
}

void LoginForm::assigningValues()
{
    _font.setFamily("Segoe UI");
    _font.setPointSize(18);

    _fontBold.setFamily("Segoe UI");
    _fontBold.setPointSize(18);
    _fontBold.setBold(true);
}

void LoginForm::connects()
{
    connect(_logIn, &QPushButton::clicked, this, &LoginForm::logInSystem);
    connect(_visiblePassword, &QPushButton::clicked, this, &LoginForm::visibilityPassword);
}

void LoginForm::creatingObjects()
{
    QString connectionName = "Connection_" + QString::number(QRandomGenerator::global()->generate());

    _db = QSharedPointer<QSqlDatabase>::create(QSqlDatabase::addDatabase("QSQLITE", connectionName));
    _db->setDatabaseName("Database/casino.sqlite");

    _query = QSharedPointer<QSqlQueryModel>::create();
}

void LoginForm::renderingInterface()
{
    setWindowTitle("Вход");
    setFixedSize(370, 150);
    //    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);

    _widget = new QWidget();

    _gridLayout = new QGridLayout(_widget);

    renderingLayut_1();
    renderingLayut_2();
    renderingLayut_3();

    setCentralWidget(_widget);
}

void LoginForm::renderingLayut_1()
{

    _loginLabel = new QLabel(_widget);
    _loginLabel->setText("Логин: ");
    _loginLabel->setFont(_fontBold);
    _gridLayout->addWidget(_loginLabel, 0, 0);

    _inputFieldLogin = new QLineEdit(_widget);
    _inputFieldLogin->setFont(_font);
    _inputFieldLogin->setMaximumWidth(200);
    _gridLayout->addWidget(_inputFieldLogin, 0, 1);
}

void LoginForm::renderingLayut_2()
{
    _passworLabel = new QLabel(_widget);
    _passworLabel->setText("Пароль: ");
    _passworLabel->setFont(_fontBold);
    _gridLayout->addWidget(_passworLabel, 1, 0);

    _inputFieldPassword = new QLineEdit(_widget);
    _inputFieldPassword->setFont(_font);
    _inputFieldPassword->setMaximumWidth(200);
    _inputFieldPassword->setEchoMode(QLineEdit::Password);
    _gridLayout->addWidget(_inputFieldPassword, 1, 1);

    _visiblePassword = new QPushButton(_widget);
    _visiblePassword->setToolTip("Показать пароль");
    _visiblePassword->setIcon(QIcon(":/assets/пароль спрятан.png"));
    _visiblePassword->setIconSize(QSize(32, 32));
    _gridLayout->addWidget(_visiblePassword, 1, 2);
}

void LoginForm::renderingLayut_3()
{
    _horizontalLayout = new QHBoxLayout();

    _horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout->addItem(_horizontalSpacer);

    _logIn = new QPushButton(_widget);
    _logIn->setText("Войти");
    _logIn->setFont(_fontBold);
    _horizontalLayout->addWidget(_logIn);

    _horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout->addItem(_horizontalSpacer_2);

    _gridLayout->addLayout(_horizontalLayout, 3, 0, 1, 3);
}


void LoginForm::logInSystem()
{
    QString request("SELECT role FROM role WHERE login = " + _inputFieldLogin->text() + " and password = " + _inputFieldPassword->text() + "");

    _db->open();
    _query->setQuery(request, *_db);
    _db->close();

    QString role = _query->data(_query->index(0, 0)).toString();

    if(!role.isEmpty())
    {
        Role rol(role);

        AdminPanelWindow* adminPanel = new AdminPanelWindow();
        adminPanel->show();

        close();
    }
    else
        QMessageBox::warning(this, "Внимание", "Логин или пароль введены не верно!", QMessageBox::Ok);
}

void LoginForm::visibilityPassword()
{
    if (_inputFieldPassword->echoMode() == QLineEdit::Password)
    {
        _visiblePassword->setToolTip("Скрыть пароль");
        _visiblePassword->setIcon(QIcon(":/assets/пароль открыт.png"));
        _inputFieldPassword->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        _visiblePassword->setToolTip("Показать пароль");
        _visiblePassword->setIcon(QIcon(":/assets/пароль спрятан.png"));
        _inputFieldPassword->setEchoMode(QLineEdit::Password);
    }
}
