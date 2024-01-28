#ifndef CREATEGAMETABLEWINDOW_H
#define CREATEGAMETABLEWINDOW_H

#include <QWidget>
#include <QMap>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include "mythread.h"
#include "gamePluginInterface.h"

class CreateGameTableTab : public QWidget
{
    Q_OBJECT

    QMap<int, GamePluginInterface*> _games;
    QSharedPointer<MyThread> _requestTread;
    QTimer _timerHideLabel;;

private:
    QVBoxLayout* _verticalLayout;

    QLabel* _label;
    QLabel* _label_2;
    QLabel* _label_3;
    QLabel* _label_4;
    QLabel* _tableHasBeenCreated;

    QComboBox* _numPlayers;
    QComboBox* _selectGame;

    QLineEdit* _minBid;
    QLineEdit* _minRateIncreaseStep;
    QLineEdit* _minBudget;

    QHBoxLayout* _horizontalLayout;
    QHBoxLayout* _horizontalLayout_2;
    QHBoxLayout* _horizontalLayout_3;
    QHBoxLayout* _horizontalLayout_4;
    QHBoxLayout* _horizontalLayout_5;
    QHBoxLayout* _horizontalLayout_6;

    QPushButton* _create;

    QSpacerItem* _horizontalSpacer;
    QSpacerItem* _horizontalSpacer_2;
    QSpacerItem* _horizontalSpacer_3;
    QSpacerItem* _horizontalSpacer_4;
    QSpacerItem* _horizontalSpacer_5;
    QSpacerItem* _horizontalSpacer_6;

    QFont _fontLabel;
    QFont _fontBold;

public:
    explicit CreateGameTableTab();
    ~CreateGameTableTab();

private slots:
    void creating();
    void fillComboBoxNumPlayers(int index);

private:
    void creatingObjects();
    void loadPlugins();
    void addButtonGameStart(QObject* obj);
    void renderingInterface();
    void renderingLayut_1();
    void renderingLayut_2();
    void renderingLayut_3();
    void renderingLayut_4();
    void renderingLayut_5();
    void renderingLayut_6();
    void renderingLayut_7();
    void connects();
    void resetData();
    void tableHasBeenCreated();
};

#endif // CREATEGAMETABLEWINDOW_H
