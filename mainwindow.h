#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QLocale>
#include <QKeyEvent>
#include <QTableView>
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

private slots:
    void preferencesTriggered();
    void exportTriggered();
    void importTriggered();
    void setPinWindow(bool);
    void toggleDrop(bool);

signals:
    void doubleClick(int x, int y);
    void resizeCells();

public:
    explicit MainWindow(ConfigLoader* config, QWidget *parent = 0);
    bool eventFilter(QObject *watched, QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTableView* m_tableView;

    QTranslator* m_appTranslator;
    QTranslator* m_qtTranslator;

    ConfigLoader* m_config;

    static QString getLongMonthName(int);
};

#endif // MAINWINDOW_H
