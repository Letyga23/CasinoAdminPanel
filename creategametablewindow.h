#ifndef CREATEGAMETABLEWINDOW_H
#define CREATEGAMETABLEWINDOW_H

#include <QWidget>

namespace Ui {
class createGameTableWindow;
}

class createGameTableWindow : public QWidget
{
    Q_OBJECT

public:
    explicit createGameTableWindow(QWidget *parent = nullptr);
    ~createGameTableWindow();

private:
    Ui::createGameTableWindow *ui;
};

#endif // CREATEGAMETABLEWINDOW_H
