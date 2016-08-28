#include "calendarfile.h"

CalendarFile::CalendarFile(int color, const QDate& date, const QString& title, const QString& contentSha1) :
    color(color),
    date(date),
    title(title),
    contentSha1(contentSha1)
{
}
