#include "creategametable.h"
#include "ui_creategametable.h"
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QDebug>
#include "adminpanelwindow.h"

CreateGameTable::CreateGameTable(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateGameTable)
{
    ui->setupUi(this);

    creatingObjects();
    connects();
    loadPlugins();
    assigningValues();
}

CreateGameTable::~CreateGameTable()
{
    delete ui;
}


void CreateGameTable::creatingObjects()
{
    _requestTread = QSharedPointer<MyThread>::create();
}

void CreateGameTable::connects()
{
    connect(ui->create, &QPushButton::clicked, this, &CreateGameTable::creating);
    connect(ui->selectGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CreateGameTable::fillComboBoxNumPlayers);
    connect(_requestTread.data(), &MyThread::requestFinished, AdminPanelWindow::_existingTablesTab, &ExistingTables::refreshStartModel);
    connect(_requestTread.data(), &MyThread::requestFinished, this, &CreateGameTable::tableHasBeenCreated);
    connect(&_timerHideLabel, &QTimer::timeout, ui->tableHasBeenCreated, &QLabel::hide);
}

void CreateGameTable::loadPlugins()
{
    QDir dir("plugins/GamePlugins");

    if(!dir.exists())
    {
        QMessageBox::critical(0, "", "каталог плагинов не существует" );
        return;
    }

    for(QString strFileName : dir.entryList(QDir::Files))
    {
        QPluginLoader loader(dir.absoluteFilePath(strFileName));
        addButtonGameStart(qobject_cast<QObject*>(loader.instance()));
    }
    fillComboBoxNumPlayers();
}

void CreateGameTable::addButtonGameStart(QObject* obj)
{
    if(!obj)
        return;

    GamePluginInterface* gameI = qobject_cast<GamePluginInterface*>(obj);
    if(gameI)
    {
        ui->selectGame->blockSignals(true);
        ui->selectGame->addItem(gameI->namePlugin());
        ui->selectGame->blockSignals(false);

        _optionNumberOfPlayers.insert(ui->selectGame->count() - 1, gameI->getOptionNumberOfPlayers());
    }

    delete gameI;
}

void CreateGameTable::creating()
{
    QList<QLineEdit*> lineEdits = findChildren<QLineEdit*>();
    for(QLineEdit* lineEdit : lineEdits)
    {
        if(lineEdit->text().isEmpty())
        {
            QMessageBox::warning(this, "Внимание", "Заполните все данные", QMessageBox::Ok);
            return;
        }
    }

    QString maxNumberPlayers = ui->numPlayers->currentText();
    QString minBid = ui->minBid->text();
    QString minRateIncreaseStep = ui->minRateIncreaseStep->text();
    QString minBudget = ui->minBudget->text();
    QString nameGame = ui->selectGame->currentText();

    QString requst = "INSERT INTO ExistingTables (MaxNumberPlayers, MinBid, MinRateIncreaseStep, MinBudget, NameGame)"
                     "VALUES (" + maxNumberPlayers + ", " + minBid + ", " + minRateIncreaseStep + ", " + minBudget + ", '" + nameGame + "')";

    _requestTread->request(requst);
}

void CreateGameTable::tableHasBeenCreated()
{
    resetData();
    ui->tableHasBeenCreated->setVisible(true);
    int timeout = 2000;
    _timerHideLabel.start(timeout);
}

void CreateGameTable::fillComboBoxNumPlayers()
{
    ui->numPlayers->clear();

    for(int numPlayer : _optionNumberOfPlayers[ui->selectGame->currentIndex()])
        ui->numPlayers->addItem(QString::number(numPlayer));
}

void CreateGameTable::resetData()
{
    ui->numPlayers->setCurrentIndex(0);
    ui->minBid->clear();
    ui->minRateIncreaseStep->clear();
    ui->minBudget->clear();
    ui->selectGame->setCurrentIndex(0);
}

void CreateGameTable::assigningValues()
{
    _timerHideLabel.setSingleShot(true);
    ui->tableHasBeenCreated->setVisible(false);
    ui->minBid->setValidator(new QIntValidator);
    ui->minRateIncreaseStep->setValidator(new QIntValidator);
    ui->minBudget->setValidator(new QIntValidator);
}
