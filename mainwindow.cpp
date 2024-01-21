#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "creategametablewindow.h"
#include "existingtableswindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    CreateGameTableWindow* w = new CreateGameTableWindow();
    w->exec();
    delete w;
}

void MainWindow::on_pushButton_3_clicked()
{
    ExistingTablesWindow* w = new ExistingTablesWindow();
    w->show();
    close();
}
