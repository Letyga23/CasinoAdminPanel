#include "adminpanelwindow.h"

CreateGameTable* AdminPanelWindow::_createGameTableTab;
ExistingTables* AdminPanelWindow::_existingTablesTab;
LoanApplications* AdminPanelWindow::_loanApplicationsTab;

AdminPanelWindow::AdminPanelWindow(QWidget *parent)
    : QMainWindow(parent)
{
    assigningValues();
    renderingInterface();
}

AdminPanelWindow::~AdminPanelWindow()
{
    delete _mainWidget;
    delete _toolBar;
    delete _menuBar;
    delete _statusBar;
}

void AdminPanelWindow::renderingInterface()
{
    setWindowTitle("Админ панель");
    resize(1200, 750);

    _mainWidget = new QWidget(this);
    setCentralWidget(_mainWidget);

    _verticalLayout_Main = new QVBoxLayout(_mainWidget);

    _tabWidget = new QTabWidget;
    _tabWidget->tabBar()->hide();
    _verticalLayout_Main->addWidget(_tabWidget);

    _statusBar = new QStatusBar();
    _statusBar->showMessage(Role::getRoleString());
    setStatusBar(_statusBar);

    _menuBar = new QMenuBar;
    setMenuBar(_menuBar);

    _toolBar = new QToolBar;
    _toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _toolBar->setStyleSheet("QToolBar { spacing: 10px; }");
    addToolBar(_toolBar);

    switch (Role::getRoleEnum())
    {
    case RoleEnum::Administrator :
        rendering_Admin();
        break;

    case RoleEnum::Dealer :
        rendering_Diller();
        break;
    }

_menuBar->setFont(_standardFont);
}

void AdminPanelWindow::rendering_WelcomeTab()
{
    _welcomTab = new Welcome(Role::getRoleString());
    _tabWidget->addTab(_welcomTab, "");
}

void AdminPanelWindow::rendering_CreateGameTableTab()
{
    _createGameTableTab = new CreateGameTable();
    _tabWidget->addTab(_createGameTableTab, "");
}

void AdminPanelWindow::rendering_ExistingTablesTab()
{
    _existingTablesTab = new ExistingTables(_toolBar);
    _tabWidget->addTab(_existingTablesTab, "");
}

void AdminPanelWindow::rendering_LoanApplicationsTab()
{
    _loanApplicationsTab = new LoanApplications(_toolBar);
    _tabWidget->addTab(_loanApplicationsTab, "");
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
    _tabWidget->setCurrentWidget(_createGameTableTab);
}

void AdminPanelWindow::openExistingTable()
{
    selectAction();
    _tabWidget->setCurrentWidget(_existingTablesTab);
}

void AdminPanelWindow::openLoanAplications()
{
    selectAction();
    _tabWidget->setCurrentWidget(_loanApplicationsTab);
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

    _menuBar->addAction(_openLoanAplications);
}

void AdminPanelWindow::rendering_MenuBar_Diller()
{
    _menuBar->addAction(_openCreateTable);
    _menuBar->addAction(_openExistingTable);
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
