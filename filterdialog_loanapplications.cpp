#include "filterdialog_loanapplications.h"
#include "ui_filterdialog_loanapplications.h"

FilterDialog_LoanApplications::FilterDialog_LoanApplications(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog_LoanApplications)
{
    ui->setupUi(this);

    connects();
    assigningValues();
}

FilterDialog_LoanApplications::~FilterDialog_LoanApplications()
{
    delete ui;
}

void FilterDialog_LoanApplications::assigningValues()
{
    _sign =
    {
        {0, ">"},
        {1, "<"}
    };

//    _filterSelection =
//    {
//        {_nameLenght, _filter_1},
//        {_surnameLenght, _filter_2},
//        {_ending, _filter_3},
//        {_beginning, _filter_4}
//    };
}

void FilterDialog_LoanApplications::connects()
{
//    QList<QLineEdit*> lineEdits = findChildren<QLineEdit*>();
//    for(QLineEdit* lineEdit : lineEdits)
//        connect(lineEdit, &QLineEdit::textChanged, this, &FilterDialog_LoanApplications::filterSelection);

    connect(ui->pushButton, &QPushButton::clicked, this, &FilterDialog_LoanApplications::applyFilter);
}


void FilterDialog_LoanApplications::applyFilter()
{
//    _filter.clear();

//    if(_filter_1->isChecked())
//    {
//        if(!_nameLenght->text().isEmpty())
//        {
//            QString sign = _sign[_comboBox->currentIndex()];
//            _filter += " AND LENGTH(Имя) " + sign + " " + _nameLenght->text();
//        }
//    }

//    if(_filter_2->isChecked())
//    {
//        if(!_surnameLenght->text().isEmpty())
//        {
//            QString sign = _sign[_comboBox_2->currentIndex()];
//            _filter += " AND LENGTH(Фамилия) " + sign + " " + _surnameLenght->text();
//        }
//    }

//    if(_filter_3->isChecked())
//    {
//        if(!_ending->text().isEmpty())
//            _filter += " AND " + _comboBox_3->currentText() + " LIKE '%" + _ending->text() + "'";
//    }

//    if(_filter_4->isChecked())
//    {
//        if(!_beginning->text().isEmpty())
//            _filter += " AND " + _comboBox_4->currentText() + " LIKE '" + _beginning->text() + "%'";
//    }

    emit filterSelected(_filter);
    accept();
}

void FilterDialog_LoanApplications::clearFilter()
{
    for(QCheckBox* filter : findChildren<QCheckBox*>())
        filter->setChecked(false);

    for(QLineEdit* text : findChildren<QLineEdit*>())
        text->clear();

    emit filterSelected("");
}

void FilterDialog_LoanApplications::filterSelection()
{
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());

    if(lineEdit->text().isEmpty())
        _filterSelection[lineEdit]->setChecked(false);
    else
        _filterSelection[lineEdit]->setChecked(true);
}
