#include "frmdefault.h"
#include "ui_frmdefault.h"

FrmDefault::FrmDefault(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmDefault)
{
    ui->setupUi(this);
}

FrmDefault::~FrmDefault()
{
    delete ui;
}
