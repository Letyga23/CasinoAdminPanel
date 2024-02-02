#ifndef LOANAPPLICATIONS_H
#define LOANAPPLICATIONS_H

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
#include <QToolBar>
#include <QComboBox>
#include <QPushButton>
#include "filterdialog_loanapplications.h"
#include "mythread.h"

namespace Ui {
class LoanApplications;
}

class LoanApplications : public QWidget
{
    Q_OBJECT

    Ui::LoanApplications *ui;
    int _currentPage;
    int _rowsPerPage;
    int _maxPageModel;
    int _minPageModel;
    int _maxPage;
    bool _autoNumRows;
    bool _sortingOn;
    QString _filter;
    QString _columtSort;
    QString _typeSort;
    QString _typeSearch;
    QString _tableWorkInDB;
    QMutex _mutex;
    QTimer _resizeTimer;

    QMap<int, QString> _typesSorting;

    QVector<QSharedPointer<QSqlQueryModel>> _models;

    QSharedPointer<MyThread> _startTreadModel;
    QSharedPointer<MyThread> _nextTreadModel;
    QSharedPointer<MyThread> _prevTreadModel;
    QSharedPointer<MyThread> _getMaxPageTread;
    QSharedPointer<MyThread> _getNamesColumn;

    QString _like;
    QString _column;
    QSharedPointer<FilterDialog_LoanApplications> _filterDialog;

    QTimer _searchTimer;
    QTimer _goToPageTimer;

    QString _pushButtonStyleSheet;

    QToolBar* _toolBar;

    QAction* _addFilter;
    QAction* _clearFilter;
    QAction* _resetTable;

    QVector<QPushButton*> _numberRows;

public:
    explicit LoanApplications(QToolBar* toolBar, QWidget *parent = nullptr);
    ~LoanApplications();
    void refreshStartModel();

private:
    void updateCurrentPageInLabel();
    void updateTablePage();
    void initializationStartModel();
    void loadingModel(QSharedPointer<MyThread> thread, QSharedPointer<QSqlQueryModel> model, int offset);
    void setModel(QSharedPointer<QSqlQueryModel> model);
    void setFilter(const QString& filter);
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
    void blockAndOperate(QObject* widget, const std::function<void()>& operation);
    void setValueToMaxPage(int maxPage);
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void hideEvent(QHideEvent* event) override;
    void automaticNumberRows();
    void setValueNameColumn(QVector<QString>* namesColumn);
    void sorting();
    void rendoringToolBar();

private slots:
    void on_clearSearch_clicked();
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void openAddFilters();
    void clearFilters();
    void resetTable();
    void on_checkBox_stateChanged(int arg1);
    void on_sorting_stateChanged(int arg1);
    void on_pushButton_search_clicked();
    void onHeaderClicked(int logicalIndex);
    void changeNumberRows();
    void on_searchColumn_currentIndexChanged(int index);
    void on_searchText_textChanged(const QString &arg1);
    void on_pageNumberToNavigate_textChanged(const QString &arg1);
};

#endif // LOANAPPLICATIONS_H
