#ifndef CALENDAR_H
#define CALENDAR_H

#include <QCalendarWidget>
#include <QPainter>
#include <QLocale>
#include "dataadapter.h"

class Calendar : public QCalendarWidget
{
    Q_OBJECT

public:
    Calendar(QWidget* parent = Q_NULLPTR);
    void setDataAdapter(DataAdapter* dataAdapter);

public slots:
    void startOfWeekChanged(Qt::DayOfWeek);

private slots:
    void loadMonthEventList();

protected:
    void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;

private:
    static const int FONTSIZE_DAYOFMONTH = 16;
    static const int FONTSIZE_ITEMTITLE = 12;
    DataAdapter* m_dataAdapter;
    QList<QStringList> m_monthEventList;
};

#endif // CALENDAR_H
