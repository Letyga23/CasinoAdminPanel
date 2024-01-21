#ifndef CREATEGAMETABLEWINDOW_H
#define CREATEGAMETABLEWINDOW_H

#include <QDialog>
#include <QMap>
#include "gamePluginInterface.h"

namespace Ui {
class createGameTableWindow;
}

class CreateGameTableWindow : public QDialog
{
    Q_OBJECT
    Ui::createGameTableWindow *ui;
    QMap<int, GamePluginInterface*> _games;
    static QVector<QObject*> games;

public:
    explicit CreateGameTableWindow(QWidget *parent = nullptr);
    ~CreateGameTableWindow();

private slots:
    void on_pushButton_clicked();
    void on_selectGame_currentIndexChanged(int index);

private:
    void loadPlugins();
    void addButtonGameStart(QObject* obj);
    void closeEvent(QCloseEvent* event) override;
    void fillComboBoxNumPlayers(int index);
};

#endif // CREATEGAMETABLEWINDOW_H
