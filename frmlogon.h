#ifndef FRMLOGON_H
#define FRMLOGON_H

#include <QWidget>
#include "commapi.h"
#include "frmmainwindow.h"

namespace Ui {
class FrmLogon;
}

class FrmLogon : public QWidget
{
    Q_OBJECT

public:
    explicit FrmLogon(QWidget *parent = 0);
    ~FrmLogon();
    void setUI();
    void authAccount(QString username, QString password);

private slots:
    void on_logonButton_clicked();

private:
    Ui::FrmLogon *ui;
};

#endif // FRMLOGON_H
