#include "mythread.h"
#include <QDebug>
#include <QSqlError>
#include <QRandomGenerator>
#include "adminpanelwindow.h"

QMutex MyThread::_searchMutex;
bool MyThread::_resultIsFound = false;

MyThread::MyThread(QObject* parent)
    : QThread(parent)
{
    QString connectionName = "Connection_" + QString::number(QRandomGenerator::global()->generate());
    _db = QSharedPointer<QSqlDatabase>::create(QSqlDatabase::addDatabase("QSQLITE", connectionName));
    _db->setDatabaseName("Database/casino.sqlite");

    _query = QSharedPointer<QSqlQueryModel>::create();
}

MyThread::~MyThread()
{
    quit();
    wait();
}

void MyThread::setTask(std::function<void()> task)
{
    _task = std::move(task);
}

void MyThread::run()
{
    if (_task)
    {
        QMutexLocker locker(&_runThreadMutex);
        _task();
    }
}

void MyThread::completion(QSharedPointer<QSqlQueryModel> model, QString nameTable, int limit, int offset, QString filters, QString sort)
{
    setTask([=]()
    {
        MyThread::sleep(2);

        _db->open();

        QString filterLimit = QString(" LIMIT %1 OFFSET %2").arg(limit).arg(offset);
        QString request("CREATE TEMPORARY TABLE temp_" + nameTable + " AS SELECT ROW_NUMBER() OVER () AS №, sorted_data.* FROM "
                        "(SELECT * FROM " + nameTable + " ORDER BY '" + sort + "') AS sorted_data WHERE 1=1" + filters + filterLimit);

        _query->setQuery(request, *_db);

        if (!_query->lastError().isValid())
        {
            QString zapros = "SELECT * FROM temp_" + nameTable;
            model->setQuery(zapros, *_db);
            emit completedSuccessfully();
        }

        _db->close();
    });

    start();
}


void MyThread::getMaxPage(QString nameTable, int rowsPerPage, QString filters)
{
    setTask([=]()
    {
        MyThread::sleep(2);
        _db->open();

        QString request("SELECT COUNT(*) FROM " + nameTable + " WHERE 1=1 " + filters);

        _query->setQuery(request, *_db);

        _db->close();

        if (!_query->lastError().isValid())
        {
            double rowCount = _query->data(_query->index(0, 0)).toDouble();
            int maxPage = static_cast<int>(std::ceil(rowCount / rowsPerPage));
            emit returnMaxPage(maxPage);
        }
    });

    start();
}

void MyThread::search(QString nameTable, QString column, QString like, QString typeSearch, QString filters, QString sort, int limit, int offset, int rowsPerPage)
{
    setTask([=]()
    {
        if(!_resultIsFound)
        {
            _db->open();
            QString request("SELECT numbered_rows.№ FROM (SELECT ROW_NUMBER() OVER "
                            "(ORDER BY " + sort + ") AS №, * FROM " + nameTable + " WHERE 1=1 " + filters +  " LIMIT " + QString::number(limit) + " OFFSET " + QString::number(offset) + ") "
                            "AS numbered_rows WHERE numbered_rows." + column + " LIKE '" + like + typeSearch + "' LIMIT 1");

            _query->setQuery(request, *_db);

            int currentPage{};

            if(_query->rowCount() > 0 && !_resultIsFound)
            {
                QMutexLocker locker(&_searchMutex);
                _resultIsFound = true;
                double row = _query->data(_query->index(0, 0)).toDouble();
                currentPage = static_cast<int>(std::ceil(row / rowsPerPage));
            }

            _db->close();
            emit searchResultFound(currentPage, _resultIsFound);
        }
    });

    start();
}

void MyThread::getNameColumn(QString nameTable)
{
    setTask([=]()
    {
        _db->open();

        QString request = QString("SELECT name FROM pragma_table_info('%1')").arg(nameTable);

        _query->setQuery(request, *_db);

        _db->close();

        QVector<QString>* namesColumn = new QVector<QString>;

        for (int row = 0; row < _query->rowCount(); row++)
        {
            QModelIndex index = _query->index(row, 0);
            QString name = _query->data(index).toString();
            namesColumn->push_back(name);
        }

        if(!namesColumn->isEmpty())
            emit toSendNameColumng(namesColumn);
    });

    start();
}

bool MyThread::isRun()
{
    if (_runThreadMutex.tryLock())
    {
        _runThreadMutex.unlock();
        return false;
    }
    else
        return true;
}

void MyThread::request(QString request)
{
    setTask([=]()
    {
        _db->open();
        _query->setQuery(request, *_db);
        _db->close();

        if (_query->lastError().isValid())
            qDebug() << _query->lastError();
        else
            emit requestFinished();
    });

    start();
}
