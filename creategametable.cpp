#include "creategametable.h"
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QDebug>
#include "adminpanelwindow.h"

CreateGameTableTab::CreateGameTableTab()
{
    renderingInterface();
    connects();
    loadPlugins();
}

CreateGameTableTab::~CreateGameTableTab()
{
    delete this;
}

void CreateGameTableTab::renderingInterface()
{
    _fontLabel.setFamily("Segoe UI");
    _fontLabel.setPointSize(14);

    _fontBold.setFamily("Segoe UI");
    _fontBold.setPointSize(18);
    _fontBold.setBold(true);

    verticalLayout = new QVBoxLayout(this);

    renderingLayut_1();
    renderingLayut_2();
    renderingLayut_3();
    renderingLayut_4();
    renderingLayut_5();
    renderingLayut_6();
}

void CreateGameTableTab::renderingLayut_1()
{
    horizontalLayout = new QHBoxLayout();

    label = new QLabel(this);
    label->setText("Кол-во игроков: ");
    label->setFont(_fontLabel);
    horizontalLayout->addWidget(label);

    comboBox = new QComboBox(this);
    comboBox->setFont(_fontBold);
    comboBox->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    horizontalLayout->addWidget(comboBox);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);

    verticalLayout->addLayout(horizontalLayout);
}

void CreateGameTableTab::renderingLayut_2()
{
    horizontalLayout_2 = new QHBoxLayout();

    label_2 = new QLabel();
    label_2->setText("Минимальная ставка: ");
    label_2->setFont(_fontLabel);
    horizontalLayout_2->addWidget(label_2);

    lineEdit = new QLineEdit(this);
    lineEdit->setFont(_fontBold);
    lineEdit->setValidator(new QIntValidator());
    lineEdit->setMaximumWidth(150);
    lineEdit->setMaxLength(9);
    horizontalLayout_2->addWidget(lineEdit);

    horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_4);

    verticalLayout->addLayout(horizontalLayout_2);
}

void CreateGameTableTab::renderingLayut_3()
{
    horizontalLayout_3= new QHBoxLayout();

    label_3 = new QLabel();
    label_3->setText("Минимальный шаг \n"
                     "повышения ставки: ");
    label_3->setFont(_fontLabel);
    horizontalLayout_3->addWidget(label_3);

    lineEdit_2 = new QLineEdit(this);
    lineEdit_2->setFont(_fontBold);
    lineEdit_2->setValidator(new QIntValidator());
    lineEdit_2->setMaximumWidth(150);
    lineEdit_2->setMaxLength(9);
    horizontalLayout_3->addWidget(lineEdit_2);

    horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_3->addItem(horizontalSpacer_5);

    verticalLayout->addLayout(horizontalLayout_3);
}

void CreateGameTableTab::renderingLayut_4()
{
    horizontalLayout_4= new QHBoxLayout();

    label_4 = new QLabel();
    label_4->setText("Минимальный бюджет \n"
                     "для начала игры: ");
    label_4->setFont(_fontLabel);
    horizontalLayout_4->addWidget(label_4);


    lineEdit_3 = new QLineEdit(this);
    lineEdit_3->setFont(_fontBold);
    lineEdit_3->setValidator(new QIntValidator());
    lineEdit_3->setMaximumWidth(150);
    lineEdit_3->setMaxLength(9);
    horizontalLayout_4->addWidget(lineEdit_3);

    horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_4->addItem(horizontalSpacer_6);
    verticalLayout->addLayout(horizontalLayout_4);
}

void CreateGameTableTab::renderingLayut_5()
{
    horizontalLayout_5= new QHBoxLayout();

    selectGame = new QComboBox(this);
    selectGame->setFont(_fontBold);
    selectGame->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    horizontalLayout_5->addWidget(selectGame);

    verticalLayout->addLayout(horizontalLayout_5);
}

void CreateGameTableTab::renderingLayut_6()
{
    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(20);

    horizontalLayout_6= new QHBoxLayout();

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_6->addItem(horizontalSpacer_2);

    create = new QPushButton(this);
    create->setText("Создать");
    create->setFont(font);
    horizontalLayout_6->addWidget(create);

    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_6->addItem(horizontalSpacer_3);

    verticalLayout->addLayout(horizontalLayout_6);
}

void CreateGameTableTab::connects()
{
    connect(create, &QPushButton::clicked, this, &CreateGameTableTab::creating);
    connect(selectGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CreateGameTableTab::fillComboBoxNumPlayers);
}

void CreateGameTableTab::loadPlugins()
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

void CreateGameTableTab::addButtonGameStart(QObject* obj)
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

void CreateGameTableTab::creating()
{
    _games[selectGame->currentIndex()]->openGameWindow();
}

void CreateGameTableTab::fillComboBoxNumPlayers(int index)
{
    QVector<int> optionNumberOfPlayers;
    if (index >= 0 && index < _games.size())
        optionNumberOfPlayers = _games[index]->getOptionNumberOfPlayers();

    comboBox->clear();

    for(int numPlayer : optionNumberOfPlayers)
        comboBox->addItem(QString::number(numPlayer));
}
