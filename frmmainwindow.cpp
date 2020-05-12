#include "frmmainwindow.h"
#include "ui_frmmainwindow.h"

FrmMainWindow::FrmMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FrmMainWindow)
{
    ui->setupUi(this);
    setUI();

    mainPage = new QStackedWidget;
    markingCodeRepPage = NULL;
    woRepPage = NULL;
    hk075Page = NULL;

    this->setCentralWidget(mainPage);

    connect(ui->actionMarkingCode, &QAction::triggered, [=]()
    {
        if(CommApi::gUserName != "07885")
        {
            QMessageBox::information(this,"提醒","请打开浏览器10.160.31.114进行维护,此功能关闭");
            return;
        }else
        {
            if (markingCodeRepPage == NULL) {
                markingCodeRepPage = new FrmMarkingCodeRep;
                mainPage->addWidget(markingCodeRepPage);
            }

            mainPage->setCurrentWidget(markingCodeRepPage);
        }
    });

    connect(ui->actionWO, &QAction::triggered, [=]()
    {
        if (woRepPage == NULL) {
            woRepPage = new FrmWORep;
            mainPage->addWidget(woRepPage);
        }

        mainPage->setCurrentWidget(woRepPage);
    });

    connect(ui->actionHk075, &QAction::triggered, [=]()
    {
        if (hk075Page == NULL) {
            hk075Page = new FrmCustWOData;
            mainPage->addWidget(hk075Page);
        }

        mainPage->setCurrentWidget(hk075Page);
    });

}

FrmMainWindow::~FrmMainWindow()
{
    delete ui;
}

// Set the mainwindow UI
void FrmMainWindow::setUI()
{
    this->setWindowTitle(QString("生产管理系统"));
    this->setWindowIcon(QIcon(":/images/ERPFINE.jpg"));
    //状态栏
    QLabel* label = new QLabel(" 当前用户: \t" + CommApi::gUserName + " " + CommApi::gUserName2 , this);
    ui->statusbar->addWidget(label);

    QLabel* currentTimeLabel = new QLabel(this);
    currentTimeLabel->setStyleSheet("color: blue");
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,&QTimer::timeout,[=]()
    {
        QDateTime time = QDateTime::currentDateTime();
        QString strCurrentTime = time.toString("yyyy-MM-dd hh:mm:ss dddd");
        currentTimeLabel->setText(strCurrentTime);
        ui->statusbar->addPermanentWidget(currentTimeLabel);
    });

    QTimer *timer2 = new QTimer(this);
    timer2->start(60000);
    connect(timer2, &QTimer::timeout, [=]()
    {
        //        qDebug() << "Alive";
        aliveDBConn();
    });
}

void FrmMainWindow::on_actionCloseCurrPage_triggered()
{
    QWidget *widget = mainPage->currentWidget();
    mainPage->removeWidget(widget);

    if (widget == markingCodeRepPage) {
        delete markingCodeRepPage;
        markingCodeRepPage = NULL;
    }

    if (widget == woRepPage) {
        delete woRepPage;
        woRepPage = NULL;
    }

    if (widget == hk075Page) {
        delete hk075Page;
        hk075Page = NULL;
    }
}

void FrmMainWindow::aliveDBConn()
{
    CommApi::isOracleConnAlive();
    CommApi::isSqlserverConnAlive();
    CommApi::isSqlserver2ConnAlive();
}




