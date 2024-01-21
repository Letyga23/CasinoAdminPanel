#include "creategametablewindow.h"
#include "ui_creategametablewindow.h"

createGameTableWindow::createGameTableWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::createGameTableWindow)
{
    ui->setupUi(this);
}

createGameTableWindow::~createGameTableWindow()
{
    delete ui;
}
