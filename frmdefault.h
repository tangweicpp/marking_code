#ifndef FRMDEFAULT_H
#define FRMDEFAULT_H

#include <QWidget>

namespace Ui {
class FrmDefault;
}

class FrmDefault : public QWidget
{
    Q_OBJECT

public:
    explicit FrmDefault(QWidget *parent = 0);
    ~FrmDefault();

private:
    Ui::FrmDefault *ui;
};

#endif // FRMDEFAULT_H
