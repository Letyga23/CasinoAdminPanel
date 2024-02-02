#ifndef CREATEGAMETABLE_H
#define CREATEGAMETABLE_H

#include <QWidget>
#include <QMap>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include "mythread.h"
#include "gamePluginInterface.h"

namespace Ui {
class CreateGameTable;
}

class CreateGameTable : public QWidget
{
    Q_OBJECT

    Ui::CreateGameTable *ui;
    QMap<int, QVector<int>> _optionNumberOfPlayers;
    QSharedPointer<MyThread> _requestTread;
    QTimer _timerHideLabel;;

public:
    explicit CreateGameTable(QWidget *parent = nullptr);
    ~CreateGameTable();

private slots:
    void creating();
    void fillComboBoxNumPlayers();

private:
    void assigningValues();
    void creatingObjects();
    void loadPlugins();
    void addButtonGameStart(QObject* obj);
    void connects();
    void resetData();
    void tableHasBeenCreated();
};

#endif // CREATEGAMETABLE_H
