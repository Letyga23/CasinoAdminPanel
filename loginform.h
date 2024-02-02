#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QMainWindow>
#include <QSharedPointer>
#include <QRandomGenerator>
#include <QSqlDatabase>
#include <QSqlQueryModel>

namespace Ui {
class LoginForm;
}

class LoginForm : public QMainWindow
{
    Q_OBJECT

    Ui::LoginForm *ui;
    QFont _font;
    QFont _fontBold;
    QSharedPointer<QSqlDatabase> _db;
    QSharedPointer<QSqlQueryModel> _query;

public:
    explicit LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

private:
    void creatingObjects();
    void assigningValues();
    void connects();

private slots:
    void logInSystem();
    void visibilityPassword();
};

#endif // LOGINFORM_H
