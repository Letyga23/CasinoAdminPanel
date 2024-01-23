#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "existingtableswindow.h"
#include "loanapplicationswindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QWidget* centralwidget;

    QVBoxLayout* verticalLayout;

    QPushButton* _openCreateGameTableWin;
    QPushButton* _openExistingTablesWin;
    QPushButton* _openLoanApplicationsWin;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void renderingInterface();
    void connects();

private slots:
    void openCreateGameTableWin();
    void openExistingTablesWin();
    void openLoanApplicationsWin();

};
#endif // MAINWINDOW_H
