#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class LoginForm : public QMainWindow
{
    QFont _font;
    QFont _fontBold;

private:
    QWidget* _widget;

    QGridLayout* _gridLayout;

    QLabel* _loginLabel;
    QLabel* _passworLabel;

    QLineEdit* _inputFieldLogin;
    QLineEdit* _inputFieldPassword;

    QHBoxLayout* _horizontalLayout;

    QSpacerItem* _horizontalSpacer;
    QSpacerItem* _horizontalSpacer_2;

    QPushButton* _logIn;
    QPushButton* _visiblePassword;

public:
    LoginForm(QWidget *parent = nullptr);
    ~LoginForm();

private:
    void assigningValues();
    void connects();
    void renderingInterface();
    void renderingLayut_1();
    void renderingLayut_2();
    void renderingLayut_3();

private slots:
    void logInSystem();
    void visibilityPassword();
};

#endif // LOGINFORM_H
