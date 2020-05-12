#ifndef FRMMARKINGCODEREP_H
#define FRMMARKINGCODEREP_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QImage>
#include <QDateTime>

#include "commapi.h"

namespace Ui {
class FrmMarkingCodeRep;
}

typedef struct
{
    QString partNo;
    QString idenCode;
    QString pds;
    QString desc;
    QString pdsPath;
} RULEDATA;

typedef struct
{
    QString mailTitle;
    QString mailBody;
    QString mailTo;
    QString mailCC;
    QString mailAttachment;
} MAILDATA;

class FrmMarkingCodeRep : public QWidget
{
    Q_OBJECT

public:
    explicit FrmMarkingCodeRep(QWidget *parent = 0);
    ~FrmMarkingCodeRep();
    void initData();
    void setUI();
    void showData(QString sql);
    bool insertData(RULEDATA & data);
    bool savePDSFile(RULEDATA & data);
    bool sentInsertEml(RULEDATA & data);
    bool sentEstablishConfirmEml(RULEDATA & data);
    bool sentReturnbackEml(RULEDATA & data);
    QString getSentTo(QString partNo);

    void clearCtrls();

private slots:
    void on_queryButton_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_openFileButton_clicked();

    void on_enlargeButton_clicked();

    void on_newButton_clicked();

    void on_deleteButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_returnButton_clicked();

private:
    Ui::FrmMarkingCodeRep *ui;
    const QString serverDir_Win = "\\\\10.160.1.84\\open\\FileServer\\PDS\\";
    const QString serverDir_Linux = "\\svn\\OpenFolder\\FileServer\\PDS\\";
};

#endif // FRMMARKINGCODEREP_H
