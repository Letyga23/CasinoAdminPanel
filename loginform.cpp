#include "loginform.h"
#include "ui_loginform.h"
#include "adminpanelwindow.h"
#include "role.h"
#include <QDebug>
#include <QMessageBox>

LoginForm::LoginForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
    assigningValues();
    connects();
    creatingObjects();
}

LoginForm::~LoginForm()
{
    delete ui;
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
    connect(ui->logIn, &QPushButton::clicked, this, &LoginForm::logInSystem);
    connect(ui->visiblePassword, &QPushButton::clicked, this, &LoginForm::visibilityPassword);
}

void LoginForm::creatingObjects()
{
    QString connectionName = "Connection_" + QString::number(QRandomGenerator::global()->generate());

    _db = QSharedPointer<QSqlDatabase>::create(QSqlDatabase::addDatabase("QSQLITE", connectionName));
    _db->setDatabaseName("Database/casino.sqlite");

    _query = QSharedPointer<QSqlQueryModel>::create();
}

void LoginForm::logInSystem()
{
    QString request("SELECT role FROM role WHERE login = " + ui->inputFieldLogin->text() + " and password = " + ui->inputFieldPassword->text() + "");

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
    if (ui->inputFieldPassword->echoMode() == QLineEdit::Password)
    {
        ui->visiblePassword->setToolTip("Скрыть пароль");
        ui->visiblePassword->setIcon(QIcon(":/assets/passwordOpen.png"));
        ui->inputFieldPassword->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        ui->visiblePassword->setToolTip("Показать пароль");
        ui->visiblePassword->setIcon(QIcon(":/assets/passwordClose.png"));
        ui->inputFieldPassword->setEchoMode(QLineEdit::Password);
    }
}
