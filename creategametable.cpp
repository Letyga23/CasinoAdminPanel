#include "creategametable.h"
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QDebug>
#include "adminpanelwindow.h"

CreateGameTableTab::CreateGameTableTab()
{
    renderingInterface();
    creatingObjects();
    connects();
    loadPlugins();

    _timerHideLabel.setSingleShot(true);
}

CreateGameTableTab::~CreateGameTableTab()
{
    delete _verticalLayout;
}

void CreateGameTableTab::creatingObjects()
{
    _requestTread = QSharedPointer<MyThread>::create();
}

void CreateGameTableTab::renderingInterface()
{
    _fontLabel.setFamily("Segoe UI");
    _fontLabel.setPointSize(14);

    _fontBold.setFamily("Segoe UI");
    _fontBold.setPointSize(18);
    _fontBold.setBold(true);

    _verticalLayout = new QVBoxLayout(this);

    renderingLayut_1();
    renderingLayut_2();
    renderingLayut_3();
    renderingLayut_4();
    renderingLayut_5();
    renderingLayut_6();
    renderingLayut_7();
}

void CreateGameTableTab::renderingLayut_1()
{
    _horizontalLayout = new QHBoxLayout();

    _label = new QLabel(this);
    _label->setText("Кол-во игроков: ");
    _label->setFont(_fontLabel);
    _horizontalLayout->addWidget(_label);

    _numPlayers = new QComboBox(this);
    _numPlayers->setFont(_fontBold);
    _numPlayers->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    _horizontalLayout->addWidget(_numPlayers);

    _horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout->addItem(_horizontalSpacer);

    _verticalLayout->addLayout(_horizontalLayout);
}

void CreateGameTableTab::renderingLayut_2()
{
    _horizontalLayout_2 = new QHBoxLayout();

    _label_2 = new QLabel();
    _label_2->setText("Минимальная ставка: ");
    _label_2->setFont(_fontLabel);
    _horizontalLayout_2->addWidget(_label_2);

    _minBid = new QLineEdit(this);
    _minBid->setFont(_fontBold);
    _minBid->setValidator(new QIntValidator());
    _minBid->setMaximumWidth(150);
    _minBid->setMaxLength(9);
    _horizontalLayout_2->addWidget(_minBid);

    _horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_2->addItem(_horizontalSpacer_4);

    _verticalLayout->addLayout(_horizontalLayout_2);
}

void CreateGameTableTab::renderingLayut_3()
{
    _horizontalLayout_3= new QHBoxLayout();

    _label_3 = new QLabel();
    _label_3->setText("Минимальный шаг \n"
                      "повышения ставки: ");
    _label_3->setFont(_fontLabel);
    _horizontalLayout_3->addWidget(_label_3);

    _minRateIncreaseStep = new QLineEdit(this);
    _minRateIncreaseStep->setFont(_fontBold);
    _minRateIncreaseStep->setValidator(new QIntValidator());
    _minRateIncreaseStep->setMaximumWidth(150);
    _minRateIncreaseStep->setMaxLength(9);
    _horizontalLayout_3->addWidget(_minRateIncreaseStep);

    _horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_3->addItem(_horizontalSpacer_5);

    _verticalLayout->addLayout(_horizontalLayout_3);
}

void CreateGameTableTab::renderingLayut_4()
{
    _horizontalLayout_4= new QHBoxLayout();

    _label_4 = new QLabel();
    _label_4->setText("Минимальный бюджет \n"
                      "для начала игры: ");
    _label_4->setFont(_fontLabel);
    _horizontalLayout_4->addWidget(_label_4);


    _minBudget = new QLineEdit(this);
    _minBudget->setFont(_fontBold);
    _minBudget->setValidator(new QIntValidator());
    _minBudget->setMaximumWidth(150);
    _minBudget->setMaxLength(9);
    _horizontalLayout_4->addWidget(_minBudget);

    _horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_4->addItem(_horizontalSpacer_6);

    _verticalLayout->addLayout(_horizontalLayout_4);
}

void CreateGameTableTab::renderingLayut_5()
{
    _horizontalLayout_5= new QHBoxLayout();

    _selectGame = new QComboBox(this);
    _selectGame->setFont(_fontBold);
    _selectGame->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    _horizontalLayout_5->addWidget(_selectGame);

    _verticalLayout->addLayout(_horizontalLayout_5);
}

void CreateGameTableTab::renderingLayut_6()
{
    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(20);

    _horizontalLayout_6= new QHBoxLayout();

    _horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_6->addItem(_horizontalSpacer_2);

    _create = new QPushButton(this);
    _create->setText("Создать");
    _create->setFont(font);
    _horizontalLayout_6->addWidget(_create);

    _horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_6->addItem(_horizontalSpacer_3);

    _verticalLayout->addLayout(_horizontalLayout_6);
}

void CreateGameTableTab::renderingLayut_7()
{
    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(20);
    font.setBold(true);

    _tableHasBeenCreated = new QLabel();
    _tableHasBeenCreated->setText("Стол создан");
    _tableHasBeenCreated->setFont(font);
    _tableHasBeenCreated->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    _tableHasBeenCreated->setAlignment(Qt::AlignCenter);
    _tableHasBeenCreated->setVisible(false);
    _verticalLayout->addWidget(_tableHasBeenCreated);
}

void CreateGameTableTab::connects()
{
    connect(_create, &QPushButton::clicked, this, &CreateGameTableTab::creating);
    connect(_selectGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CreateGameTableTab::fillComboBoxNumPlayers);
    connect(_requestTread.data(), &MyThread::requestFinished, AdminPanelWindow::_existingTablesTab, &ExistingTables::refreshStartModel);
    connect(_requestTread.data(), &MyThread::requestFinished, this, &CreateGameTableTab::tableHasBeenCreated);
    connect(&_timerHideLabel, &QTimer::timeout, _tableHasBeenCreated, &QLabel::hide);
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
    fillComboBoxNumPlayers();
}

void CreateGameTableTab::addButtonGameStart(QObject* obj)
{
    if(!obj)
        return;

    GamePluginInterface* gameI = qobject_cast<GamePluginInterface*>(obj);
    if(gameI)
    {
        _selectGame->blockSignals(true);
        _selectGame->addItem(gameI->namePlugin());
        _selectGame->blockSignals(false);

        _optionNumberOfPlayers.insert(_selectGame->count() - 1, gameI->getOptionNumberOfPlayers());
    }

    delete gameI;
}

void CreateGameTableTab::creating()
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

    QString maxNumberPlayers = _numPlayers->currentText();
    QString minBid = _minBid->text();
    QString minRateIncreaseStep = _minRateIncreaseStep->text();
    QString minBudget = _minBudget->text();
    QString nameGame = _selectGame->currentText();

    QString requst = "INSERT INTO ExistingTables (MaxNumberPlayers, MinBid, MinRateIncreaseStep, MinBudget, NameGame)"
                     "VALUES (" + maxNumberPlayers + ", " + minBid + ", " + minRateIncreaseStep + ", " + minBudget + ", '" + nameGame + "')";

    _requestTread->request(requst);
}

void CreateGameTableTab::tableHasBeenCreated()
{
    resetData();
    _tableHasBeenCreated->setVisible(true);
    int timeout = 2000;
    _timerHideLabel.start(timeout);
}

void CreateGameTableTab::fillComboBoxNumPlayers()
{    
    _numPlayers->clear();

    for(int numPlayer : _optionNumberOfPlayers[_selectGame->currentIndex()])
        _numPlayers->addItem(QString::number(numPlayer));
}

void CreateGameTableTab::resetData()
{
    _numPlayers->setCurrentIndex(0);
    _minBid->clear();
    _minRateIncreaseStep->clear();
    _minBudget->clear();
    _selectGame->setCurrentIndex(0);
}
