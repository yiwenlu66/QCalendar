#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>
#include <QDate>
#include <QTime>
#include <QString>
#include <QList>
#include "calendarevent.h"

namespace Ui {
class EventDialog;
}

class EventDialog : public QDialog
{
    Q_OBJECT

public:
    enum RepeatMode {
        NONE,
        ONCE,
        DAILY,
        WEEKLY,
        MONTHLY,
        YEARLY,
        CUSTOM_EVEN,    // with equal intervals between adjacent events
        CUSTOM_UNEVEN    // with inequal intervals between adjacent events
    };

    explicit EventDialog(QWidget *parent = 0);
    EventDialog(const QDate& date, QWidget *parent = 0);
    EventDialog(const CalendarEvent& event, QWidget *parent = 0);

    QString title();
    QList<QDate> dates();
    QTime startTime();
    QTime endTime();
    Qt::GlobalColor color();
    QString location();
    QString comments();

    ~EventDialog();

public slots:
    void repeatModeSet();
    void repeatModeChanged();
    void checkInputsLegal();

private:
    Ui::EventDialog *ui;
    static RepeatMode getRepeatMode(const QList<QDate>&);
    void hideRepeatIntervalUi();
    void showRepeatIntervalUi();
    void hideRepeatCountUi();
    void showRepeatCountUi();
};

#endif // EVENTDIALOG_H
