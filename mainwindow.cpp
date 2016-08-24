#include "mainwindow.h"
#include "preferencedialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ConfigLoader* config, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_config(config)
{
    ui->setupUi(this);

    switch (config->pref()->language) {
    case PreferenceManager::ENGLISH:
        localeChanged(QLocale::English);
        break;
    case PreferenceManager::CHINESE:
        localeChanged(QLocale::Chinese);
        break;
    }

    connect(ui->actionPreferences, SIGNAL(triggered(bool)), this, SLOT(preferencesTriggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::localeChanged(const QLocale& locale)
{
    ui->calendarWidget->setLocale(locale);
}

void MainWindow::preferencesTriggered()
{
    PreferenceDialog dlg(m_config->pref());
    connect(&dlg, SIGNAL(changeLocale(QLocale)), this, SLOT(localeChanged(QLocale)));
    connect(&dlg, SIGNAL(changePreferences()), m_config, SLOT(configChanged()));
    dlg.exec();
}
