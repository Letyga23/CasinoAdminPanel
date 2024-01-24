#include "adminpanelwindow.h"

AdminPanelWindow::AdminPanelWindow(QWidget *parent)
    : QMainWindow(parent)
{
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
    resize(1100, 700);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(20);

    _mainWidget = new QWidget(this);

    _verticalLayout_Main = new QVBoxLayout(_mainWidget);

    _tabWidget = new QTabWidget;
    _verticalLayout_Main->addWidget(_tabWidget);

    _statusBar = new QStatusBar();
    setStatusBar(_statusBar);

    setCentralWidget(_mainWidget);

    rendering_Tab_1();
    rendering_Tab_2();
    rendering_Tab_3();

}

void AdminPanelWindow::rendering_Tab_1()
{
    _tab1_Widget = new CreateGameTableTab();
    _tabWidget->addTab(_tab1_Widget, "Страница 1");
}

void AdminPanelWindow::rendering_Tab_2()
{
    _tab2_Widget = new ExistingTablesWindow(_statusBar);
    _tabWidget->addTab(_tab2_Widget, "Страница 2");
}

void AdminPanelWindow::rendering_Tab_3()
{
    _tab3_Widget = new LoanApplicationsWindow(_statusBar);
    _tabWidget->addTab(_tab3_Widget, "Страница 3");
}

void AdminPanelWindow::connects()
{

}
