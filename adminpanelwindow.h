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

    QString _role;

private:
    QWidget* _mainWidget;

    CreateGameTableTab* _createGameTableTab;
    ExistingTablesWindow* _existingTablesTab;
    LoanApplicationsWindow* _loanApplicationsTab;

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
    void renderingInterface();
    void rendering_CreateGameTableTab();
    void rendering_ExistingTablesTab();
    void rendering_LoanApplicationsTab();
    void rendering_ToolBar();
    void connects();
    void creatingObjects();

private slots:
    void openCreateTable();
    void openExistingTable();
    void openLoanAplications();

};
#endif // MAINWINDOW_H
