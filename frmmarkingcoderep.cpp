#include "frmmarkingcoderep.h"
#include "ui_frmmarkingcoderep.h"

FrmMarkingCodeRep::FrmMarkingCodeRep(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmMarkingCodeRep)
{
    ui->setupUi(this);
    setUI();
    initData();
}

FrmMarkingCodeRep::~FrmMarkingCodeRep()
{
    delete ui;
}

// Init the var
void FrmMarkingCodeRep::initData()
{

}

// Set the UI
void FrmMarkingCodeRep::setUI()
{
    if (CommApi::gUserName != "07885") {
        ui->returnBackEdit->setVisible(false);
    }

    ui->splitter->setStretchFactor(1,1);
    ui->splitter_2->setStretchFactor(1,1);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setGridStyle(Qt::DashDotLine);
    ui->tableWidget->setSortingEnabled(true);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setStyleSheet("QTableView { border: none;"
                                   "selection-background-color: #8EDE21;"
                                   "background-color:rgb(231, 231, 231);"
                                   "color: blue}");
}

// Query data
void FrmMarkingCodeRep::on_queryButton_clicked()
{
    QString sql = "select HT_PN,CREATE_DATE,CREATE_BY,ESTABLISH_FLAG,BUY_OFF_FLAG,REMARK,DESCRIBE,PDS_DIR from tbl_markingcode_rep ";

    if (!ui->queryGroup->isChecked()) {
        if (!ui->partNoEdit->text().trimmed().isEmpty())
            sql.append( tr("where HT_PN = '%1' " ).arg(ui->partNoEdit->text().trimmed()) );

    } else {
        if (ui->needCreateButton->isChecked())
            sql.append("WHERE ESTABLISH_FLAG = 'N'");

        if (ui->needBuyOffButton->isChecked())
            sql.append("WHERE ESTABLISH_FLAG = 'Y' AND BUY_OFF_FLAG = 'N'");
    }

    sql.append(" order by ID desc ");
    showData(sql);
}

// Show data
void FrmMarkingCodeRep::showData(QString sql)
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);

    QStringList header;
    header << "厂内机种" << "申请时间" << "申请人" << "是否创建" << "是否验收" << "识别码" << "描述" << "PDS路径";
    ui->tableWidget->setColumnCount( header.size() );
    ui->tableWidget->setHorizontalHeaderLabels( header );
    ui->tableWidget->horizontalHeader()->setDefaultAlignment( Qt::AlignLeft | Qt::AlignVCenter );

    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");

    QSqlQuery query(db);
    query.exec(sql);

    if (!query.next()) {
        QMessageBox::warning(this,"","查询不到数据");
        return ;
    }

    query.previous();

    while (query.next()) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        for (int i = 0; i < header.size(); i++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(i).toString());
            ui->tableWidget->setItem( row, i, item);
        }
    }

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}

// Click then show data
void FrmMarkingCodeRep::on_tableWidget_clicked(const QModelIndex &index)
{
    ui->partNoEdit->setText(ui->tableWidget->item(index.row(),0)->text());
    ui->idenCodeEdit->setText(ui->tableWidget->item(index.row(),5)->text());
    ui->descEdit->setPlainText(ui->tableWidget->item(index.row(),6)->text());
    ui->pdsEdit->setText(ui->tableWidget->item(index.row(),7)->text());

    QString fileName = ui->pdsEdit->text();
    if(fileName.isEmpty()) {
        ui->pdsPicture->setVisible(false);
        return;
    }
    ui->pdsPicture->setVisible(true);
    ui->pdsEdit->setText(fileName);
    ui->pdsPicture->setPixmap(QPixmap(fileName).scaled(QSize(200,200), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

// Load and show the PDS picture
void FrmMarkingCodeRep::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"打开PDS截图文件","C:\\","PNG Files(*.png);;JPEG Files(*.jpg)");
    if(fileName.isEmpty())
        return;

    ui->pdsEdit->setText(fileName);
    ui->pdsPicture->setPixmap(QPixmap(fileName).scaled(QSize(200,200), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

// Enlarge the picture
void FrmMarkingCodeRep::on_enlargeButton_clicked()
{
    if (ui->pdsEdit->text().isEmpty())
        return;

    QDialog * dlg = new QDialog(this);
    dlg->setWindowTitle(QString("PDS工程图片"));
    dlg->setAttribute(Qt::WA_DeleteOnClose);

    QLabel  * label = new QLabel("Test", dlg);
    QPixmap pixmap(ui->pdsEdit->text());
    label->setPixmap(pixmap);
    dlg->resize(pixmap.width(),pixmap.height());
    dlg->exec();
}

// Add new rule
void FrmMarkingCodeRep::on_newButton_clicked()
{
    RULEDATA data;
    data.partNo = ui->partNoEdit->text().trimmed();
    data.idenCode = ui->idenCodeEdit->text().trimmed();
    data.pds = ui->pdsEdit->text().trimmed();
    data.desc = ui->descEdit->toPlainText().trimmed();

    // 不区分大小写
    if (!data.pds.contains(".png", Qt::CaseInsensitive) && !data.pds.contains(".jpg", Qt::CaseInsensitive)) {
        QMessageBox::warning(this, "提示", "PDS文件请上传截图文件(.jpg .png格式), 推荐使用微信截图");
        return ;
    }

    if ( data.partNo.isEmpty() || data.idenCode.isEmpty() || data.pds.isEmpty() || data.desc.isEmpty() ) {
        QMessageBox::information(this, "提示", "厂内机种名,识别码,PDS,打标码描述不可为空");
        return ;
    }

    QString sql = QString("select * from tbltsvnpiproduct where qtechptno = '%1' ").arg(data.partNo);
    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);
    query.exec(sql);

    if (!query.next()) {
        QMessageBox::warning(this, "", QString("厂内机种名:%1不存在").arg(data.partNo));
        return;
    }

    sql = QString("select HT_PN from tbl_markingcode_rep where HT_PN = '%1' ").arg(data.partNo);
    query.exec(sql);

    if (query.next()) {
        QMessageBox::warning(this, "", QString("厂内机种名:%1已申请过打标规则,不可再次新增").arg(data.partNo));
        return;
    }

    if (QMessageBox::question(this,"提示", QString("是否确定新增机种%1的打标码规则").arg(data.partNo),
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::No ) {
        return;
    }

    if (insertData(data)) {
        savePDSFile(data);
        sentInsertEml(data);
        clearCtrls();
    }
}

// Clear the window ctrls
void FrmMarkingCodeRep::clearCtrls()
{
    ui->partNoEdit->setText("");
    ui->idenCodeEdit->setText("");
    ui->descEdit->setPlainText("");
    ui->pdsEdit->setText("");
}

// Insert data
bool FrmMarkingCodeRep::insertData(RULEDATA & data)
{
    // Get max id
    QString sql = "select max(ID) + 1 from tbl_markingcode_rep";
    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);
    query.exec(sql);

    if (!query.next()) {
        return false;
    }

    long id = query.value(0).toString().toLong();

    // Insert db
    QString username = CommApi::gUserName + "|" + CommApi::gUserName2;
    sql = QString(" insert into tbl_markingcode_rep(ID,HT_PN,CREATE_DATE,CREATE_BY,ESTABLISH_FLAG,BUY_OFF_FLAG,REMARK,DESCRIBE,APPLY_FLAG) "
                  " values(%1,'%2',sysdate,'%3','N','N','%5','%6','Y')  ").arg(id).arg(data.partNo).arg(username).arg(data.idenCode).arg(data.desc);

    if (query.exec(sql)) {
        QMessageBox::information(this,"提示",QString("机种%1的打标码规则已经新增,请等待IT建立").arg(data.partNo));
        emit ui->queryButton->clicked();
    } else {
        QMessageBox::information(this,"提示","新增失败");
        return false;
    }

    return true;
}

// Save the PDS file to server
bool FrmMarkingCodeRep::savePDSFile(RULEDATA & data)
{
    // Get the path
    QString strRemoteServerDir_Win = serverDir_Win;

    strRemoteServerDir_Win.append( data.partNo );
    QString strRemoteDir = strRemoteServerDir_Win;

    QDir dir;
    if (!dir.exists(strRemoteDir))
        dir.mkpath(strRemoteDir);

    QString strLocalFileName = ui->pdsEdit->text().trimmed();

    QStringList strFileNamelist = strLocalFileName.split("/");
    QString strFileName = strFileNamelist.at(strFileNamelist.size()-1);
    QString strRemoteFileName = strRemoteDir.append("\\").append(strFileName);

    // Update the PN`s PDS path
    QString tmpPath = strRemoteFileName;
    QString path = tmpPath.replace("\\","/");
    QString sql = QString("update tbl_markingcode_rep set PDS_DIR = '%1' where ht_pn = '%2' ").arg(path).arg(data.partNo);
    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);
    query.exec(sql);

    // Save the file
    QFileInfo fi(strRemoteFileName);
    if(fi.exists())
    {
        QDateTime time = QDateTime::currentDateTime();
        QString strCurrentTime = time.toString("yyyyMMdd-hhmmss");

        strRemoteFileName = strRemoteServerDir_Win.append("\\").append(strCurrentTime).append(strFileName);
    }

    if ( QFile::copy( strLocalFileName, strRemoteFileName )) {
        data.pdsPath = strRemoteFileName;

        return true;
    } else {
        return false;
    }
}

// Sent the insert email
bool FrmMarkingCodeRep::sentInsertEml(RULEDATA & data)
{
    MAILDATA mail;

    mail.mailAttachment = data.pdsPath.replace(serverDir_Win, serverDir_Linux).replace("\\","/");
    mail.mailTitle = QString("机种%1打标码规则新增已申请, 请IT建立").arg(data.partNo);
    mail.mailBody = QString("NPI申请人: %1").arg(CommApi::gUserName + "|" + CommApi::gUserName2).append("\n\n厂内机种:\n").append(data.partNo).append("\n\n识别码:\n").append(data.idenCode).append(QString("\n\nPDS见附件:")).append("\n\n规则描述:\n").append(data.desc);
    mail.mailTo = "wei.tang_ks@ht-tech.com,yifan.zhu_ks@ht-tech.com";
    mail.mailCC = "yong.liu_ks@ht-tech.com";
    mail.mailTo.append(",").append(getSentTo(data.partNo));

    QString sql = QString("insert into tbl_Sent_Mail(MAIL_ID,SENT_FROM,SENT_TIME,SENT_TO,SENT_TO_CC,MAIL_TITLE,MAIL_BODY,MAIL_ATTACHMENT,FLAG) "
                          " values(MAILID_SEQ.Nextval,'%1',sysdate,'%2','%3','%4','%5','%6','0')").arg(CommApi::gUserName + CommApi::gUserName2).arg(mail.mailTo).arg(mail.mailCC).arg(mail.mailTitle).arg(mail.mailBody).arg(mail.mailAttachment);

    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    return query.exec(sql);
}

// Delete the rule
void FrmMarkingCodeRep::on_deleteButton_clicked()
{
    if (CommApi::gUserName != "07885") {
        QMessageBox::warning(this,"提示","你没有删除权限");
        return;
    }

    QString partNo = ui->partNoEdit->text().trimmed();
    if (partNo.isEmpty()) {
        QMessageBox::warning(this,"提示","请输入你要删除的机种名");
        return;
    }

    QString sql = QString("delete from tbl_markingcode_rep where ht_pn = '%1'").arg(partNo);
    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    if (QMessageBox::question(this, "提示", QString("是否确定删除机种%1的打标码规则").arg(partNo),
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::No ) {
        return;
    }

    query.exec(sql);
    QMessageBox::information(this, "提示", QString("成功删除机种%1打标码规则").arg(partNo));
    emit ui->queryButton->clicked();
}


// Establish confirm by IT
void FrmMarkingCodeRep::on_pushButton_clicked()
{
    RULEDATA data;
    if (CommApi::gUserName != "07885") {
        QMessageBox::warning(this,"提示","你没有确认建立权限");
        return;
    }

    QString partNo = ui->partNoEdit->text().trimmed();
    if (partNo.isEmpty()) {
        QMessageBox::warning(this,"提示","请输入你已经建立机种名");
        return;
    }

    // Sent Eml
    data.partNo = partNo;
    sentEstablishConfirmEml(data);

    // Update
    QString sql = QString("update tbl_markingcode_rep set ESTABLISH_FLAG = 'Y' where ht_pn = '%1' ").arg(partNo);
    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    if (QMessageBox::question(this, "提示", QString("是否确定建立机种%1的打标码规则").arg(partNo),
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::No ) {
        return;
    }

    query.exec(sql);
    QMessageBox::information(this, "提示", QString("成功建立机种%1打标码规则").arg(partNo));
    emit ui->queryButton->clicked();

}

// Sent the establish confirm email
bool FrmMarkingCodeRep::sentEstablishConfirmEml(RULEDATA & data)
{
    MAILDATA mail;
    mail.mailTitle = QString("机种%1打标码规则已建立, 请NPI尽快Buy off").arg(data.partNo);
    mail.mailBody = QString("机种%1打标码规则已建立, 请NPI尽快Buy off").arg(data.partNo);
    mail.mailTo = "wei.tang_ks@ht-tech.com,yifan.zhu_ks@ht-tech.com";
    mail.mailCC = "yong.liu_ks@ht-tech.com";
    mail.mailTo.append(",").append(getSentTo(data.partNo));

    QString sql = QString("insert into tbl_Sent_Mail(MAIL_ID,SENT_FROM,SENT_TIME,SENT_TO,SENT_TO_CC,MAIL_TITLE,MAIL_BODY,MAIL_ATTACHMENT,FLAG) "
                          " values(MAILID_SEQ.Nextval,'%1',sysdate,'%2','%3','%4','%5','%6','0')").arg(CommApi::gUserName).arg(mail.mailTo).arg(mail.mailCC).arg(mail.mailTitle).arg(mail.mailBody).arg(mail.mailAttachment);

    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    return query.exec(sql);
}

QString FrmMarkingCodeRep::getSentTo(QString partNo)
{
    QString username;
    QString emlname;
    QString sql = QString("select CREATE_BY from tbl_markingcode_rep where ht_pn = '%1'").arg(partNo);

    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    query.exec(sql);

    if (!query.next()) {
        return "";
    }
    username = query.value(0).toString();

    sql = QString("select EMLNAME from TBL_MAIL_NAME where instr('%1',USERID) > 0 ").arg(username);
    query.exec(sql);

    if (!query.next()) {
        return "";
    }
    emlname = query.value(0).toString();
    qDebug() << "emlname" << emlname;

    return emlname;
}

void FrmMarkingCodeRep::on_pushButton_2_clicked()
{
    QString partNo = ui->partNoEdit->text().trimmed();
    if (partNo.isEmpty()) {
        QMessageBox::warning(this,"提示","请输入你需要验收的机种名");
        return;
    }

    QString sql = QString("update tbl_markingcode_rep set BUY_OFF_FLAG = 'Y' where ht_pn = '%1' and ESTABLISH_FLAG = 'Y' and instr(CREATE_BY,'%2') > 0 ").arg(partNo).arg(CommApi::gUserName);
    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    if (QMessageBox::question(this, "提示", QString("是否确定Buyoff机种%1的打标码规则").arg(partNo),
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::No ) {
        return;
    }

    query.exec(sql);
    QMessageBox::information(this, "提示", QString("成功BuyOff机种%1打标码规则").arg(partNo));
    emit ui->queryButton->clicked();
}

void FrmMarkingCodeRep::on_returnButton_clicked()
{
    if (CommApi::gUserName != "07885") {
        QMessageBox::information(this,"没有权限","没有权限退回");
        return;
    }

    QString backReason = ui->returnBackEdit->toPlainText().trimmed();
    if (backReason.isEmpty()) {
        QMessageBox::information(this,"提示","请填写退回理由");
        return;
    }

    QString partNo = ui->partNoEdit->text().trimmed();
    if (partNo.isEmpty()) {
        QMessageBox::warning(this,"提示","请输入你需要退回的机种名");
        return;
    }

    // Sent Eml
    RULEDATA data;
    data.partNo = partNo;
    sentReturnbackEml(data);

    // Delete
    QString sql = QString("delete from tbl_markingcode_rep where ht_pn = '%1' ").arg(partNo);
    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    if (QMessageBox::question(this, "提示", QString("是否确定退回机种%1的打标码规则").arg(partNo),
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::No ) {
        return;
    }

    query.exec(sql);
    QMessageBox::information(this, "提示", QString("成功退回机种%1打标码规则").arg(partNo));

    emit ui->queryButton->clicked();


}

// Sent the returnback email
bool FrmMarkingCodeRep::sentReturnbackEml(RULEDATA & data)
{
    MAILDATA mail;
    mail.mailTitle = QString("机种%1打标码规则被退回,请查看邮件正文说明").arg(data.partNo);
    mail.mailBody = "\n\n\t" + ui->returnBackEdit->toPlainText().trimmed();
    mail.mailTo = "wei.tang_ks@ht-tech.com,yifan.zhu_ks@ht-tech.com";
    mail.mailCC = "yong.liu_ks@ht-tech.com";
    mail.mailTo.append(",").append(getSentTo(data.partNo));

    QString sql = QString("insert into tbl_Sent_Mail(MAIL_ID,SENT_FROM,SENT_TIME,SENT_TO,SENT_TO_CC,MAIL_TITLE,MAIL_BODY,MAIL_ATTACHMENT,FLAG) "
                          " values(MAILID_SEQ.Nextval,'%1',sysdate,'%2','%3','%4','%5','%6','0')").arg(CommApi::gUserName).arg(mail.mailTo).arg(mail.mailCC).arg(mail.mailTitle).arg(mail.mailBody).arg(mail.mailAttachment);

    CommApi::isOracleConnAlive();
    QSqlDatabase db = QSqlDatabase::database("oracle");
    QSqlQuery query(db);

    return query.exec(sql);
}
