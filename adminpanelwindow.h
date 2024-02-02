#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include "existingtables.h"
#include "loanapplications.h"
#include "creategametable.h"
#include "role.h"
#include "welcome.h"

class AdminPanelWindow : public QMainWindow
{
    Q_OBJECT
    QAction* _lastAction;
    QFont _standardFont;

private:
    QWidget* _mainWidget;

    Welcome* _welcomTab;

    QVBoxLayout* _verticalLayout_Main;

    QStatusBar* _statusBar;

    QTabWidget* _tabWidget;

    QMenuBar* _menuBar;
    QToolBar* _toolBar;

    QAction* _openCreateTable;
    QAction* _openExistingTable;
    QAction* _openLoanAplications;

public:
    AdminPanelWindow(QWidget *parent = nullptr);
    ~AdminPanelWindow();

    static CreateGameTable* _createGameTableTab;
    static ExistingTables* _existingTablesTab;
    static LoanApplications* _loanApplicationsTab;

private:
    void assigningValues();
    void selectAction();
    void renderingInterface();

    void rendering_WelcomeTab();
    void rendering_CreateGameTableTab();
    void rendering_ExistingTablesTab();
    void rendering_LoanApplicationsTab();

    void connects_Admin();
    void connects_Diller();

    void creatingObjects_Admin();
    void creatingObjects_Diller();

    void rendering_Admin();
    void rendering_Diller();

    void rendering_MenuBar_Admin();
    void rendering_MenuBar_Diller();

private slots:
    void openCreateTable();
    void openExistingTable();
    void openLoanAplications();

};
#endif // MAINWINDOW_H
