#include "commapi.h"

QString CommApi::gUserName = "";
QString CommApi::gUserName2 = "";

CommApi::CommApi(QObject *parent) : QObject(parent)
{

}

// Open oracle
bool CommApi::openOracle()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI","oracle");
    db.setHostName("10.160.2.19");
    db.setDatabaseName("mesora2");
    db.setPort(1521);
    db.setUserName("INSITEQT2");
    db.setPassword("KsMesDB_ht89");

    if (!db.open()) {
        QMessageBox::about(NULL,"",QString("ORACLE数据库错误：%1").arg(QSqlDatabase::database("oracle").lastError().text()));
        return false;
    }else
    {
        return true;
    }
}

// Open sql-server
bool CommApi::openSqlServer()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","sqlserver");
    QString hostName = "10.160.1.13,1433";
    QString databaseName = "erpbase";
    QString dsn = QString("DRIVER={SQL Server};SERVER=%1;DATABASE=%2").arg(hostName).arg(databaseName);
    db.setDatabaseName(dsn);
    db.setUserName("sa");
    db.setPassword("ksxtDB");

    if (!db.open()) {
        QMessageBox::about(NULL,"",QString("SQLSERVER数据库错误：%1").arg(QSqlDatabase::database("sqlserver").lastError().text()));
        return false;
    }else
    {
        return true;
    }
}

bool CommApi::openSqlServer2()
{
    if (!QSqlDatabase::database("sqlserver2").isOpen()) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QODBC","sqlserver2");
        QString hostName = "10.160.8.15,1433";
        QString databaseName = "xtw";
        QString dsn = QString("DRIVER={SQL Server};SERVER=%1;DATABASE=%2").arg(hostName).arg(databaseName);
        db.setDatabaseName(dsn);
        db.setUserName("sa");
        db.setPassword("123");

        if (!db.open()) {
            QMessageBox::about(NULL,"",QString("SQLSERVER2数据库错误：%1").arg(QSqlDatabase::database("sqlserver2").lastError().text()));
            return false;
        }else
        {
            return true;
        }
    }
    return true;
}

// Check oracle alive or not
bool CommApi::isOracleConnAlive()
{
    if (!QSqlDatabase::database("oracle").isValid()) {
        openOracle();
    }

    QString sql = "select * from v$Version";
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    query.exec(sql);

    if (!query.next()) {
        db.open();
        return false;
    }
    return true;
}

// Check sql server alive or not
bool CommApi::isSqlserverConnAlive()
{
    if (!QSqlDatabase::database("sqlserver").isValid()) {
        openSqlServer();
    }

    QString sql = "select GETDATE()";
    QSqlDatabase db = QSqlDatabase::database("sqlserver");
    QSqlQuery query(db);

    query.exec(sql);

    if (!query.next()) {
        db.open();
        return false;
    }
    return true;
}

// Check sql server alive or not
bool CommApi::isSqlserver2ConnAlive()
{
    if (!QSqlDatabase::database("sqlserver2").isValid()) {
        openSqlServer2();
    }

    QString sql = "select GETDATE()";
    QSqlDatabase db = QSqlDatabase::database("sqlserver2");
    QSqlQuery query(db);

    query.exec(sql);

    if (!query.next()) {
        db.open();
        return false;
    }
    return true;
}

// Check is the data existed
bool CommApi::isDataExisted_oracle(QString sql)
{
    isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    query.exec(sql);
    if (query.next()) {
        return true;
    }
    return false;
}

// Get oracle return value
QString CommApi::getOracleStr(QString sql)
{
    isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    query.exec(sql);
    if (query.next()) {
        return query.value(0).toString();
    }
    return "";
}

// Get oracle return count
int CommApi::getOracleCnt(QString sql)
{
    int i = 0;
    isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    query.exec(sql);
    while (query.next()) {
        i++;
    }
    return 0;
}

// Get current date
QString CommApi::currYear()
{
    return QString::number(QDate::currentDate().year());

}

QString CommApi::currMonth()
{
    return QString::number(QDate::currentDate().month());
}

QString CommApi::currDay()
{
    return QString::number(QDate::currentDate().day());
}

QString CommApi::currWeek()
{
    return QString::number(QDate::currentDate().weekNumber());
}
