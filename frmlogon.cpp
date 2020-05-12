#include "frmlogon.h"
#include "ui_frmlogon.h"

FrmLogon::FrmLogon(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmLogon)
{
    ui->setupUi(this);

    setUI();
}

FrmLogon::~FrmLogon()
{
    delete ui;
}

// Set the window UI
void FrmLogon::setUI()
{
    this->setFixedSize(500, 300);
    this->setWindowTitle("生产管理系统");
    this->setWindowIcon(QIcon(":/images/login.jpg"));
    this->setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
    this->setAutoFillBackground(true);

    // Set backgroud picture
    QPalette pal = palette();
    pal.setBrush(QPalette::Window,QBrush(QPixmap(tr(":/images/erp4.jpg")).scaled(size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(pal);
}

// Logon the system
void FrmLogon::on_logonButton_clicked()
{
    if (ui->userNameLineEdit->text().trimmed().isEmpty() || ui->passwordLineEdit->text().trimmed().isEmpty()) {
        QMessageBox::information(this, "登录提示", "请输入用户名和密码");
        return;
    }

    authAccount(ui->userNameLineEdit->text().trimmed(),ui->passwordLineEdit->text().trimmed());
}

// Auth the login account
void FrmLogon::authAccount(QString username, QString password)
{
    CommApi::isSqlserverConnAlive();

    QSqlDatabase db = QSqlDatabase::database("sqlserver");
    QSqlQuery query(db);
    QString sql = QString("select 用户号 from erpbase..tblOperatorData where 用户号='%1' and 密码='%2' and 状态标记 = 1").arg(username).arg(password);
    query.exec(sql);

    if (query.next()) {
        CommApi::gUserName = username;
        sql = QString("select empname from xtw.dbo.employee where empno = '%1'").arg(username);

        CommApi::isSqlserver2ConnAlive();
        QSqlDatabase db = QSqlDatabase::database("sqlserver2");
        QSqlQuery query(db);
        query.exec(sql);

        if(query.next()) {
            CommApi::gUserName2 = query.value(0).toString();
        }

        if (CommApi::gUserName == "07885") {
            CommApi::gUserName2 = "系统管理员";
        }

        FrmMainWindow *w = new FrmMainWindow;
        w->show();
        close();
    } else {
        QMessageBox::critical(this, ("登录错误"), ("请输入正确的账号和密码"));
    }
}
