#include "calendarevent.h"

CalendarEvent::CalendarEvent(int color, QList<QDate> dates, QTime startTime,
                             QTime endTime, QString title, QString location, QString comments) :
    color(color),
    dates(dates),
    startTime(startTime),
    endTime(endTime),
    title(title),
    location(location),
    comments(comments)
{
}
