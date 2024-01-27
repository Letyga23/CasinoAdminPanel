#include "adminpanelwindow.h"

AdminPanelWindow::AdminPanelWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _role = Role::getRoleString();

    creatingObjects();
    renderingInterface();
    connects();
}

AdminPanelWindow::~AdminPanelWindow()
{
    delete _mainWidget;
}

void AdminPanelWindow::renderingInterface()
{
    setWindowTitle("Админ панель");
    resize(1100, 750);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(20);

    _mainWidget = new QWidget(this);

    _verticalLayout_Main = new QVBoxLayout(_mainWidget);

    _tabWidget = new QTabWidget;
    _tabWidget->tabBar()->hide();
    _verticalLayout_Main->addWidget(_tabWidget);

    _statusBar = new QStatusBar();
    _statusBar->showMessage(_role);
    setStatusBar(_statusBar);

    setCentralWidget(_mainWidget);

    rendering_CreateGameTableTab();
    rendering_ExistingTablesTab();
    rendering_LoanApplicationsTab();
    rendering_ToolBar();
}

void AdminPanelWindow::rendering_CreateGameTableTab()
{
    _createGameTableTab = new CreateGameTableTab();
    _tabWidget->addTab(_createGameTableTab, "");
}

void AdminPanelWindow::rendering_ExistingTablesTab()
{
    _existingTablesTab = new ExistingTablesWindow();
    _tabWidget->addTab(_existingTablesTab, "");
}

void AdminPanelWindow::rendering_LoanApplicationsTab()
{
    _loanApplicationsTab = new LoanApplicationsWindow();
    _tabWidget->addTab(_loanApplicationsTab, "");
}

void AdminPanelWindow::rendering_ToolBar()
{
    _toolBar = new QToolBar;
    addToolBar(_toolBar);

    _toolBar->addAction(_openCreateTable);
    _toolBar->addAction(_openExistingTable);
    _toolBar->addAction(_openLoanAplications);
}

void AdminPanelWindow::connects()
{
    connect(_openCreateTable, &QAction::triggered, this, &AdminPanelWindow::openCreateTable);
    connect(_openExistingTable, &QAction::triggered, this, &AdminPanelWindow::openExistingTable);
    connect(_openLoanAplications, &QAction::triggered, this, &AdminPanelWindow::openLoanAplications);
}

void AdminPanelWindow::creatingObjects()
{
    _openCreateTable = new QAction;
    _openCreateTable->setText("Создать стол");

    _openExistingTable = new QAction;
    _openExistingTable->setText("Существующие столы");

    _openLoanAplications = new QAction;
    _openLoanAplications->setText("Заявки на кредиты");
}

void AdminPanelWindow::openCreateTable()
{
    _tabWidget->setCurrentWidget(_createGameTableTab);
}

void AdminPanelWindow::openExistingTable()
{
    _tabWidget->setCurrentWidget(_existingTablesTab);
}

void AdminPanelWindow::openLoanAplications()
{
    _tabWidget->setCurrentWidget(_loanApplicationsTab);
}
