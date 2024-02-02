#include "loanapplications.h"
#include "ui_loanapplications.h"

LoanApplications::LoanApplications(QToolBar* toolBar, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoanApplications),
    _toolBar(toolBar)
{
    ui->setupUi(this);

    assigningValues();
    creatingObjects();
    rendoringToolBar();
    workingWithTableView();

    connects();

    _getNamesColumn->getNameColumn(_tableWorkInDB);
}

LoanApplications::~LoanApplications()
{
    delete ui;
}

void LoanApplications::assigningValues()
{
    _currentPage = 1;
    _rowsPerPage = 10;
    _maxPageModel = 5;
    _minPageModel = 1;
    _typeSearch = "%";

    _tableWorkInDB = "name_pred";

    _autoNumRows = false;
    _sortingOn = false;

    _typesSorting =
    {
        {0, "ASC"},
        {1, "DESC"}
    };

    _pushButtonStyleSheet = "QPushButton {"
                            "	background-color: #3498db;"
                            "	border: 1px solid #2980b9;"
                            "	color: #ffffff;"
                            "	padding: 5px 10px;"
                            "	border-radius: 3px;}"

                            "QPushButton:hover {background-color: #2184cb;border: 1px solid #1c6da5;}"

                            "QPushButton:pressed {"
                            "	background-color: #1a548b;"
                            "	border: 1px solid #174172;}"

                            "QPushButton:disabled {"
                            "	background-color: #d3d3d3;"
                            "	color: #555555;"
                            "	border: 1px solid #a3a3a3;}";

    _searchTimer.setSingleShot(true);
    _goToPageTimer.setSingleShot(true);
    _resizeTimer.setSingleShot(true);

    _numberRows.push_back(ui->numberRows_10);
    _numberRows.push_back(ui->numberRows_15);
    _numberRows.push_back(ui->numberRows_20);
    _numberRows.push_back(ui->automaticNumberRows);
}

void LoanApplications::workingWithTableView()
{
    ui->tableView->setStyleSheet("selection-background-color: rgb(42, 117, 255);");
    ui->tableView->setWordWrap(false);

    //Устанавливка жирного шрифта для заголовков столбцов
    QFont font = ui->tableView->horizontalHeader()->font();
    font.setBold(true);
    ui->tableView->horizontalHeader()->setFont(font);

    //Скрыть номер строк в tableView
    ui->tableView->verticalHeader()->setVisible(false);

    ui->tableView->horizontalHeader()->setStyleSheet("QHeaderView { font-size: 14pt; }");

    //Устанавка растягивания для заголовков строк и столбцов на по размеру содержимого
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Устанавка растягивания для строк и столбцов на всю высоту
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Запрет редактирования данных в ячейке
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Для выделения всей строки
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void LoanApplications::rendoringToolBar()
{
    _addFilter = new QAction(this);
    _addFilter->setIcon(QIcon(":/assets/addFilter.png"));
    _addFilter->setVisible(false);
    _addFilter->setText("Добавить фильтр");
    _toolBar->addAction(_addFilter);

    _clearFilter = new QAction(this);
    _clearFilter->setIcon(QIcon(":/assets/clearFilter.png"));
    _clearFilter->setIconText("Сбросить фильтр");
    _clearFilter->setVisible(false);
    _toolBar->addAction(_clearFilter);

    _resetTable = new QAction(this);
    _resetTable->setText("Сбросить \nрезультат");
    _resetTable->setVisible(false);
    _toolBar->addAction(_resetTable);
}

void LoanApplications::creatingObjects()
{
    for(int i = 0; i < 3; i++)
        _models.push_back(QSharedPointer<QSqlQueryModel>::create());

    _filterDialog = QSharedPointer<FilterDialog_LoanApplications>::create(this);

    _nextTreadModel = QSharedPointer<MyThread>::create();
    _prevTreadModel = QSharedPointer<MyThread>::create();
    _startTreadModel = QSharedPointer<MyThread>::create();
    _getMaxPageTread = QSharedPointer<MyThread>::create();
    _getNamesColumn = QSharedPointer<MyThread>::create();
}

void LoanApplications::connects()
{
    connect(_startTreadModel.get(), &MyThread::completedSuccessfully, this, &LoanApplications::startLoadModelFinished);
    connect(_nextTreadModel.get(), &MyThread::completedSuccessfully, this, &LoanApplications::threadFinished);
    connect(_prevTreadModel.get(), &MyThread::completedSuccessfully, this, &LoanApplications::threadFinished);
    connect(_getMaxPageTread.get(), &MyThread::returnMaxPage, this, &LoanApplications::setValueToMaxPage);
    connect(_getNamesColumn.get(), &MyThread::toSendNameColumng, this, &LoanApplications::setValueNameColumn);

    connect(_filterDialog.get(), &FilterDialog_LoanApplications::filterSelected, this, &LoanApplications::setFilter);

    connect(ui->sortingColumn, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoanApplications::sorting);
    connect(ui->typeSorting, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LoanApplications::sorting);

    connect(&_searchTimer, &QTimer::timeout, this, &LoanApplications::searchInModels);

    connect(&_resizeTimer, &QTimer::timeout, this, &LoanApplications::automaticNumberRows);

    connect(&_goToPageTimer, &QTimer::timeout, this, [=]()
    {
        if(!ui->pageNumberToNavigate->text().isEmpty())
        {
            _like.clear();
            goToPage(ui->pageNumberToNavigate->text().toInt());
        }
    });

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &LoanApplications::onHeaderClicked);

    connect(_addFilter, &QAction::triggered, this, &LoanApplications::openAddFilters);
    connect(_clearFilter, &QAction::triggered, this, &LoanApplications::clearFilters);
    connect(_resetTable, &QAction::triggered, this, &LoanApplications::resetTable);

    for(QPushButton* buttonNum : _numberRows)
        connect(buttonNum, &QPushButton::clicked, this, &LoanApplications::changeNumberRows);

    connect(ui->searchText, &QLineEdit::textChanged, this, &LoanApplications::on_searchText_textChanged);
    connect(ui->pageNumberToNavigate, &QLineEdit::textChanged, this, &LoanApplications::on_pageNumberToNavigate_textChanged);
}

void LoanApplications::searchInDB()
{
    int totalRowCount = _maxPage * _rowsPerPage;
    int limit = 2000;
    int numThreads = static_cast<int>(std::ceil(static_cast<double>(totalRowCount) / limit));

    MyThread::_resultIsFound = false;

    for (int i = 0; i < numThreads; i++)
    {
        int offset = i * limit;

        QSharedPointer<MyThread> searchTask = QSharedPointer<MyThread>::create();
        connect(searchTask.get(), &MyThread::searchResultFound, this, [=](int currentPage, bool result)
        {
            if(result)
                goToPage(currentPage);
            else if (i == (numThreads - 1))
                QMessageBox::warning(this, "Внимание", "Данных нет!", QMessageBox::Ok);
        });
        searchTask->search(_tableWorkInDB, _column, _like, _typeSearch, _filter, _columtSort, _typeSort, limit, offset, _rowsPerPage);
    }
}

void LoanApplications::initializationStartModel()
{
    ui->status->setText("Идёт загрузка данных...");
    ui->tableView->setModel(nullptr);

    int setPages = _currentPage - currentPageInModel();

    int startOffset = setPages * _rowsPerPage;
    int nextOffset = (setPages + _maxPageModel) * _rowsPerPage;
    int prevOffset = (setPages - _maxPageModel) * _rowsPerPage;

    loadingModel(_startTreadModel, _models[0], startOffset);
    loadingModel(_nextTreadModel, _models[1], nextOffset);
    loadingModel(_prevTreadModel, _models[2], prevOffset);
}

void LoanApplications::loadingModel(QSharedPointer<MyThread> thread, QSharedPointer<QSqlQueryModel> model, int offset)
{
    thread->completion(std::ref(model), _tableWorkInDB, _rowsPerPage * _maxPageModel, offset, _filter, _columtSort, _typeSort);
}

void LoanApplications::startLoadModelFinished()
{
    blockingInterface(true);
    ui->status->clear();
    setModel(_models[0]);

    if(!_like.isEmpty())
        searchInModels();
}

void LoanApplications::threadFinished()
{
    ui->nextButton->setEnabled(true);
    ui->prevButton->setEnabled(true);
}

void LoanApplications::on_clearSearch_clicked()
{
    ui->searchText->clear();
}

void LoanApplications::updateTablePage()
{
    updateCurrentPageInLabel();

    int startIndex = (currentPageInModel() - 1) * _rowsPerPage;
    int endIndex = startIndex + _rowsPerPage;

    int rowCountModel = ui->tableView->model()->rowCount();
    for (int row = 0; row < rowCountModel; row++)
    {
        bool rowVisible = (row >= startIndex && row < endIndex);
        ui->tableView->setRowHidden(row, !rowVisible);
    }
}

void LoanApplications::updateCurrentPageInLabel()
{
    ui->labelCurrentPage->setText(QString::number(_currentPage));
}


void LoanApplications::goToPage(int currentPage)
{
    int setPages = _currentPage - currentPageInModel();

    _currentPage = (currentPage > _maxPage) ? _maxPage : currentPage;

    if(setPages < _currentPage && _currentPage <= (setPages + _maxPageModel))
        updateTablePage();
    else
    {
        blockingInterface(false);
        initializationStartModel();
    }
}

void LoanApplications::on_prevButton_clicked()
{
    if(_currentPage > 1)
    {
        ui->nextButton->setEnabled(true);
        if(currentPageInModel() == _minPageModel)
        {
            if (!_prevTreadModel->isRunning())
            {
                if(_models[2]->rowCount() != 0)
                    goToPrevModel();
            }
            else
                ui->prevButton->setEnabled(false);
        }
        else
        {
            _currentPage--;
            updateTablePage();
        }
    }
}

void LoanApplications::on_nextButton_clicked()
{
    if(_currentPage < _maxPage)
    {
        ui->prevButton->setEnabled(true);
        if(currentPageInModel() == _maxPageModel)
        {
            if(!_nextTreadModel->isRun())
            {
                if(_models[1]->rowCount() != 0)
                    goToNextModel();
            }
            else
                ui->nextButton->setEnabled(false);
        }
        else
        {
            _currentPage++;
            updateTablePage();
        }
    }
    else
        QMessageBox::warning(this, "Внимание", "Данных больше нет!", QMessageBox::Ok);
}

void LoanApplications::setModel(QSharedPointer<QSqlQueryModel> model)
{
    if(model->rowCount() == 0)
    {
        QMessageBox::warning(this, "Внимание", "Данных нет!", QMessageBox::Ok);
        return;
    }

    ui->tableView->setModel(model.data());

    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    for (int col = 1; col < model->columnCount(); ++col)
    {
        QString originalHeaderText = model->headerData(col, Qt::Horizontal).toString();
        QString wrappedHeaderText = originalHeaderText.replace(" ", "\n");
        model->setHeaderData(col, Qt::Horizontal, wrappedHeaderText);
    }

    updateTablePage();
}


void LoanApplications::goToNextModel()
{
    _currentPage++;

    setModel(_models[1]);
    std::rotate(_models.begin(), _models.begin() + 1, _models.end());

    int nextOffset = (_currentPage + _maxPageModel - 1) * _rowsPerPage;
    loadingModel(_nextTreadModel, _models[1], nextOffset);
}

void LoanApplications::goToPrevModel()
{
    _currentPage--;

    setModel(_models[2]);
    std::rotate(_models.begin(), _models.begin() + (_models.size() - 1), _models.end());

    int prevOffset = (_currentPage - _maxPageModel * 2) * _rowsPerPage;
    loadingModel(_prevTreadModel, _models[2], prevOffset);
}

void LoanApplications::blockingInterface(bool flag)
{
    QList<QPushButton*> pushbuttons = this->findChildren<QPushButton*>();
    for(QPushButton* pushbutton : pushbuttons)
        pushbutton->setEnabled(flag);

    QList<QComboBox*> comboBoxs = this->findChildren<QComboBox*>();
    for(QComboBox* comboBox : comboBoxs)
        comboBox->setEnabled(flag);

    ui->sorting->setEnabled(flag);
    ui->pageNumberToNavigate->setEnabled(flag);
    ui->searchText->setEnabled(flag);
}

void LoanApplications::refreshStartModel()
{
    _columtSort = (_sortingOn) ? ui->sortingColumn->currentText() : "";
    _typeSort = _typesSorting[ui->typeSorting->currentIndex()];
    _like.clear();

    blockingInterface(false);

    ui->labelMaxPage->setText("????");
    ui->labelCurrentPage->setText("1");

    blockAndOperate(ui->pageNumberToNavigate, [&]() { ui->pageNumberToNavigate->clear(); });

    _currentPage = 1;

    _getMaxPageTread->getMaxPage(_tableWorkInDB, _rowsPerPage, _filter);
    initializationStartModel();
}

void LoanApplications::setFilter(const QString &filter)
{
    _filter = filter;
}

void LoanApplications::openAddFilters()
{
    if (_filterDialog->exec() == QDialog::Accepted)
    {
        if(!_filter.isEmpty())
            refreshStartModel();
    }
}

void LoanApplications::clearFilters()
{
    if(!_filter.isEmpty())
    {
        _filter.clear();
        _filterDialog->clearFilter();
        refreshStartModel();
    }
}

int LoanApplications::currentPageInModel()
{
    int pageModel = _currentPage % _maxPageModel;

    if(pageModel == 0)
        pageModel = _maxPageModel;

    return pageModel;
}

void LoanApplications::searchInModels()
{
    if(ui->searchText->text().isEmpty())
        return;

    bool resultSearchInModel = false;
    _like = ui->searchText->text();
    _column = ui->searchColumn->currentText();

    for (QSharedPointer<QSqlQueryModel> model : _models)
    {
        for (int row = 0; row < model->rowCount(); row++)
        {
            QModelIndex index = model->index(row, ui->searchColumn->currentIndex() + 1);
            QVariant data = model->data(index);

            if (_typeSearch == "%")
                resultSearchInModel = data.toString().startsWith(_like, Qt::CaseInsensitive);
            else
                resultSearchInModel = (data.toString().compare(_like, Qt::CaseInsensitive) == 0);

            if (resultSearchInModel)
            {
                double resultRow = model->data(model->index(row, 0)).toDouble();
                _currentPage = std::ceil(resultRow / _rowsPerPage);
                setModel(model);
                ui->tableView->setCurrentIndex(index);
                return;
            }
        }
    }

    searchInDB();
}

void LoanApplications::blockAndOperate(QObject* widget, const std::function<void()>& operation)
{
    widget->blockSignals(true);
    operation();
    widget->blockSignals(false);
}


void LoanApplications::resetTable()
{
    blockAndOperate(ui->searchText, [&]() { ui->searchText->clear(); });
    blockAndOperate(ui->sortingColumn, [&]() { ui->sortingColumn->setCurrentIndex(0); });
    blockAndOperate(ui->typeSorting, [&]() { ui->typeSorting->setCurrentIndex(0); });

    _filterDialog->clearFilter();

    refreshStartModel();
}

void LoanApplications::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 2)
        _typeSearch.clear();
    else if(arg1 == 0)
        _typeSearch = '%';
}

void LoanApplications::on_pushButton_search_clicked()
{
    if(!ui->searchText->text().isEmpty())
        searchInModels();
}

void LoanApplications::onHeaderClicked(int logicalIndex)
{
    if(!_sortingOn)
        return;

    QString headerText = ui->tableView->model()->headerData(logicalIndex, Qt::Horizontal).toString();
    headerText = headerText.replace("\n", " ");

    if (ui->sortingColumn->currentText() == headerText)
    {
        int currentSortIndex = ui->typeSorting->currentIndex();
        currentSortIndex = (currentSortIndex + 1) % ui->typeSorting->count();
        ui->typeSorting->setCurrentIndex(currentSortIndex);
    }
    else
        settingValueInComboBox(ui->sortingColumn, headerText);

    if(ui->searchColumn->currentText() != headerText)
        settingValueInComboBox(ui->searchColumn, headerText);
}

void LoanApplications::settingValueInComboBox(QComboBox* comboBox, QString& headerText)
{
    int comboBoxIndex = comboBox->findText(headerText);
    if (comboBoxIndex != -1)
        comboBox->setCurrentIndex(comboBoxIndex);
}

void LoanApplications::setValueToMaxPage(int maxPage)
{
    QMutexLocker locker(&_mutex);
    _maxPage = maxPage;
    ui->labelMaxPage->setText(QString::number(_maxPage));
}

void LoanApplications::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    _resizeTimer.start(500);
}

void LoanApplications::automaticNumberRows()
{
    if(!_autoNumRows)
        return;

    if(ui->tableView->model())
    {
        int visibleHeight = ui->tableView->viewport()->height();
        int rowHeight = ui->tableView->verticalHeader()->defaultSectionSize();

        if(visibleHeight / rowHeight != 0)
        {
            _rowsPerPage = visibleHeight / rowHeight;
            refreshStartModel();
        }
        else
            QMessageBox::warning(this, "Внимание", "Слишком маленькая высота", QMessageBox::Ok);
    }
}

void LoanApplications::changeNumberRows()
{
    QPushButton* button = (QPushButton*)sender();

    for(QPushButton* buttonNum : _numberRows)
        buttonNum->setStyleSheet("");

    button->setStyleSheet(_pushButtonStyleSheet);

    if(button->objectName() == "automaticNumberRows")
    {
        _autoNumRows = true;
        automaticNumberRows();
        return;
    }
    else
        _autoNumRows = false;

    QStringList nums = button->objectName().split('_');

    if (nums.size() >= 2)
        QString num = nums[1];
    else
    {
        qDebug() << "Размер листа " + QString::number(nums.size());
        return;
    }

    int num = nums[1].toInt();

    if(_rowsPerPage == num)
        return;

    _rowsPerPage = num;

    refreshStartModel();
}

void LoanApplications::setValueNameColumn(QVector<QString>* namesColumn)
{
    for(QString nameColumn : *namesColumn)
    {
        ui->sortingColumn->blockSignals(true);
        ui->sortingColumn->addItem(nameColumn);
        ui->sortingColumn->blockSignals(false);

        ui->searchColumn->blockSignals(true);
        ui->searchColumn->addItem(nameColumn);
        ui->searchColumn->blockSignals(false);
    }

    if(!namesColumn->isEmpty())
    {
        delete namesColumn;
        refreshStartModel();
    }
}

void LoanApplications::on_sorting_stateChanged(int arg1)
{
    _sortingOn = (arg1 == 2) ? true : false;

    refreshStartModel();
}

void LoanApplications::sorting()
{
    if(!_sortingOn)
        return;

    refreshStartModel();
}

void LoanApplications::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    QList<QAction*> actions = this->findChildren<QAction*>();
    for(QAction* action : actions)
        action->setVisible(true);
}

void LoanApplications::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);

    QList<QAction*> actions = this->findChildren<QAction*>();
    for(QAction* action : actions)
        action->setVisible(false);
}

void LoanApplications::on_searchColumn_currentIndexChanged(int index)
{
    Q_UNUSED(index);

    if(!_like.isEmpty())
        searchInModels();
}

void LoanApplications::on_searchText_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    _searchTimer.start(1000);
}

void LoanApplications::on_pageNumberToNavigate_textChanged(const QString &arg1)
{
    if(arg1 == "0")
        ui->pageNumberToNavigate->clear();

    _goToPageTimer.start(1000);
}
