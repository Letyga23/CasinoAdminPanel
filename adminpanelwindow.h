﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "existingtables.h"
#include "loanapplications.h"
#include "creategametable.h"

class AdminPanelWindow : public QMainWindow
{
    Q_OBJECT

    QString _role;

private:
    QWidget* _mainWidget;

    CreateGameTableTab* _tab1_Widget;
    ExistingTablesWindow* _tab2_Widget;
    LoanApplicationsWindow* _tab3_Widget;

    QVBoxLayout* _verticalLayout_Main;

    QPushButton* _openExistingTablesWin;
    QPushButton* _openLoanApplicationsWin;

    QStatusBar* _statusBar;

    QTabWidget* _tabWidget;

public:
    AdminPanelWindow(QString role = "", QWidget *parent = nullptr);
    ~AdminPanelWindow();

private:
    void renderingInterface();
    void rendering_Tab_1();
    void rendering_Tab_2();
    void rendering_Tab_3();
    void connects();

private slots:

};
#endif // MAINWINDOW_H