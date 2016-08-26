#include "mainwindow.h"
#include "preferencedialog.h"
#include "ui_mainwindow.h"
#include <QTranslator>
#include <QApplication>
#include <QtDebug>

MainWindow::MainWindow(ConfigLoader* config, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
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

    ui->calendarWidget->setDataAdapter(config->data());

    ui->calendarWidget->setFirstDayOfWeek(config->pref()->startOfWeek);

    monthChanged(QDate::currentDate().year(), QDate::currentDate().month());
    connect(ui->calendarWidget, SIGNAL(currentPageChanged(int,int)), this, SLOT(monthChanged(int,int)));

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
        qApp->installTranslator(m_appTranslator);
        qApp->installTranslator(m_qtTranslator);
    } else {
        qApp->removeTranslator(m_appTranslator);
        qApp->removeTranslator(m_qtTranslator);
    }
    ui->retranslateUi(this);
}

void MainWindow::monthChanged(int year, int month)
{
    ui->label_month->setText(QString("<h1>") + getLongMonthName(month) + QString("</h1>"));
    ui->label_year->setText(QString::number(year));
}

void MainWindow::preferencesTriggered()
{
    PreferenceDialog dlg(m_config->pref());
    connect(&dlg, SIGNAL(changeLocale(QLocale)), this, SLOT(localeChanged(QLocale)));
    connect(&dlg, SIGNAL(changeStartOfWeek(Qt::DayOfWeek)), ui->calendarWidget, SLOT(startOfWeekChanged(Qt::DayOfWeek)));
    connect(&dlg, SIGNAL(changePreferences()), m_config, SLOT(configChanged()));
    dlg.exec();
}

QString MainWindow::getLongMonthName(int month)
{
    switch (month) {
    case 1:
        return tr("January");
    case 2:
        return tr("February");
    case 3:
        return tr("March");
    case 4:
        return tr("April");
    case 5:
        return tr("May");
    case 6:
        return tr("June");
    case 7:
        return tr("July");
    case 8:
        return tr("August");
    case 9:
        return tr("September");
    case 10:
        return tr("October");
    case 11:
        return tr("November");
    case 12:
        return tr("December");
    default:
        return QString("");
    }
}
