#include "calendar.h"
#include <QLocale>

Calendar::Calendar(QWidget* parent)
    : QCalendarWidget(parent)
{
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setGridVisible(true);
    setSelectionMode(QCalendarWidget::NoSelection);
    setStyleSheet("selection-background-color: cornsilk");
    setNavigationBarVisible(false);
}

void Calendar::paintCell(QPainter * painter, const QRect & rect, const QDate & date) const
{
    if (date.month() != QDate::currentDate().month()) {
        painter->setPen(Qt::lightGray);
    } else if (date.dayOfWeek() == Qt::Saturday || date.dayOfWeek() == Qt::Sunday) {
        painter->setPen(Qt::red);
    } else {
        painter->setPen(Qt::black);
    }
    painter->drawText(rect, Qt::AlignTop | Qt::AlignRight, QString::number(date.day()));
}

void Calendar::startOfWeekChanged(Qt::DayOfWeek day)
{
    setFirstDayOfWeek(day);
}
