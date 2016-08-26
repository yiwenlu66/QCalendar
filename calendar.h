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
    static const int MARGIN_BELOW_DAY = 4;
    static const int MARGIN_BETWEEN_TILES = 2;
    static const int MARGIN_TILE_SIDE = 2;
    static const int PADDING_TOP = 2;
    static const int PADDING_BOTTOM = 2;
    static const int PADDING_LEFT = 2;
    static const int PADDING_RIGHT = 2;
    static constexpr char COLOR_TODAY[] = "#FFF8DC";
    DataAdapter* m_dataAdapter;
    QList<QStringList> m_monthEventList;
};

#endif // CALENDAR_H
