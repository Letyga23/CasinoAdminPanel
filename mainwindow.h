#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "existingtableswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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
