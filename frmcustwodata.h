#ifndef FRMCUSTWODATA_H
#define FRMCUSTWODATA_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include <QTableWidget>
#include <QTableWidgetSelectionRange>
#include <QTableWidgetItem>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QMessageBox>
#include <QDebug>
#include "commapi.h"

namespace Ui {
class FrmCustWOData;
}

class FrmCustWOData : public QWidget
{
    Q_OBJECT

public:
    explicit FrmCustWOData(QWidget *parent = 0);
    ~FrmCustWOData();
    void initUI();
    void showData(QString strSql);

private slots:
    void on_btnQuery_clicked();
    void on_btnModify_clicked();
    void changeTest(int row, int col);
    void on_checkBox_clicked();

    void on_btnModify_2_clicked();

    void on_dateEdit_editingFinished();

private:
    Ui::FrmCustWOData *ui;
};

#endif // FRMCUSTWODATA_H
