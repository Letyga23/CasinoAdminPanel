#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include "existingtables.h"
#include "loanapplications.h"
#include "creategametable.h"
#include "role.h"

class AdminPanelWindow : public QMainWindow
{
    Q_OBJECT
    QAction* _lastAction;
    QFont _currentFont;
    QFont _standartFont;

private:
    QWidget* _mainWidget;

    CreateGameTableTab* _createGameTableTab;
    ExistingTables* _existingTablesTab;
    LoanApplications* _loanApplicationsTab;

    QVBoxLayout* _verticalLayout_Main;

    QStatusBar* _statusBar;

    QTabWidget* _tabWidget;

    QToolBar* _toolBar;

    QAction* _openCreateTable;
    QAction* _openExistingTable;
    QAction* _openLoanAplications;

public:
    AdminPanelWindow(QWidget *parent = nullptr);
    ~AdminPanelWindow();

private:
    void assigningValues();
    void selectAction();
    void renderingInterface();

    void rendering_CreateGameTableTab();
    void rendering_ExistingTablesTab();
    void rendering_LoanApplicationsTab();

    void connects_Admin();
    void connects_Diller();

    void creatingObjects_Admin();
    void creatingObjects_Diller();

    void rendering_Admin();
    void rendering_Diller();

    void rendering_ToolBar_Admin();
    void rendering_ToolBar_Diller();

private slots:
    void openCreateTable();
    void openExistingTable();
    void openLoanAplications();

};
#endif // MAINWINDOW_H
