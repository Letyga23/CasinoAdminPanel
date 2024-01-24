#ifndef CREATEGAMETABLEWINDOW_H
#define CREATEGAMETABLEWINDOW_H

#include <QWidget>
#include <QMap>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include "gamePluginInterface.h"

class CreateGameTableTab : public QWidget
{
    QMap<int, GamePluginInterface*> _games;

private:
    QVBoxLayout *verticalLayout;

    QGridLayout *gridLayout;

    QLabel *label;
    QLabel *label_2;

    QComboBox* comboBox;
    QComboBox *selectGame;

    QLineEdit *lineEdit;

    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_4;

    QPushButton *create;

    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;

    QFont _fontLabel;
    QFont _fontBold;

public:
    explicit CreateGameTableTab();
    ~CreateGameTableTab();

private slots:
    void creating();
    void fillComboBoxNumPlayers(int index);

private:
    void loadPlugins();
    void addButtonGameStart(QObject* obj);
    void renderingInterface();
    void renderingLayut_1();
    void renderingLayut_2();
    void renderingLayut_3();
    void renderingLayut_4();
    void connects();
};

#endif // CREATEGAMETABLEWINDOW_H
