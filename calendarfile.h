#ifndef CALENDARFILE_H
#define CALENDARFILE_H

#include <QDate>
#include <QString>

class CalendarFile
{
public:
    CalendarFile(int color, const QDate& date, const QString& title, const QString& contentSha1);
    int color;
    QDate date;
    QString title;
    QString contentSha1;
};

#endif // CALENDARFILE_H
