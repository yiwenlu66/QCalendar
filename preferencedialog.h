#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>
#include <QLocale>

namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QDialog
{
    Q_OBJECT
signals:
    void changeLocale(const QLocale& locate);

protected slots:
    void accept();

public:
    explicit PreferenceDialog(const QLocale& currentLocale, QWidget *parent = 0);
    ~PreferenceDialog();

private:
    Ui::PreferenceDialog *ui;
};

#endif // PREFERENCEDIALOG_H
