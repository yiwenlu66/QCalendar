#include "preferencedialog.h"
#include "ui_preferencedialog.h"
#include <QPushButton>

PreferenceDialog::PreferenceDialog(PreferenceManager* pref, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog),
    m_pref(pref)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    ui->comboBox_language->setCurrentIndex(pref->language);
    ui->comboBox_startOfWeek->setCurrentIndex(pref->startOfWeek - Qt::Monday);
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::accept()
{
    // update locale
    switch (ui->comboBox_language->currentIndex()) {
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

    // update start of week
    Qt::DayOfWeek startOfWeek = (Qt::DayOfWeek)(ui->comboBox_startOfWeek->currentIndex() + Qt::Monday);
    m_pref->startOfWeek = startOfWeek;
    emit changeStartOfWeek(startOfWeek);

    emit changePreferences();
    QDialog::accept();
}
