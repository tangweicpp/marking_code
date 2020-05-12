#include "frmlogon.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FrmLogon w;
    w.show();

    return a.exec();
}
