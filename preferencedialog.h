#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>
#include <QLocale>
#include "preferencemanager.h"

namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QDialog
{
    Q_OBJECT
signals:
    void changeLocale(const QLocale& locate);
    void changeStartOfWeek(Qt::DayOfWeek day);
    void changePreferences();

protected slots:
    void accept();

public:
    explicit PreferenceDialog(PreferenceManager* pref, QWidget *parent = 0);
    ~PreferenceDialog();

private:
    Ui::PreferenceDialog *ui;
    PreferenceManager* m_pref;
};

#endif // PREFERENCEDIALOG_H
