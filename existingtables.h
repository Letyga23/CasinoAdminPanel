#ifndef EXISTINGTABLESWINDOW_H
#define EXISTINGTABLESWINDOW_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>
#include <QTableView>
#include <QSqlTableModel>
#include <QTimer>
#include <math.h>
#include <QMutexLocker>
#include <QSqlQuery>
#include <functional>
#include "filterdialog.h"
#include "mythread.h"

#include <QVariant>
#include <QIcon>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QSpacerItem>
#include <QStatusBar>
#include <QVBoxLayout>

class ExistingTables : public QWidget
{
    Q_OBJECT

    int _currentPage;
    int _rowsPerPage;
    int _maxPageModel;
    int _minPageModel;
    int _maxPage;
    int _rowCountModel;
    bool _autoNumRows;
    QString _filter;
    QString _sort;
    QString _typeSearch;
    QString _tableWorkInDB;
    QMutex _mutex;

    QMap<int, QString> _typeSort;

    QVector<QSharedPointer<QSqlQueryModel>> _models;

    QSharedPointer<MyThread> _startTreadModel;
    QSharedPointer<MyThread> _nextTreadModel;
    QSharedPointer<MyThread> _prevTreadModel;
    QSharedPointer<MyThread> _getMaxPageTread;
    QSharedPointer<MyThread> _getNamesColumn;

    QString _like;
    QString _column;
    QSharedPointer<FilterDialog> _filterDialog;

    QTimer _searchTimer;
    QTimer _goToPageTimer;

private:
    QLabel* _labelSearch;
    QLabel* _labelGoToPageNum;
    QLabel* _labelSelectPage;
    QLabel* _labelSortColumn;
    QLabel* _label_5;
    QLabel* _label_6;
    QLabel* _labelCurrentPage;
    QLabel* _labelMaxPage;
    QLabel* _status;

    QVBoxLayout* _verticalLayout;

    QHBoxLayout* _horizontalLayout;
    QHBoxLayout* _horizontalLayout_2;
    QHBoxLayout* _horizontalLayout_3;
    QHBoxLayout* _horizontalLayout_4;
    QHBoxLayout* _horizontalLayout_5;
    QHBoxLayout* _horizontalLayout_6;

    QComboBox* _searchColumn;
    QComboBox* _sortingColumn;
    QComboBox* _typeSorting;

    QLineEdit* _searchText;
    QLineEdit* _pageNumberToNavigate;

    QCheckBox* _checkBox;

    QPushButton* _pushButton_search;
    QPushButton* _clearSearch;
    QPushButton* _addFilter;
    QPushButton* _clearFilter;
    QPushButton* _resetTable;
    QPushButton* _prevButton;
    QPushButton* _nextButton;
    QPushButton* _automaticNumberRows;
    QPushButton* _moreDetailed;

    QVector<QPushButton*> _numberRows;

    QSpacerItem* _horizontalSpacer;
    QSpacerItem* _horizontalSpacer_2;
    QSpacerItem* _horizontalSpacer_3;
    QSpacerItem* _horizontalSpacer_4;
    QSpacerItem* _horizontalSpacer_5;
    QSpacerItem* _horizontalSpacer_6;
    QSpacerItem* _horizontalSpacer_7;
    QSpacerItem* _horizontalSpacer_8;
    QSpacerItem* _horizontalSpacer_9;

    QTableView* _tableView;

    QFont _font1;
    QFont _font2;

    QString _comboBoxStyleSheet;
    QString _pushButtonStyleSheet;

public:
    ExistingTables();
    ~ExistingTables();

private:
    void updateCurrentPageInLabel();
    void updateTablePage();
    void initializationStartModel();
    void loadingModel(QSharedPointer<MyThread> thread, QSharedPointer<QSqlQueryModel> model, int offset);
    void setModel(QSharedPointer<QSqlQueryModel> model);
    void setFilter(const QString& filter);
    void refreshStartModel();
    void blockingInterface(bool flag);
    void startLoadModelFinished();
    void threadFinished();
    void goToPage(int _labelCurrentPage);
    void goToNextModel();
    void goToPrevModel();
    void searchInModels();
    int currentPageInModel();
    void searchInDB();
    void settingValueInComboBox(QComboBox* _searchColumn, QString& headerText);
    void assigningValues();
    void workingWithTableView();
    void creatingObjects();
    void connects();
    void renderingInterface();
    void renderingLayout_1();
    void renderingLayout_2();
    void renderingLayout_3();
    void renderingLayout_4();
    void renderingLayout_5();
    void renderingLayout_6();
    void blockAndOperate(QObject* widget, const std::function<void()>& operation);
    void setValueToMaxPage(int maxPage);
    void resizeEvent(QResizeEvent* event) override;
    void automaticNumberRows();
    void setValueNameColumn(QVector<QString>* namesColumn);

private slots:
    void on_clearSearch_clicked();
    void on_pageNumberToNavigate_textChanged();
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_addFilter_clicked();
    void on_clearFilter_clicked();
    void on_searchText_textChanged();
    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_resetTable_clicked();
    void on_checkBox_stateChanged(int arg1);
    void on_pushButton_search_clicked();
    void onHeaderClicked(int logicalIndex);
    void changeNumberRows();
    void openMoreDetailed();
};
#endif // EXISTINGTABLESWINDOW_H
