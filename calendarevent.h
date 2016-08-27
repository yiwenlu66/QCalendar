#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include <Qt>
#include <QList>
#include <QDate>
#include <QTime>

class CalendarEvent
{
public:
    CalendarEvent(int color, QList<QDate> dates, QTime startTime, QTime endTime,
                  QString title, QString location, QString comments);
    int color;
    QList<QDate> dates;
    QTime startTime;
    QTime endTime;
    QString title;
    QString location;
    QString comments;
};

#endif // CALENDAREVENT_H
