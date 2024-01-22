#include "mythread.h"
#include <QDebug>
#include <QSqlError>
#include <QRandomGenerator>
#include "mainwindow.h"

QMutex MyThread::_searchMutex;
bool MyThread::_resultIsFound = false;

MyThread::MyThread(QObject* parent)
    : QThread(parent)
{
    QString connectionName = "Connection_" + QString::number(QRandomGenerator::global()->generate());
    _db = QSharedPointer<QSqlDatabase>::create(QSqlDatabase::addDatabase("QSQLITE", connectionName));
    _db->setDatabaseName("Database/name.sqlite");

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
    setTask([=]() mutable
    {
        MyThread::sleep(2);

        _db->open();

        QString filterLimit = QString(" LIMIT %1 OFFSET %2").arg(limit).arg(offset);
        QString request("CREATE TEMPORARY TABLE temp_table AS SELECT ROW_NUMBER() OVER () AS №, sorted_data.* FROM "
                        "(SELECT * FROM " + nameTable + " ORDER BY " + sort + ") AS sorted_data WHERE 1=1" + filters + filterLimit);

        _query->setQuery(request, *_db);

        if (!_query->lastError().isValid())
        {
            QString zapros = "SELECT * FROM temp_table;";
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

        if (!_query->lastError().isValid())
        {
            double rowCount = _query->data(_query->index(0, 0)).toDouble();
            int maxPage = static_cast<int>(std::ceil(rowCount / rowsPerPage));
            emit returnMaxPage(maxPage);
        }

        _db->close();
    });

    start();
}

void MyThread::search(QString nameTable, QString column, QString like, QString typeSearch, QString filters, QString sort, int limit, int offset)
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
                currentPage = static_cast<int>(std::ceil(row / 10));
            }

            _db->close();
            emit searchResultFound(currentPage, _resultIsFound);
        }
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
