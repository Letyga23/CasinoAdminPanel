#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

namespace Ui {
class filterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT
    QString _filter;
    QMap<int, QString> _sign;
    QMap<QLineEdit*, QCheckBox*> _filterSelection;
    QString _comboBoxStyleSheet;
    QString _pushButtonStyleSheet;
    QFont _font;

private:
    QVBoxLayout* _verticalLayout;

    QHBoxLayout* _horizontalLayout;
    QHBoxLayout* _horizontalLayout_2;
    QHBoxLayout* _horizontalLayout_3;
    QHBoxLayout* _horizontalLayout_4;
    QHBoxLayout* _horizontalLayout_5;

    QCheckBox* _filter_1;
    QCheckBox* _filter_2;
    QCheckBox* _filter_3;
    QCheckBox* _filter_4;

    QComboBox* _comboBox;
    QComboBox* _comboBox_2;
    QComboBox* _comboBox_3;
    QComboBox* _comboBox_4;

    QLabel* _labelLetter;
    QLabel* _label_2_Letter;
    QLabel* _labelEnds;
    QLabel* _labelStartsWith;

    QLineEdit* _nameLenght;
    QLineEdit* _surnameLenght;
    QLineEdit* _ending;
    QLineEdit* _beginning;

    QSpacerItem* _horizontalSpacer;
    QSpacerItem* _horizontalSpacer_2;
    QSpacerItem* _horizontalSpacer_3;
    QSpacerItem* _horizontalSpacer_4;
    QSpacerItem* _horizontalSpacer_5;
    QSpacerItem* _horizontalSpacer_6;

    QPushButton* _pushButton;


public:
    explicit FilterDialog(QWidget* parent = nullptr);
    ~FilterDialog();

    void clearFilter();

private:
    void assigningValues();
    void connects();
    void renderingInterface();
    void renderingFilter_1();
    void renderingFilter_2();
    void renderingFilter_3();
    void renderingFilter_4();
    void renderingConfirmationButton();

private slots:
    void applyFilter();
    void filterSelection();

signals:
    void filterSelected(QString filter);

};

#endif // FILTERDIALOG_H
