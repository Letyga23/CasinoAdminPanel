#include "adminpanelwindow.h"
#include "ui_adminpanelwindow.h"

CreateGameTable* AdminPanelWindow::_createGameTableTab;
ExistingTables* AdminPanelWindow::_existingTablesTab;
LoanApplications* AdminPanelWindow::_loanApplicationsTab;

AdminPanelWindow::AdminPanelWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AdminPanelWindow)
{
    ui->setupUi(this);

    assigningValues();
    renderingInterface();
}

AdminPanelWindow::~AdminPanelWindow()
{
    delete ui;
}


void AdminPanelWindow::renderingInterface()
{
    ui->tabWidget->tabBar()->hide();

    ui->statusbar->showMessage(Role::getRoleString());

    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->toolBar->setStyleSheet("QToolBar { spacing: 10px; }");

    switch (Role::getRoleEnum())
    {
    case RoleEnum::Administrator :
        rendering_Admin();
        break;

    case RoleEnum::Dealer :
        rendering_Diller();
        break;
    }

    ui->menubar->setFont(_standardFont);
}

void AdminPanelWindow::rendering_WelcomeTab()
{
    _welcomTab = new Welcome(Role::getRoleString());
    ui->tabWidget->addTab(_welcomTab, "");
}

void AdminPanelWindow::rendering_CreateGameTableTab()
{
    _createGameTableTab = new CreateGameTable();
    ui->tabWidget->addTab(_createGameTableTab, "");
}

void AdminPanelWindow::rendering_ExistingTablesTab()
{
    _existingTablesTab = new ExistingTables(ui->toolBar);
    ui->tabWidget->addTab(_existingTablesTab, "");
}

void AdminPanelWindow::rendering_LoanApplicationsTab()
{
    _loanApplicationsTab = new LoanApplications(ui->toolBar);
    ui->tabWidget->addTab(_loanApplicationsTab, "");
}


void AdminPanelWindow::connects_Admin()
{
    connects_Diller();

    connect(_openLoanAplications, &QAction::triggered, this, &AdminPanelWindow::openLoanAplications);
}

void AdminPanelWindow::connects_Diller()
{
    connect(_openCreateTable, &QAction::triggered, this, &AdminPanelWindow::openCreateTable);
    connect(_openExistingTable, &QAction::triggered, this, &AdminPanelWindow::openExistingTable);
}

void AdminPanelWindow::creatingObjects_Admin()
{
    creatingObjects_Diller();

    _openLoanAplications = new QAction("Заявки на кредиты");
}

void AdminPanelWindow::creatingObjects_Diller()
{
    _openCreateTable = new QAction("Создать стол");
    _openExistingTable = new QAction("Существующие столы");
}

void AdminPanelWindow::openCreateTable()
{
    selectAction();
    ui->tabWidget->setCurrentWidget(_createGameTableTab);
}

void AdminPanelWindow::openExistingTable()
{
    selectAction();
    ui->tabWidget->setCurrentWidget(_existingTablesTab);
}

void AdminPanelWindow::openLoanAplications()
{
    selectAction();
    ui->tabWidget->setCurrentWidget(_loanApplicationsTab);
}

void AdminPanelWindow::rendering_Admin()
{
    rendering_WelcomeTab();
    rendering_ExistingTablesTab();
    rendering_CreateGameTableTab();
    rendering_LoanApplicationsTab();

    creatingObjects_Admin();

    rendering_MenuBar_Admin();

    connects_Admin();
}

void AdminPanelWindow::rendering_Diller()
{
    rendering_WelcomeTab();
    rendering_ExistingTablesTab();
    rendering_CreateGameTableTab();

    creatingObjects_Diller();

    rendering_MenuBar_Diller();

    connects_Diller();
}

void AdminPanelWindow::rendering_MenuBar_Admin()
{
    rendering_MenuBar_Diller();

    ui->menubar->addAction(_openLoanAplications);
}

void AdminPanelWindow::rendering_MenuBar_Diller()
{
    ui->menubar->addAction(_openCreateTable);
    ui->menubar->addAction(_openExistingTable);
}

void AdminPanelWindow::selectAction()
{
    if (_lastAction != nullptr)
        _lastAction->setEnabled(true);

    QAction* currentAction = qobject_cast<QAction*>(sender());
    currentAction->setEnabled(false);

    _lastAction = currentAction;
}

void AdminPanelWindow::assigningValues()
{
    _standardFont.setFamily("Segoe UI");
    _standardFont.setPointSize(18);

    _lastAction = nullptr;
}
