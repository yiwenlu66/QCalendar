#include "mainwindow.h"
#include "preferencedialog.h"
#include "ui_mainwindow.h"
#include <QTranslator>

MainWindow::MainWindow(QApplication* app, ConfigLoader* config, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_app(app),
    m_appTranslator(new QTranslator),
    m_qtTranslator(new QTranslator),
    m_config(config)
{
    ui->setupUi(this);

    m_appTranslator->load(":/i18n/QCalendar_zh_CN");
    m_qtTranslator->load(":/i18n/qt_zh_CN");

    switch (config->pref()->language) {
    case PreferenceManager::ENGLISH:
        localeChanged(QLocale::English);
        break;
    case PreferenceManager::CHINESE:
        localeChanged(QLocale::Chinese);
        break;
    }

    ui->calendarWidget->setFirstDayOfWeek(config->pref()->startOfWeek);

    connect(ui->actionPreferences, SIGNAL(triggered(bool)), this, SLOT(preferencesTriggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::localeChanged(const QLocale& locale)
{
    ui->calendarWidget->setLocale(locale);
    if (locale == QLocale::Chinese) {
        m_app->installTranslator(m_appTranslator);
        m_app->installTranslator(m_qtTranslator);
    } else {
        m_app->removeTranslator(m_appTranslator);
        m_app->removeTranslator(m_qtTranslator);
    }
    ui->retranslateUi(this);
}

void MainWindow::preferencesTriggered()
{
    PreferenceDialog dlg(m_config->pref());
    connect(&dlg, SIGNAL(changeLocale(QLocale)), this, SLOT(localeChanged(QLocale)));
    connect(&dlg, SIGNAL(changeStartOfWeek(Qt::DayOfWeek)), ui->calendarWidget, SLOT(startOfWeekChanged(Qt::DayOfWeek)));
    connect(&dlg, SIGNAL(changePreferences()), m_config, SLOT(configChanged()));
    dlg.exec();
}
