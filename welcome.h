#ifndef WELCOM_H
#define WELCOM_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

namespace Ui {
class Welcom;
}

class Welcome : public QWidget
{
    Q_OBJECT

    QString _user;

private:
    QVBoxLayout* _verticalLayout;

    QLabel* _label;

public:
    explicit Welcome(QString user, QWidget *parent = nullptr);
    ~Welcome();

private:
    void renderingInterface();
    void setTextToLabel();
};

#endif // WELCOM_H
