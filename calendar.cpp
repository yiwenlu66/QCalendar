#include "calendar.h"
#include <QLocale>
#include <QFontMetrics>

const int Calendar::FONTSIZE_DAYOFMONTH;
const int Calendar::FONTSIZE_ITEMTITLE;

Calendar::Calendar(QWidget* parent):
    QCalendarWidget(parent)
{
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setGridVisible(true);
    setSelectionMode(QCalendarWidget::NoSelection);
    setStyleSheet("selection-background-color: cornsilk");
    setNavigationBarVisible(false);
    connect(this, SIGNAL(currentPageChanged(int,int)), this, SLOT(loadMonthEventList()));
}

void Calendar::paintCell(QPainter * painter, const QRect & rect, const QDate & date) const
{
    if (date.month() != monthShown()) {
        painter->setPen(Qt::lightGray);
    } else if (date.dayOfWeek() == Qt::Saturday || date.dayOfWeek() == Qt::Sunday) {
        painter->setPen(Qt::red);
    } else {
        painter->setPen(Qt::black);
    }
    painter->setFont(QFont("Sans", FONTSIZE_DAYOFMONTH));
    painter->drawText(rect, Qt::AlignTop | Qt::AlignRight, QString::number(date.day()));
    QFont itemTitleFont = QFont("Sans", FONTSIZE_ITEMTITLE);
    QFontMetrics itemTitleMetrics(itemTitleFont);
    painter->setFont(itemTitleFont);
    if (date.month() == monthShown() && !m_monthEventList[date.day()].isEmpty()) {
        int maxLine = (rect.height() - FONTSIZE_DAYOFMONTH) / (FONTSIZE_ITEMTITLE + 3);
        painter->save();
        painter->translate(rect.x(), rect.y());
        painter->translate(0, FONTSIZE_DAYOFMONTH + 1);
        auto dayList = m_monthEventList[date.day()];
        for (int i = 0; i < dayList.size(); ++i) {
            if (i == maxLine - 1 && i < dayList.size() - 1) {
                painter->setPen(Qt::gray);
                painter->drawText(1, FONTSIZE_ITEMTITLE + 1, tr("%1 More").arg(dayList.size() - i));
                break;
            }
            QString sha1 = dayList[i];
            const CalendarEvent* event = m_dataAdapter->getEvent(sha1);
            painter->setPen(Qt::NoPen);
            painter->setBrush(event->color);
            painter->drawRect(1, 0, rect.width() - 2, FONTSIZE_ITEMTITLE + 2);
            painter->setPen(Qt::black);
            QString elidedTitle = itemTitleMetrics.elidedText(event->title, Qt::ElideRight, rect.width() - 3);
            painter->drawText(2, FONTSIZE_ITEMTITLE + 1, elidedTitle);
            painter->translate(0, FONTSIZE_ITEMTITLE + 3);
        }
        painter->restore();
    }
}

void Calendar::startOfWeekChanged(Qt::DayOfWeek day)
{
    setFirstDayOfWeek(day);
}

void Calendar::loadMonthEventList()
{
    m_monthEventList = m_dataAdapter->getEventsForMonth(yearShown(), monthShown());
    update();
}

void Calendar::setDataAdapter(DataAdapter *dataAdapter)
{
    m_dataAdapter = dataAdapter;
    loadMonthEventList();
}
