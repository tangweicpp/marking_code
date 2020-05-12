#ifndef FRMMAINWINDOW_H
#define FRMMAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QTimer>
#include <QDateTime>
#include "commapi.h"
#include "frmdefault.h"
#include "frmmarkingcoderep.h"
#include "frmcustwodata.h"
#include "frmworep.h"

namespace Ui {
class FrmMainWindow;
}

class FrmMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FrmMainWindow(QWidget *parent = 0);
    ~FrmMainWindow();
    void setUI();
    void aliveDBConn();


private slots:
    void on_actionCloseCurrPage_triggered();

private:
    Ui::FrmMainWindow *ui;
    QStackedWidget *mainPage;
    FrmMarkingCodeRep *markingCodeRepPage;
    FrmWORep *woRepPage;
    FrmCustWOData *hk075Page;
};

#endif // FRMMAINWINDOW_H
