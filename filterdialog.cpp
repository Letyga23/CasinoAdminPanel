#include "filterdialog.h"
#include <QDebug>
#include <QIntValidator>
#include <QLineEdit>
#include <QPushButton>
#include <QApplication>

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent)
{
    renderingInterface();
    connects();
    assigningValues();
}

FilterDialog::~FilterDialog()
{
    delete _verticalLayout;
}

void FilterDialog::assigningValues()
{
    _sign =
    {
        {0, ">"},
        {1, "<"}
    };

    _filterSelection =
    {
        {_nameLenght, _filter_1},
        {_surnameLenght, _filter_2},
        {_ending, _filter_3},
        {_beginning, _filter_4}
    };
}

void FilterDialog::connects()
{
    QList<QLineEdit*> lineEdits = findChildren<QLineEdit*>();
    for(QLineEdit* lineEdit : lineEdits)
        connect(lineEdit, &QLineEdit::textChanged, this, &FilterDialog::filterSelection);

    connect(_pushButton, &QPushButton::clicked, this, &FilterDialog::applyFilter);
}

void FilterDialog::renderingInterface()
{
    setWindowTitle("Фильтр");
    resize(466, 257);

    _verticalLayout = new QVBoxLayout(this);

    _comboBoxStyleSheet =   "QComboBox {"
                            "    background-color: #0E9252;"
                            "    border: 1px solid #2980b9;"
                            "    color: #ffffff;"
                            "    padding: 5px;"
                            "    border-radius: 3px;}"

                            "QComboBox:hover {"
                            "    background-color: #42A977;"
                            "    border: 1px solid #1c6da5;}"

                            "QComboBox:disabled {"
                            "    background-color: #d3d3d3;"
                            "    color: #555555;"
                            "    border: 1px solid #a3a3a3;}";

    _pushButtonStyleSheet = "QPushButton {\n"
                            "	background-color: #3498db;"
                            "	border: 1px solid #2980b9;"
                            "	 color: #ffffff;"
                            "	padding: 5px 10px;"
                            "	border-radius: 3px;}"

                            "QPushButton:hover {background-color: #2184cb; border: 1px solid #1c6da5;}"

                            "QPushButton:pressed {"
                            "	background-color: #1a548b;"
                            "	border: 1px solid #174172;}"

                            "QPushButton:disabled {"
                            "	background-color: #d3d3d3;"
                            "	color: #555555;"
                            "	border: 1px solid #a3a3a3;}";

    _font.setFamily(QString::fromUtf8("Segoe UI"));
    _font.setPointSize(12);
    _font.setBold(true);
    _font.setWeight(75);

    renderingFilter_1();
    renderingFilter_2();
    renderingFilter_3();
    renderingFilter_4();
    renderingConfirmationButton();
}

void FilterDialog::renderingFilter_1()
{
    _horizontalLayout = new QHBoxLayout();

    _filter_1 = new QCheckBox(this);
    _filter_1->setFont(_font);
    _filter_1->setText("В имени");
    _horizontalLayout->addWidget(_filter_1);

    _comboBox = new QComboBox(this);
    _comboBox->addItem("больше");
    _comboBox->addItem("меньше");
    _comboBox->setFont(_font);
    _comboBox->setStyleSheet(_comboBoxStyleSheet);
    _horizontalLayout->addWidget(_comboBox);

    _nameLenght = new QLineEdit(this);
    _nameLenght->setFont(_font);
    _nameLenght->setValidator(new QIntValidator(this));
    _horizontalLayout->addWidget(_nameLenght);

    _labelLetter = new QLabel(this);
    _labelLetter->setText("букв");
    _labelLetter->setFont(_font);
    _horizontalLayout->addWidget(_labelLetter);

    _horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout->addItem(_horizontalSpacer);

    _verticalLayout->addLayout(_horizontalLayout);
}

void FilterDialog::renderingFilter_2()
{
    _horizontalLayout_2 = new QHBoxLayout();

    _filter_2 = new QCheckBox(this);
    _filter_2->setFont(_font);
    _filter_2->setText("В фамилии");
    _horizontalLayout_2->addWidget(_filter_2);

    _comboBox_2 = new QComboBox(this);
    _comboBox_2->addItem("больше");
    _comboBox_2->addItem("меньше");
    _comboBox_2->setFont(_font);
    _comboBox_2->setStyleSheet(_comboBoxStyleSheet);
    _horizontalLayout_2->addWidget(_comboBox_2);

    _surnameLenght = new QLineEdit(this);
    _surnameLenght->setFont(_font);
    _surnameLenght->setValidator(new QIntValidator(this));
    _horizontalLayout_2->addWidget(_surnameLenght);

    _label_2_Letter = new QLabel(this);
    _label_2_Letter->setText("букв");
    _label_2_Letter->setFont(_font);
    _horizontalLayout_2->addWidget(_label_2_Letter);

    _horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_2->addItem(_horizontalSpacer_2);

    _verticalLayout->addLayout(_horizontalLayout_2);
}

void FilterDialog::renderingFilter_3()
{
    _horizontalLayout_3 = new QHBoxLayout();

    _filter_3 = new QCheckBox(this);
    _horizontalLayout_3->addWidget(_filter_3);

    _comboBox_3 = new QComboBox(this);
    _comboBox_3->addItem("Имя");
    _comboBox_3->addItem("Фамилия");
    _comboBox_3->setFont(_font);
    _comboBox_3->setStyleSheet(_comboBoxStyleSheet);
    _horizontalLayout_3->addWidget(_comboBox_3);

    _labelEnds = new QLabel(this);
    _labelEnds->setText("заканивается на");
    _labelEnds->setFont(_font);
    _horizontalLayout_3->addWidget(_labelEnds);

    _ending = new QLineEdit(this);
    _ending->setFont(_font);
    _ending->setValidator(new QRegExpValidator(QRegExp("[^\\d\\W_]+")));
    _horizontalLayout_3->addWidget(_ending);

    _horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_3->addItem(_horizontalSpacer_3);

    _verticalLayout->addLayout(_horizontalLayout_3);
}

void FilterDialog::renderingFilter_4()
{
    _horizontalLayout_4 = new QHBoxLayout();

    _filter_4 = new QCheckBox(this);
    _horizontalLayout_4->addWidget(_filter_4);

    _comboBox_4 = new QComboBox(this);
    _comboBox_4->addItem("Имя");
    _comboBox_4->addItem("Фамилия");
    _comboBox_4->setFont(_font);
    _comboBox_4->setStyleSheet(_comboBoxStyleSheet);
    _horizontalLayout_4->addWidget(_comboBox_4);

    _labelStartsWith = new QLabel(this);
    _labelStartsWith->setText("начинается на");
    _labelStartsWith->setFont(_font);
    _horizontalLayout_4->addWidget(_labelStartsWith);

    _beginning = new QLineEdit(this);
    _beginning->setFont(_font);
    _beginning->setValidator(new QRegExpValidator(QRegExp("[^\\d\\W_]+")));
    _horizontalLayout_4->addWidget(_beginning);

    _horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_4->addItem(_horizontalSpacer_4);

    _verticalLayout->addLayout(_horizontalLayout_4);
}

void FilterDialog::renderingConfirmationButton()
{
    _horizontalLayout_5 = new QHBoxLayout();

    _horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_5->addItem(_horizontalSpacer_5);

    _pushButton = new QPushButton(this);
    _pushButton->setText("Применить");
    _pushButton->setFont(_font);
    _pushButton->setStyleSheet(_pushButtonStyleSheet);
    _horizontalLayout_5->addWidget(_pushButton);

    _horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_5->addItem(_horizontalSpacer_6);

    _verticalLayout->addLayout(_horizontalLayout_5);
}

void FilterDialog::applyFilter()
{
    _filter.clear();

    if(_filter_1->isChecked())
    {
        if(!_nameLenght->text().isEmpty())
        {
            QString sign = _sign[_comboBox->currentIndex()];
            _filter += " AND LENGTH(Имя) " + sign + " " + _nameLenght->text();
        }
    }

    if(_filter_2->isChecked())
    {
        if(!_surnameLenght->text().isEmpty())
        {
            QString sign = _sign[_comboBox_2->currentIndex()];
            _filter += " AND LENGTH(Фамилия) " + sign + " " + _surnameLenght->text();
        }
    }

    if(_filter_3->isChecked())
    {
        if(!_ending->text().isEmpty())
            _filter += " AND " + _comboBox_3->currentText() + " LIKE '%" + _ending->text() + "'";
    }

    if(_filter_4->isChecked())
    {
        if(!_beginning->text().isEmpty())
            _filter += " AND " + _comboBox_4->currentText() + " LIKE '" + _beginning->text() + "%'";
    }

    emit filterSelected(_filter);
    accept();
}

void FilterDialog::clearFilter()
{
    for(QCheckBox* filter : findChildren<QCheckBox*>())
        filter->setChecked(false);

    for(QLineEdit* text : findChildren<QLineEdit*>())
        text->clear();

    emit filterSelected("");
}

void FilterDialog::filterSelection()
{
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(sender());

    if(lineEdit->text().isEmpty())
        _filterSelection[lineEdit]->setChecked(false);
    else
        _filterSelection[lineEdit]->setChecked(true);
}

