#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QLocale>
#include "configloader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void localeChanged(const QLocale& locale);
    void monthChanged(int year, int month);
    void showEventDialog(const QString& sha1);

private slots:
    void preferencesTriggered();

signals:
    void doubleClick(int x, int y);
    void resizeCells();

public:
    explicit MainWindow(ConfigLoader* config, QWidget *parent = 0);
    bool eventFilter(QObject *watched, QEvent *event);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTranslator* m_appTranslator;
    QTranslator* m_qtTranslator;

    ConfigLoader* m_config;

    static QString getLongMonthName(int);
};

#endif // MAINWINDOW_H
