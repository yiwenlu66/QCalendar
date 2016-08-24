#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void preferencesTriggered();

public:
    explicit MainWindow(ConfigLoader* config, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ConfigLoader* m_config;
};

#endif // MAINWINDOW_H
