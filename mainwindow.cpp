#include "mainwindow.h"
#include "creategametablewindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    renderingInterface();
    connects();
}

MainWindow::~MainWindow()
{
    delete centralwidget;
}


void MainWindow::openCreateGameTableWin()
{
    CreateGameTableWindow* w = new CreateGameTableWindow();
    w->exec();
    delete w;
}

void MainWindow::openExistingTablesWin()
{
    ExistingTablesWindow* w = new ExistingTablesWindow();
    w->show();
    close();
}

void MainWindow::openLoanApplicationsWin()
{
    LoanApplicationsWindow* w = new LoanApplicationsWindow();
    w->show();
    close();
}

void MainWindow::renderingInterface()
{
    setWindowTitle("Админ панель");
    resize(300, 280);

    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);

    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(20);

    centralwidget = new QWidget(this);

    verticalLayout = new QVBoxLayout(centralwidget);

    _openCreateGameTableWin = new QPushButton(centralwidget);
    _openCreateGameTableWin->setSizePolicy(sizePolicy);
    _openCreateGameTableWin->setFont(font);
    _openCreateGameTableWin->setText("Создать стол игры");
    verticalLayout->addWidget(_openCreateGameTableWin);

    _openExistingTablesWin = new QPushButton(centralwidget);
    _openExistingTablesWin->setSizePolicy(sizePolicy);
    _openExistingTablesWin->setFont(font);
    _openExistingTablesWin->setText("Текущие игры \n(созданные столы)");
    verticalLayout->addWidget(_openExistingTablesWin);

    _openLoanApplicationsWin = new QPushButton(centralwidget);
    _openLoanApplicationsWin->setSizePolicy(sizePolicy);
    _openLoanApplicationsWin->setFont(font);
    _openLoanApplicationsWin->setText("Заявки на кредиты");
    verticalLayout->addWidget(_openLoanApplicationsWin);

    setCentralWidget(centralwidget);
}

void MainWindow::connects()
{
    connect(_openCreateGameTableWin, &QPushButton::clicked, this, &MainWindow::openCreateGameTableWin);
    connect(_openExistingTablesWin, &QPushButton::clicked, this, &MainWindow::openExistingTablesWin);
    connect(_openLoanApplicationsWin, &QPushButton::clicked, this, &MainWindow::openLoanApplicationsWin);
}
