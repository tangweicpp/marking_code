#ifndef FRMWOREP_H
#define FRMWOREP_H

#include <QWidget>
#include "commapi.h"
#include <QAxObject>

namespace Ui {
class FrmWORep;
}

class FrmWORep : public QWidget
{
    Q_OBJECT

public:
    explicit FrmWORep(QWidget *parent = 0);
    ~FrmWORep();
    bool getData(QVector<WODATA> & v);
    bool checkData(WODATA & data, int col);
    bool saveData(QVector<WODATA> & v);
    QString getLaserMark(WODATA & data);
private slots:
    void on_pushButton_clicked();

    void on_queryButton_clicked();

    void on_newButton_2_clicked();

private:
    Ui::FrmWORep *ui;
};

#endif // FRMWOREP_H
