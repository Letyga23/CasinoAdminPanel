#ifndef ADMINPANELWINDOW_H
#define ADMINPANELWINDOW_H

#include <QMainWindow>
#include <QAction>
#include "existingtables.h"
#include "loanapplications.h"
#include "creategametable.h"
#include "role.h"
#include "welcome.h"

namespace Ui {
class AdminPanelWindow;
}

class AdminPanelWindow : public QMainWindow
{
    Q_OBJECT

    Ui::AdminPanelWindow *ui;
    QAction* _lastAction;
    QFont _standardFont;

    Welcome* _welcomTab;

    QAction* _openCreateTable;
    QAction* _openExistingTable;
    QAction* _openLoanAplications;

public:
    explicit AdminPanelWindow(QWidget *parent = nullptr);
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

#endif // ADMINPANELWINDOW_H
