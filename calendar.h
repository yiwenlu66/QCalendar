#ifndef CALENDAR_H
#define CALENDAR_H

#include <QCalendarWidget>
#include <QPainter>
#include <QLocale>

class Calendar : public QCalendarWidget
{
    Q_OBJECT

public:
    Calendar(QWidget* parent = Q_NULLPTR);

public slots:
    void startOfWeekChanged(Qt::DayOfWeek);

protected:
    void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;
};

#endif // CALENDAR_H
