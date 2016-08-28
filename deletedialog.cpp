#include "deletedialog.h"
#include "ui_deletedialog.h"
#include <QPushButton>

DeleteDialog::DeleteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeleteDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

bool DeleteDialog::deleteSeries()
{
    return ui->radioButton_series->isChecked();
}

DeleteDialog::~DeleteDialog()
{
    delete ui;
}
