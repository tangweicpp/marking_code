#include "frmcustwodata.h"
#include "ui_frmcustwodata.h"

FrmCustWOData::FrmCustWOData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmCustWOData)
{
    ui->setupUi(this);
    initUI();
    connect(ui->tableWidget, SIGNAL(cellChanged(int,int)), this, SLOT(changeTest(int, int)));
}

FrmCustWOData::~FrmCustWOData()
{
    delete ui;
}

void FrmCustWOData::initUI()
{
    //隔行自动换色
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setStyleSheet("QTableView { border: none;"
                                   "selection-background-color: rgb(100,120,100);"
                                   //                                   "background-color:rgb(231, 231, 231);"
                                   "color: blue}"
                                   );
    //背景网格线设置
    //显示
    ui->tableWidget->setShowGrid(true);
    //网格背景画笔
    ui->tableWidget->setGridStyle(Qt::DashDotLine);
    //排序功能
    ui->tableWidget->setSortingEnabled(true);
}

//查询
void FrmCustWOData::on_btnQuery_clicked()
{
    QString strCustPO = ui->editCustPO->text().trimmed();
    QString strCustLot = ui->editCustLot->text().trimmed();
    QString strSql = "";

    if(strCustPO.isEmpty() && strCustLot.isEmpty())
    {
        QMessageBox::information(this,"提示",QString("PO和LOT至少输入一项,否则无法查询"));
        return;
    }

    if(!strCustPO.isEmpty())
    {
        if(!strCustLot.isEmpty())
        {
            strSql =QString(" SELECT distinct t2.PO_NUM,t2.RETICLE_LEVEL_72,t2.RETICLE_LEVEL_73,t2.mpn_desc,t2.SOURCE_BATCH_ID 客户LOT,t1.CREATEDATE,t1.remark FROM mappingdatatest t1 "
                            " INNER JOIN CUSTOMEROITBL_TEST t2 ON to_char(t2.id) = t1.FILENAME AND t2.SOURCE_BATCH_ID = t1.LOTID "
                            " WHERE t2.CUSTOMERSHORTNAME = 'HK075' AND t2.PO_NUM = '%1' AND t2.SOURCE_BATCH_ID = '%2' order by PO_NUM,RETICLE_LEVEL_72").arg(strCustPO).arg(strCustLot);
        }
        else
        {
            strSql =QString(" SELECT distinct t2.PO_NUM,t2.RETICLE_LEVEL_72,t2.RETICLE_LEVEL_73,t2.mpn_desc,t2.SOURCE_BATCH_ID 客户LOT,t1.CREATEDATE,t1.remark  FROM mappingdatatest t1 "
                            " INNER JOIN CUSTOMEROITBL_TEST t2 ON to_char(t2.id) = t1.FILENAME AND t2.SOURCE_BATCH_ID = t1.LOTID "
                            " WHERE t2.CUSTOMERSHORTNAME = 'HK075' AND t2.PO_NUM = '%1' order by PO_NUM,RETICLE_LEVEL_72").arg(strCustPO);

        }
    }
    else
    {
        strSql =QString(" SELECT distinct t2.PO_NUM,t2.RETICLE_LEVEL_72,t2.RETICLE_LEVEL_73,t2.mpn_desc,t2.SOURCE_BATCH_ID 客户LOT,t1.CREATEDATE,t1.remark  FROM mappingdatatest t1 "
                        " INNER JOIN CUSTOMEROITBL_TEST t2 ON to_char(t2.id) = t1.FILENAME AND t2.SOURCE_BATCH_ID = t1.LOTID "
                        " WHERE t2.CUSTOMERSHORTNAME = 'HK075' AND t2.SOURCE_BATCH_ID = '%1' order by PO_NUM,RETICLE_LEVEL_72").arg(strCustLot);
    }
    showData(strSql);
}

//Show Data
void FrmCustWOData::showData(QString strSql)
{
    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    QStringList header;
    QString sql = strSql;

    ui->tableWidget->clear();
    query.exec(sql);

    header << "客户PO"  << "PO Line Item" << "12NC"<< "客户机种"<< "客户LOT" << "交期" << "待发送";
    int row =0;
    int columns = 0;

    QSqlQueryModel *queryModel = new QSqlQueryModel(this);
    queryModel->setQuery(query);
    row = queryModel->rowCount();

    if(row==0)
    {
        QMessageBox::information(this,"","查询不到信息");
        return;
    }

    columns = header.size();
    ui->tableWidget->setRowCount(row);
    ui->tableWidget->setColumnCount(columns);//列

    ui->tableWidget->setHorizontalHeaderLabels(header);
    //自动调整宽度
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //表头左对齐
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

    query.exec(sql);
    for(int i = 0; query.next(); i++)
    {
        for(int j = 0; j < columns; j++)
        {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(j).toString());
            ui->tableWidget->setItem(i,j, item);
        }
    }
}

//修改
void FrmCustWOData::on_btnModify_clicked()
{
    QString strSql = "";
    QString strDate = "";
    QString strLotID = "";

    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);
    CommApi::isSqlserverConnAlive();
    QSqlDatabase db2 = QSqlDatabase::database("sqlserver");
    QSqlQuery query2(db2);

    bool checked = false;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        if(ui->tableWidget->item(i,5)->textColor() == Qt::red)
        {
            checked = true;
            strLotID = ui->tableWidget->item(i,4)->text();
            strDate = ui->tableWidget->item(i,5)->text();

            strSql = QString("update mappingdatatest set CREATEDATE = '%1', remark = 'Y' where LOTID = '%2' ").arg(strDate).arg(strLotID);
            if(!query.exec(strSql))
            {
                QMessageBox::warning(this,"提示",QString("LotID:%1未修改").arg(strLotID));
            }

            strSql = QString("update ERPBASE.dbo.tblmappingData set CREATEDATE = '%1',remark = 'Y' where LOTID = '%2'  ").arg(strDate).arg(strLotID);
            if(!query2.exec(strSql))
            {
                QMessageBox::warning(this,"提示",QString("LotID:%1未修改").arg(strLotID));
            }
        }
    }
    if(!checked)
    {
        QMessageBox::information(this,"",QString("请选择需要修改的LotID"));
    }
    else
    {
        QMessageBox::information(this,"提示",QString("交期已经修改"));
        //再次查询
        emit ui->btnQuery->clicked();
    }
}

//判断tablewidget是否checked
void FrmCustWOData::changeTest(int row, int col)
{
    if(ui->tableWidget->item(row,col)->checkState() == Qt::Checked)
    {
        qDebug() << "row:" << row << "被选中";
    }
    else
    {

    }
}

//全选 反选
void FrmCustWOData::on_checkBox_clicked()
{
    int i = 0;
    if(ui->checkBox->checkState() == Qt::Checked)
    {
        for(i = 0; i < ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->item(i,5)->setSelected(true);
        }
    }
    else
    {
        for(i = 0; i < ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->item(i,5)->setSelected(false);
        }
    }
}

//清空
void FrmCustWOData::on_btnModify_2_clicked()
{
    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    CommApi::isSqlserverConnAlive();
    QSqlDatabase db2 = QSqlDatabase::database("sqlserver");
    QSqlQuery query2(db2);


    QString strSql = "";
    QString strLotID = "";

    bool checked = false;
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if(ui->tableWidget->item(i,5)->isSelected())
        {
            checked = true;
            strLotID = ui->tableWidget->item(i,4)->text();

            strSql = QString("update mappingdatatest set CREATEDATE = '', REMARK = '' where LOTID = '%1'   ").arg(strLotID);
            if(!query.exec(strSql))
            {
                QMessageBox::warning(this,"提示",QString("LotID:%1未清空").arg(strLotID));
            }

            strSql = QString("update ERPBASE.dbo.tblmappingData set CREATEDATE = '',REMARK = '' where LOTID = '%1'  ").arg(strLotID);
            if(!query2.exec(strSql))
            {
                QMessageBox::warning(this,"提示",QString("LotID:%1未清空").arg(strLotID));
            }
        }
    }
    if(!checked)
    {
        QMessageBox::information(this,"",QString("请选择需清空的LotID"));
    }
    else
    {
        QMessageBox::information(this,"提示",QString("交期已经清空"));
        //再次查询
        emit ui->btnQuery->clicked();
    }
}



void FrmCustWOData::on_dateEdit_editingFinished()
{
    for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
        if (ui->tableWidget->item(i,5)->isSelected()) {
            ui->tableWidget->item(i,5)->setTextColor(Qt::red);
            ui->tableWidget->item(i,5)->setText(ui->dateEdit->text());
        }
    }
}

