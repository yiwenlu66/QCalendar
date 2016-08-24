#include "mainwindow.h"
#include "preferencedialog.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_locale(QLocale::English)
{
    ui->setupUi(this);
    connect(ui->actionPreferences, SIGNAL(triggered(bool)), this, SLOT(preferencesTriggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::localeChanged(const QLocale& locale)
{
    m_locale = locale;
    ui->calendarWidget->setLocale(locale);
}

void MainWindow::preferencesTriggered()
{
    PreferenceDialog dlg(m_locale);
    connect(&dlg, SIGNAL(changeLocale(QLocale)), this, SLOT(localeChanged(QLocale)));
    dlg.exec();
}
