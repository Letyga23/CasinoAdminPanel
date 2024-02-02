#ifndef FILTERDIALOG_LOANAPPLICATIONS_H
#define FILTERDIALOG_LOANAPPLICATIONS_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

namespace Ui {
class FilterDialog_LoanApplications;
}

class FilterDialog_LoanApplications : public QDialog
{
    Q_OBJECT

    Ui::FilterDialog_LoanApplications *ui;
    QString _filter;
    QMap<int, QString> _sign;
    QMap<QLineEdit*, QCheckBox*> _filterSelection;
    QString _comboBoxStyleSheet;
    QString _pushButtonStyleSheet;
    QFont _font;

public:
    explicit FilterDialog_LoanApplications(QWidget *parent = nullptr);
    ~FilterDialog_LoanApplications();

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

#endif // FILTERDIALOG_LOANAPPLICATIONS_H
