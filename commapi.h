#ifndef COMMAPI_H
#define COMMAPI_H

#include <QObject>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessagebox>
#include <QFileDialog>
#include <QVector>
#include <QLabel>
#include <QAxObject>
#include <QDateTime>
#include <QDebug>

typedef struct
{
    QString uploadName;
    QString uploadID;
    QString uploadType;
    QString uploadBy;
    QString uploadTime;
    QString bondedType;
    QString custCode;
    QString custPN;
    QString custFabPN;
    QString custJobID;
    QString htPN;
    QString custPO;
    QString custLotID;
    QString custWaferNo;
    QString custWaferID;
    QString custLaserMark;
    QString col_A;
    QString col_B;
    QString col_C;
    QString col_D;
    QString col_E;
    QString col_F;
    QString col_G;
    QString col_H;
    QString col_I;
    QString col_J;
    QString col_K;
    QString col_L;
    QString col_M;
    QString col_N;
    QString col_O;
    QString col_P;
    QString col_Q;
    QString col_R;
    QString col_S;
    QString col_T;
    QString col_U;

    long goodDies;
    long ngDies;
    long grossDies;
} WODATA;

class CommApi : public QObject
{
    Q_OBJECT
public:
    explicit CommApi(QObject *parent = 0);

    static bool openOracle();
    static bool openSqlServer();
    static bool openSqlServer2();
    static bool isOracleConnAlive();
    static bool isSqlserverConnAlive();
    static bool isSqlserver2ConnAlive();
    static bool isDataExisted_oracle(QString sql);
    static QString getOracleStr(QString sql);
    static int getOracleCnt(QString sql);

    static QString currYear();
    static QString currMonth();
    static QString currDay();
    static QString currWeek();
    static QString gUserName;
    static QString gUserName2;

signals:

public slots:
};

#endif // COMMAPI_H
