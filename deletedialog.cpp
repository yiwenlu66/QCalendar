#include "deletedialog.h"
#include "ui_deletedialog.h"

DeleteDialog::DeleteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteDialog)
{
    ui->setupUi(this);
}

bool DeleteDialog::deleteSeries()
{
    return ui->radioButton_series->isChecked();
}

DeleteDialog::~DeleteDialog()
{
    delete ui;
}
