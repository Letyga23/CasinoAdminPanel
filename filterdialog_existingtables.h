#ifndef FILTERDIALOG_EXISTINGTABLES_H
#define FILTERDIALOG_EXISTINGTABLES_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

namespace Ui {
class FilterDialog_ExistingTables;
}

class FilterDialog_ExistingTables : public QDialog
{
    Q_OBJECT

    Ui::FilterDialog_ExistingTables *ui;
    QString _filter;
    QMap<int, QString> _sign;
    QMap<QLineEdit*, QCheckBox*> _filterSelection;
    QString _comboBoxStyleSheet;
    QString _pushButtonStyleSheet;
    QFont _font;

public:
    explicit FilterDialog_ExistingTables(QWidget *parent = nullptr);
    ~FilterDialog_ExistingTables();

    void clearFilter();

private:
    void assigningValues();
    void connects();

private slots:
    void applyFilter();
    void filterSelection();

signals:
    void filterSelected(QString filter);
};

#endif // FILTERDIALOG_EXISTINGTABLES_H
