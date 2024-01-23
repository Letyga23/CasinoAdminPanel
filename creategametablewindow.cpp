#include "creategametablewindow.h"
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>

CreateGameTableWindow::CreateGameTableWindow(QWidget *parent) :
    QDialog(parent)
{
    renderingInterface();
    connects();

    loadPlugins();
}

CreateGameTableWindow::~CreateGameTableWindow()
{
    delete verticalLayout;
}

void CreateGameTableWindow::renderingInterface()
{
    setWindowTitle("Создание стола");

    verticalLayout = new QVBoxLayout(this);

    _fontLabel.setFamily("Segoe UI");
    _fontLabel.setPointSize(14);

    _fontBold.setFamily("Segoe UI");
    _fontBold.setPointSize(18);
    _fontBold.setBold(true);

    renderingLayut_1();
    renderingLayut_2();
    renderingLayut_3();
    renderingLayut_4();
}

void CreateGameTableWindow::renderingLayut_1()
{
    horizontalLayout = new QHBoxLayout();

    label = new QLabel(this);
    label->setText("Кол-во игроков:");
    label->setFont(_fontLabel);
    horizontalLayout->addWidget(label);

    comboBox = new QComboBox(this);
    comboBox->setFont(_fontBold);
    comboBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    horizontalLayout->addWidget(comboBox);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);

    verticalLayout->addLayout(horizontalLayout);
}

void CreateGameTableWindow::renderingLayut_2()
{
    horizontalLayout_2 = new QHBoxLayout();

    label_2 = new QLabel(this);
    label_2->setText("Мин. ставка(руб.):");
    label_2->setFont(_fontLabel);
    horizontalLayout_2->addWidget(label_2);

    lineEdit = new QLineEdit(this);
    lineEdit->setFont(_fontBold);
    lineEdit->setValidator(new QIntValidator(this));
    lineEdit->setMaximumWidth(100);
    lineEdit->setMaxLength(6);
    horizontalLayout_2->addWidget(lineEdit);

    verticalLayout->addLayout(horizontalLayout_2);
}

void CreateGameTableWindow::renderingLayut_3()
{
    horizontalLayout_3= new QHBoxLayout();

    selectGame = new QComboBox(this);
    selectGame->setFont(_fontBold);
    selectGame->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    horizontalLayout_3->addWidget(selectGame);

    verticalLayout->addLayout(horizontalLayout_3);
}

void CreateGameTableWindow::renderingLayut_4()
{
    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(20);

    horizontalLayout_4= new QHBoxLayout();

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer_2);

    create = new QPushButton(this);
    create->setText("Создать");
    create->setFont(font);
    horizontalLayout_4->addWidget(create);

    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer_3);

    verticalLayout->addLayout(horizontalLayout_4);
}

void CreateGameTableWindow::connects()
{
    connect(create, &QPushButton::clicked, this, &CreateGameTableWindow::creating);
    connect(selectGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CreateGameTableWindow::fillComboBoxNumPlayers);
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
    fillComboBoxNumPlayers(selectGame->currentIndex());
}

void CreateGameTableWindow::addButtonGameStart(QObject* obj)
{
    if(!obj)
        return;

    GamePluginInterface* gameI = qobject_cast<GamePluginInterface*>(obj);
    if(gameI)
    {
        selectGame->blockSignals(true);
        selectGame->addItem(gameI->namePlugin());
        selectGame->blockSignals(false);

        _games.insert(selectGame->count() - 1, gameI);
    }
}

void CreateGameTableWindow::creating()
{
    _games[selectGame->currentIndex()]->openGameWindow();
    accept();
}

void CreateGameTableWindow::fillComboBoxNumPlayers(int index)
{
    QVector<int> optionNumberOfPlayers;
    if (index >= 0 && index < _games.size())
        optionNumberOfPlayers = _games[index]->getOptionNumberOfPlayers();

    comboBox->clear();

    for(int numPlayer : optionNumberOfPlayers)
        comboBox->addItem(QString::number(numPlayer));
}
