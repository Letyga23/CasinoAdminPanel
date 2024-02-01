#include "existingtables.h"

ExistingTables::ExistingTables(QToolBar* toolBar) : _toolBar(toolBar)
{
    assigningValues();
    creatingObjects();

    renderingInterface();
    connects();

    _getNamesColumn->getNameColumn(_tableWorkInDB);
}

ExistingTables::~ExistingTables()
{
    delete _verticalLayout;
}

void ExistingTables::assigningValues()
{
    _currentPage = 1;
    _rowsPerPage = 10;
    _maxPageModel = 5;
    _minPageModel = 1;
    _typeSearch = "%";

    _tableWorkInDB = "ExistingTables_pred";

    _autoNumRows = false;
    _sortingOn = false;

    _typesSorting =
    {
        {0, "ASC"},
        {1, "DESC"}
    };

    _font1.setFamily("Segoe UI");
    _font1.setPointSize(14);
    _font1.setBold(true);

    _font2.setFamily("Segoe UI");
    _font2.setPointSize(12);

    _pushButtonStyleSheet = "QPushButton {\n"
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

    _comboBoxStyleSheet = "QComboBox {"
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

    _searchTimer.setSingleShot(true);
    _goToPageTimer.setSingleShot(true);
    _resizeTimer.setSingleShot(true);
}

void ExistingTables::workingWithTableView()
{
    _tableView = new QTableView(this);
    _tableView->setFont(_font2);
    _tableView->setStyleSheet("selection-background-color: rgb(42, 117, 255);");
    _tableView->setWordWrap(false);
    _verticalLayout->addWidget(_tableView);

    //Устанавливка жирного шрифта для заголовков столбцов
    QFont font = _tableView->horizontalHeader()->font();
    font.setBold(true);
    _tableView->horizontalHeader()->setFont(font);

    //Скрыть номер строк в tableView
    _tableView->verticalHeader()->setVisible(false);

    _tableView->horizontalHeader()->setStyleSheet("QHeaderView { font-size: 14pt; }");

    //Устанавка растягивания для заголовков строк и столбцов на по размеру содержимого
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Устанавка растягивания для строк и столбцов на всю высоту
    _tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //Запрет редактирования данных в ячейке
    _tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Для выделения всей строки
    _tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void ExistingTables::creatingObjects()
{
    for(int i = 0; i < 3; i++)
        _models.push_back(QSharedPointer<QSqlQueryModel>::create());

    _filterDialog = QSharedPointer<FilterDialog_ExistingTables>::create(this);

    _nextTreadModel = QSharedPointer<MyThread>::create();
    _prevTreadModel = QSharedPointer<MyThread>::create();
    _startTreadModel = QSharedPointer<MyThread>::create();
    _getMaxPageTread = QSharedPointer<MyThread>::create();
    _getNamesColumn = QSharedPointer<MyThread>::create();
}

void ExistingTables::connects()
{
    connect(_startTreadModel.get(), &MyThread::completedSuccessfully, this, &ExistingTables::startLoadModelFinished);
    connect(_nextTreadModel.get(), &MyThread::completedSuccessfully, this, &ExistingTables::threadFinished);
    connect(_prevTreadModel.get(), &MyThread::completedSuccessfully, this, &ExistingTables::threadFinished);
    connect(_getMaxPageTread.get(), &MyThread::returnMaxPage, this, &ExistingTables::setValueToMaxPage);
    connect(_getNamesColumn.get(), &MyThread::toSendNameColumng, this, &ExistingTables::setValueNameColumn);

    connect(_filterDialog.get(), &FilterDialog_ExistingTables::filterSelected, this, &ExistingTables::setFilter);

    connect(_sortingColumn, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ExistingTables::sorting);
    connect(_typeSorting, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ExistingTables::sorting);

    connect(&_searchTimer, &QTimer::timeout, this, &ExistingTables::searchInModels);

    connect(&_resizeTimer, &QTimer::timeout, this, &ExistingTables::automaticNumberRows);

    connect(&_goToPageTimer, &QTimer::timeout, this, [=]()
    {
        if(!_pageNumberToNavigate->text().isEmpty())
        {
            _like.clear();
            goToPage(_pageNumberToNavigate->text().toInt());
        }
    });

    connect(_tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, &ExistingTables::onHeaderClicked);

    connect(_addFilter, &QAction::triggered, this, &ExistingTables::openAddFilters);
    connect(_clearFilter, &QAction::triggered, this, &ExistingTables::clearFilters);
    connect(_resetTable, &QAction::triggered, this, &ExistingTables::resetTable);

    connect(_pushButton_search, &QPushButton::clicked, this, &ExistingTables::on_pushButton_search_clicked);
    connect(_clearSearch, &QPushButton::clicked, this, &ExistingTables::on_clearSearch_clicked);
    connect(_nextButton, &QPushButton::clicked, this, &ExistingTables::on_nextButton_clicked);
    connect(_prevButton, &QPushButton::clicked, this, &ExistingTables::on_prevButton_clicked);
    connect(_moreDetailed, &QPushButton::clicked, this, &ExistingTables::openMoreDetailed);

    for(QPushButton* buttonNum : _numberRows)
        connect(buttonNum, &QPushButton::clicked, this, &ExistingTables::changeNumberRows);

    connect(_searchText, &QLineEdit::textChanged, this, &ExistingTables::on_searchText_textChanged);
    connect(_pageNumberToNavigate, &QLineEdit::textChanged, this, &ExistingTables::on_pageNumberToNavigate_textChanged);

    connect(_checkBox, &QCheckBox::stateChanged, this, &ExistingTables::on_checkBox_stateChanged);
    connect(_sorting, &QCheckBox::stateChanged, this, &ExistingTables::on_sorting_stateChanged);
}

void ExistingTables::renderingInterface()
{
    _verticalLayout = new QVBoxLayout(this);

    renderingLayout_1();
    renderingLayout_2();
    workingWithTableView();
    renderingLayout_3();
    renderingLayout_4();
    renderingLayout_5();
    renderingLayout_6();

    QList<QComboBox*> comboBoxs = findChildren<QComboBox*>();
    for(QComboBox* comboBox : comboBoxs)
        comboBox->setStyleSheet(_comboBoxStyleSheet);
}

void ExistingTables::renderingLayout_1()
{
    _horizontalLayout = new QHBoxLayout();

    _labelSearch = new QLabel(this);
    _labelSearch->setFont(_font1);
    _labelSearch->setText("Поиск:");
    _horizontalLayout->addWidget(_labelSearch);

    _searchColumn = new QComboBox(this);
    _searchColumn->setFont(_font1);
    _horizontalLayout->addWidget(_searchColumn);

    _searchText = new QLineEdit(this);
    _searchText->setFont(_font1);
    _horizontalLayout->addWidget(_searchText);

    _checkBox = new QCheckBox(this);
    _checkBox->setFont(_font2);
    _checkBox->setText("Точное \nсовпадение");
    _horizontalLayout->addWidget(_checkBox);

    _pushButton_search = new QPushButton(this);
    _pushButton_search->setFont(_font2);
    _pushButton_search->setIcon(QIcon(":/assets/search.png"));
    _pushButton_search->setIconSize(QSize(32, 32));
    _pushButton_search->setStyleSheet(_pushButtonStyleSheet);
    _horizontalLayout->addWidget(_pushButton_search);

    _clearSearch = new QPushButton(this);
    _clearSearch->setFont(_font2);
    _clearSearch->setIcon(QIcon(":/assets/clearSearch.png"));
    _clearSearch->setIconSize(QSize(32, 32));
    _clearSearch->setStyleSheet(_pushButtonStyleSheet);
    _horizontalLayout->addWidget(_clearSearch);

    _horizontalSpacer_6 = new QSpacerItem(209, 20);
    _horizontalLayout->addItem(_horizontalSpacer_6);

    _addFilter = new QAction(this);
    _addFilter->setFont(_font2);
    _addFilter->setIcon(QIcon(":/assets/addFilter.png"));
    _addFilter->setVisible(false);
    _addFilter->setText("Добавить фильтр");
    _toolBar->addAction(_addFilter);

    _clearFilter = new QAction(this);
    _clearFilter->setFont(_font2);
    _clearFilter->setIcon(QIcon(":/assets/clearFilter.png"));
    _clearFilter->setIconText("Сбросить фильтр");
    _clearFilter->setVisible(false);
    _toolBar->addAction(_clearFilter);

    _verticalLayout->addLayout(_horizontalLayout);
}

void ExistingTables::renderingLayout_2()
{
    _horizontalLayout_2 = new QHBoxLayout();

    _labelSortColumn = new QLabel(this);
    _labelSortColumn->setFont(_font2);
    _labelSortColumn->setText("Сортировать столбец:");
    _horizontalLayout_2->addWidget(_labelSortColumn);

    _sortingColumn = new QComboBox(this);
    _sortingColumn->setFont(_font1);
    _horizontalLayout_2->addWidget(_sortingColumn);

    _label_6 = new QLabel(this);
    _label_6->setFont(_font2);
    _label_6->setText("по");
    _horizontalLayout_2->addWidget(_label_6);

    _typeSorting = new QComboBox(this);
    _typeSorting->addItem("Возрастанию (А-Я)");
    _typeSorting->addItem("Убыванию (Я-А)");
    _typeSorting->setFont(_font1);
    _horizontalLayout_2->addWidget(_typeSorting);

    _sorting = new QCheckBox(this);
    _sorting->setFont(_font2);
    _sorting->setText("Сортировать");
    _horizontalLayout_2->addWidget(_sorting);

    _horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding);
    _horizontalLayout_2->addItem(_horizontalSpacer);

    _moreDetailed = new QPushButton(this);
    _moreDetailed->setFont(_font1);
    _moreDetailed->setText("Подробнее");
    _moreDetailed->setStyleSheet(_pushButtonStyleSheet);
    _horizontalLayout_2->addWidget(_moreDetailed);

    _resetTable = new QAction(this);
    _resetTable->setFont(_font2);
    _resetTable->setText("Сбросить \nрезультат");
    _resetTable->setVisible(false);
    _toolBar->addAction(_resetTable);

    _verticalLayout->addLayout(_horizontalLayout_2);
}

void ExistingTables::renderingLayout_3()
{
    _horizontalLayout_3 = new QHBoxLayout();

    _horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_3->addItem(_horizontalSpacer_5);

    _labelSelectPage = new QLabel(this);
    _labelSelectPage->setFont(_font2);
    _labelSelectPage->setText("Текущая страница:");
    _horizontalLayout_3->addWidget(_labelSelectPage);

    _labelCurrentPage = new QLabel(this);
    _labelCurrentPage->setFont(_font1);
    _labelCurrentPage->setText("0");
    _horizontalLayout_3->addWidget(_labelCurrentPage);

    _label_5 = new QLabel(this);
    _label_5->setFont(_font2);
    _label_5->setText("/");
    _horizontalLayout_3->addWidget(_label_5);

    _labelMaxPage = new QLabel(this);
    _labelMaxPage->setFont(_font1);
    _labelMaxPage->setText("????");
    _horizontalLayout_3->addWidget(_labelMaxPage);

    _horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_3->addItem(_horizontalSpacer_4);

    _verticalLayout->addLayout(_horizontalLayout_3);
}

void ExistingTables::renderingLayout_4()
{
    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(14);

    _horizontalLayout_4 = new QHBoxLayout();

    _prevButton = new QPushButton(this);
    _prevButton->setFont(font);
    _prevButton->setText("<<");
    _prevButton->setStyleSheet(_pushButtonStyleSheet);
    _horizontalLayout_4->addWidget(_prevButton);

    _horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_4->addItem(_horizontalSpacer_2);

    _labelGoToPageNum = new QLabel(this);
    _labelGoToPageNum->setFont(_font2);
    _labelGoToPageNum->setText("Перейти к странице №:");
    _horizontalLayout_4->addWidget(_labelGoToPageNum);

    _pageNumberToNavigate = new QLineEdit(this);
    _pageNumberToNavigate->setFont(_font2);
    _pageNumberToNavigate->setValidator(new QIntValidator);
    _pageNumberToNavigate->setMaximumWidth(35);
    _horizontalLayout_4->addWidget(_pageNumberToNavigate);

    _horizontalSpacer_3 = new QSpacerItem(70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_4->addItem(_horizontalSpacer_3);

    _nextButton = new QPushButton(this);
    _nextButton->setFont(font);
    _nextButton->setText(">>");
    _nextButton->setStyleSheet(_pushButtonStyleSheet);
    _horizontalLayout_4->addWidget(_nextButton);

    _verticalLayout->addLayout(_horizontalLayout_4);
}

void ExistingTables::renderingLayout_5()
{
    QFont font;
    font.setFamily("Segoe UI");
    font.setPointSize(14);

    _horizontalLayout_5 = new QHBoxLayout();

    _horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_5->addItem(_horizontalSpacer_7);

    int tact = 5;
    for(int num = 10; num <= 20; num += tact)
    {
        QPushButton* numberRows = new QPushButton(this);
        numberRows->setFont(font);
        numberRows->setText(QString::number(num));
        numberRows->setObjectName("_numberRows_" + QString::number(num));
        _horizontalLayout_5->addWidget(numberRows);
        _numberRows.push_back(numberRows);
    }

    _automaticNumberRows = new QPushButton(this);
    _automaticNumberRows->setFont(font);
    _automaticNumberRows->setText("Авто");
    _automaticNumberRows->setObjectName("_automaticNumberRows");
    _horizontalLayout_5->addWidget(_automaticNumberRows);
    _numberRows.push_back(_automaticNumberRows);

    _numberRows[0]->setStyleSheet(_pushButtonStyleSheet);

    _horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_5->addItem(_horizontalSpacer_8);

    _verticalLayout->addLayout(_horizontalLayout_5);
}

void ExistingTables::renderingLayout_6()
{
    _horizontalLayout_6 = new QHBoxLayout();

    _status = new QLabel();
    _horizontalLayout_6->addWidget(_status);

    _horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    _horizontalLayout_6->addItem(_horizontalSpacer_9);

    _verticalLayout->addLayout(_horizontalLayout_6);
}


void ExistingTables::searchInDB()
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

void ExistingTables::initializationStartModel()
{
    _status->setText("Идёт загрузка данных...");
    _tableView->setModel(nullptr);

    int setPages = _currentPage - currentPageInModel();

    int startOffset = setPages * _rowsPerPage;
    int nextOffset = (setPages + _maxPageModel) * _rowsPerPage;
    int prevOffset = (setPages - _maxPageModel) * _rowsPerPage;

    loadingModel(_startTreadModel, _models[0], startOffset);
    loadingModel(_nextTreadModel, _models[1], nextOffset);
    loadingModel(_prevTreadModel, _models[2], prevOffset);
}

void ExistingTables::loadingModel(QSharedPointer<MyThread> thread, QSharedPointer<QSqlQueryModel> model, int offset)
{
    thread->completion(std::ref(model), _tableWorkInDB, _rowsPerPage * _maxPageModel, offset, _filter, _columtSort, _typeSort);
}

void ExistingTables::startLoadModelFinished()
{
    blockingInterface(true);
    _status->clear();
    setModel(_models[0]);

    if(!_like.isEmpty())
        searchInModels();
}

void ExistingTables::threadFinished()
{
    _nextButton->setEnabled(true);
    _prevButton->setEnabled(true);
}

void ExistingTables::on_clearSearch_clicked()
{
    _searchText->clear();
}

void ExistingTables::updateTablePage()
{
    updateCurrentPageInLabel();

    int startIndex = (currentPageInModel() - 1) * _rowsPerPage;
    int endIndex = startIndex + _rowsPerPage;

    int rowCountModel = _tableView->model()->rowCount();
    for (int row = 0; row < rowCountModel; row++)
    {
        bool rowVisible = (row >= startIndex && row < endIndex);
        _tableView->setRowHidden(row, !rowVisible);
    }
}

void ExistingTables::updateCurrentPageInLabel()
{
    _labelCurrentPage->setText(QString::number(_currentPage));
}

void ExistingTables::on_pageNumberToNavigate_textChanged()
{
    if(_pageNumberToNavigate->text() == "0")
        _pageNumberToNavigate->clear();

    _goToPageTimer.start(1000);
}

void ExistingTables::goToPage(int currentPage)
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

void ExistingTables::on_prevButton_clicked()
{
    if(_currentPage > 1)
    {
        _nextButton->setEnabled(true);
        if(currentPageInModel() == _minPageModel)
        {
            if (!_prevTreadModel->isRunning())
            {
                if(_models[2]->rowCount() != 0)
                    goToPrevModel();
            }
            else
                _prevButton->setEnabled(false);
        }
        else
        {
            _currentPage--;
            updateTablePage();
        }
    }
}

void ExistingTables::on_nextButton_clicked()
{
    if(_currentPage < _maxPage)
    {
        _prevButton->setEnabled(true);
        if(currentPageInModel() == _maxPageModel)
        {
            if(!_nextTreadModel->isRun())
            {
                if(_models[1]->rowCount() != 0)
                    goToNextModel();
            }
            else
                _nextButton->setEnabled(false);
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

void ExistingTables::setModel(QSharedPointer<QSqlQueryModel> model)
{
    if(model->rowCount() == 0)
    {
        QMessageBox::warning(this, "Внимание", "Данных нет!", QMessageBox::Ok);
        return;
    }

    _tableView->setModel(model.data());

    _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);

    for (int col = 1; col < model->columnCount(); ++col)
    {
        QString originalHeaderText = model->headerData(col, Qt::Horizontal).toString();
        QString wrappedHeaderText = originalHeaderText.replace(" ", "\n");
        model->setHeaderData(col, Qt::Horizontal, wrappedHeaderText);
    }

    updateTablePage();
}


void ExistingTables::goToNextModel()
{
    _currentPage++;

    setModel(_models[1]);
    std::rotate(_models.begin(), _models.begin() + 1, _models.end());

    int nextOffset = (_currentPage + _maxPageModel - 1) * _rowsPerPage;
    loadingModel(_nextTreadModel, _models[1], nextOffset);
}

void ExistingTables::goToPrevModel()
{
    _currentPage--;

    setModel(_models[2]);
    std::rotate(_models.begin(), _models.begin() + (_models.size() - 1), _models.end());

    int prevOffset = (_currentPage - _maxPageModel * 2) * _rowsPerPage;
    loadingModel(_prevTreadModel, _models[2], prevOffset);
}

void ExistingTables::blockingInterface(bool flag)
{
    QList<QPushButton*> pushbuttons = this->findChildren<QPushButton*>();
    for(QPushButton* pushbutton : pushbuttons)
        pushbutton->setEnabled(flag);

    QList<QComboBox*> comboBoxs = this->findChildren<QComboBox*>();
    for(QComboBox* comboBox : comboBoxs)
        comboBox->setEnabled(flag);

    _sorting->setEnabled(flag);
    _pageNumberToNavigate->setEnabled(flag);
    _searchText->setEnabled(flag);
}

void ExistingTables::refreshStartModel()
{    
    _columtSort = (_sortingOn) ? _sortingColumn->currentText() : "";
    _typeSort = _typesSorting[_typeSorting->currentIndex()];
    _like.clear();

    blockingInterface(false);

    _labelMaxPage->setText("????");
    _labelCurrentPage->setText("1");

    _pageNumberToNavigate->clear();
    _currentPage = 1;

    _getMaxPageTread->getMaxPage(_tableWorkInDB, _rowsPerPage, _filter);
    initializationStartModel();
}

void ExistingTables::setFilter(const QString &filter)
{
    _filter = filter;
}

void ExistingTables::openAddFilters()
{
    if (_filterDialog->exec() == QDialog::Accepted)
    {
        if(!_filter.isEmpty())
            refreshStartModel();
    }
}

void ExistingTables::clearFilters()
{
    if(!_filter.isEmpty())
    {
        _filter.clear();
        _filterDialog->clearFilter();
        refreshStartModel();
    }
}

int ExistingTables::currentPageInModel()
{
    int pageModel = _currentPage % _maxPageModel;

    if(pageModel == 0)
        pageModel = _maxPageModel;

    return pageModel;
}

void ExistingTables::searchInModels()
{
    if(_searchText->text().isEmpty())
        return;

    bool resultSearchInModel = false;
    _like = _searchText->text();
    _column = _searchColumn->currentText();

    for (QSharedPointer<QSqlQueryModel> model : _models)
    {
        for (int row = 0; row < model->rowCount(); row++)
        {
            QModelIndex index = model->index(row, _searchColumn->currentIndex() + 1);
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
                _tableView->setCurrentIndex(index);
                return;
            }
        }
    }

    searchInDB();
}

void ExistingTables::on_searchText_textChanged()
{
    _searchTimer.start(1000);
}

void ExistingTables::on_comboBox_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    if(!_like.isEmpty())
        searchInModels();
}

void ExistingTables::blockAndOperate(QObject* widget, const std::function<void()>& operation)
{
    widget->blockSignals(true);
    operation();
    widget->blockSignals(false);
}


void ExistingTables::resetTable()
{
    blockAndOperate(_searchText, [&]() { _searchText->clear(); });
    blockAndOperate(_sortingColumn, [&]() { _sortingColumn->setCurrentIndex(0); });
    blockAndOperate(_typeSorting, [&]() { _typeSorting->setCurrentIndex(0); });

    _filterDialog->clearFilter();

    refreshStartModel();
}

void ExistingTables::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 2)
        _typeSearch.clear();
    else if(arg1 == 0)
        _typeSearch = '%';
}

void ExistingTables::on_pushButton_search_clicked()
{
    if(!_searchText->text().isEmpty())
        searchInModels();
}

void ExistingTables::onHeaderClicked(int logicalIndex)
{
    if(!_sortingOn)
        return;

    QString headerText = _tableView->model()->headerData(logicalIndex, Qt::Horizontal).toString();
    headerText = headerText.replace("\n", " ");

    if (_sortingColumn->currentText() == headerText)
    {
        int currentSortIndex = _typeSorting->currentIndex();
        currentSortIndex = (currentSortIndex + 1) % _typeSorting->count();
        _typeSorting->setCurrentIndex(currentSortIndex);
    }
    else
        settingValueInComboBox(_sortingColumn, headerText);

    if(_searchColumn->currentText() != headerText)
        settingValueInComboBox(_searchColumn, headerText);
}

void ExistingTables::settingValueInComboBox(QComboBox* comboBox, QString& headerText)
{
    int comboBoxIndex = comboBox->findText(headerText);
    if (comboBoxIndex != -1)
        comboBox->setCurrentIndex(comboBoxIndex);
}

void ExistingTables::setValueToMaxPage(int maxPage)
{
    QMutexLocker locker(&_mutex);
    _maxPage = maxPage;
    _labelMaxPage->setText(QString::number(_maxPage));
}

void ExistingTables::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    _resizeTimer.start(500);
}

void ExistingTables::automaticNumberRows()
{
    if(!_autoNumRows)
        return;

    if(_tableView->model())
    {
        int visibleHeight = _tableView->viewport()->height();
        int rowHeight = _tableView->verticalHeader()->defaultSectionSize();

        _rowsPerPage = visibleHeight / rowHeight;
        refreshStartModel();
    }
}

void ExistingTables::changeNumberRows()
{
    QPushButton* button = (QPushButton*)sender();

    for(QPushButton* buttonNum : _numberRows)
        buttonNum->setStyleSheet("");

    button->setStyleSheet(_pushButtonStyleSheet);

    if(button->objectName() == "_automaticNumberRows")
    {
        _autoNumRows = true;
        automaticNumberRows();
        return;
    }
    else
        _autoNumRows = false;

    QStringList nums = button->objectName().split('_');

    if (nums.size() >= 3)
        QString num = nums[2];
    else
    {
        qDebug() << "Размер листа " + QString::number(nums.size());
        return;
    }

    int num = nums[2].toInt();

    if(_rowsPerPage == num)
        return;

    _rowsPerPage = num;

    refreshStartModel();
}

void ExistingTables::setValueNameColumn(QVector<QString>* namesColumn)
{
    for(QString nameColumn : *namesColumn)
    {
        _sortingColumn->blockSignals(true);
        _sortingColumn->addItem(nameColumn);
        _sortingColumn->blockSignals(false);

        _searchColumn->blockSignals(true);
        _searchColumn->addItem(nameColumn);
        _searchColumn->blockSignals(false);
    }

    if(!namesColumn->isEmpty())
    {
        delete namesColumn;
        refreshStartModel();
    }
}

void ExistingTables::on_sorting_stateChanged(int arg1)
{
    _sortingOn = (arg1 == 2) ? true : false;

    refreshStartModel();
}

void ExistingTables::sorting()
{
    if(!_sortingOn)
        return;

    refreshStartModel();
}

void ExistingTables::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    QList<QAction*> actions = this->findChildren<QAction*>();
    for(QAction* action : actions)
        action->setVisible(true);
}

void ExistingTables::hideEvent(QHideEvent* event)
{
    QWidget::hideEvent(event);

    QList<QAction*> actions = this->findChildren<QAction*>();
    for(QAction* action : actions)
        action->setVisible(false);
}

void ExistingTables::openMoreDetailed()
{

}
