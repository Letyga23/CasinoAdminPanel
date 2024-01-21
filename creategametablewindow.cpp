#include "creategametablewindow.h"
#include "ui_creategametablewindow.h"
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>

QVector<QObject*> CreateGameTableWindow::games;

CreateGameTableWindow::CreateGameTableWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::createGameTableWindow)
{
    ui->setupUi(this);
    loadPlugins();

    QFont font;
    font.setFamily(QString::fromUtf8("Segoe UI"));
    font.setPointSize(18);
    font.setBold(true);

    ui->selectGame->setFont(font);

    ui->comboBox->setFont(font);

    ui->lineEdit_2->setValidator(new QIntValidator(this));
    ui->lineEdit_2->setFont(font);
}

CreateGameTableWindow::~CreateGameTableWindow()
{
    delete ui;
}

void CreateGameTableWindow::loadPlugins()
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
    fillComboBoxNumPlayers(ui->selectGame->currentIndex());
}

void CreateGameTableWindow::addButtonGameStart(QObject* obj)
{
    if(!obj)
        return;

    GamePluginInterface* gameI = qobject_cast<GamePluginInterface*>(obj);
    if(gameI)
    {
        ui->selectGame->blockSignals(true);
        ui->selectGame->addItem(gameI->namePlugin());
        ui->selectGame->blockSignals(false);

        _games.insert(ui->selectGame->count() - 1, gameI);
    }
}

void CreateGameTableWindow::on_pushButton_clicked()
{
    _games[ui->selectGame->currentIndex()]->openGameWindow();
    accept();
}

void CreateGameTableWindow::closeEvent(QCloseEvent* event)
{
    qDeleteAll(games);
    games.clear();

    QWidget::closeEvent(event);
}

void CreateGameTableWindow::fillComboBoxNumPlayers(int index)
{
    QVector<int> optionNumberOfPlayers;
    if (index >= 0 && index < _games.size())
        optionNumberOfPlayers = _games[index]->getOptionNumberOfPlayers();

    ui->comboBox->clear();

    for(int numPlayer : optionNumberOfPlayers)
        ui->comboBox->addItem(QString::number(numPlayer));
}

void CreateGameTableWindow::on_selectGame_currentIndexChanged(int index)
{
    fillComboBoxNumPlayers(index);
}
