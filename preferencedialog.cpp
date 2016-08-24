#include "preferencedialog.h"
#include "ui_preferencedialog.h"

PreferenceDialog::PreferenceDialog(PreferenceManager* pref, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog),
    m_pref(pref)
{
    ui->setupUi(this);
    ui->comboBox->setCurrentIndex(pref->language);
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
        m_pref->language = PreferenceManager::ENGLISH;
        emit changeLocale(QLocale::English);
        break;
    case 1:
        // chinese
        m_pref->language = PreferenceManager::CHINESE;
        emit changeLocale(QLocale::Chinese);
        break;
    default:
        break;
    }
    emit changePreferences();
    QDialog::accept();
}
