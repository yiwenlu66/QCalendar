#include "preferencedialog.h"
#include "ui_preferencedialog.h"

PreferenceDialog::PreferenceDialog(const QLocale& currentLocale, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    if (currentLocale == QLocale::Chinese) {
        ui->comboBox->setCurrentIndex(1);
    }
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::accept()
{
    // update locale
    switch (ui->comboBox->currentIndex()) {
    case 0:
        // english
        emit changeLocale(QLocale::English);
        break;
    case 1:
        // chinese
        emit changeLocale(QLocale::Chinese);
        break;
    default:
        break;
    }
    QDialog::accept();
}
