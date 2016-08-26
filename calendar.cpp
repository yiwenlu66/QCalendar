#include "calendar.h"
#include <QLocale>
#include <QFontMetrics>
#include <QDateTime>

const int Calendar::FONTSIZE_DAYOFMONTH;
const int Calendar::FONTSIZE_ITEMTITLE;
const char Calendar::COLOR_TODAY[];
const int Calendar::MARGIN_BELOW_DAY;
const int Calendar::MARGIN_BETWEEN_TILES;
const int Calendar::MARGIN_TILE_SIDE;
const int Calendar::PADDING_TOP;
const int Calendar::PADDING_BOTTOM;
const int Calendar::PADDING_LEFT;
const int Calendar::PADDING_RIGHT;

Calendar::Calendar(QWidget* parent):
    QCalendarWidget(parent)
{
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setGridVisible(true);
    setStyleSheet("selection-background-color: white");
    setNavigationBarVisible(false);
    connect(this, SIGNAL(currentPageChanged(int,int)), this, SLOT(loadMonthEventList()));
}

void Calendar::paintCell(QPainter * painter, const QRect & rect, const QDate & date) const
{
    if (date == QDateTime::currentDateTime().date()) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(COLOR_TODAY));
        painter->drawRect(rect);
    }
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
        int maxLine = (rect.height() - FONTSIZE_DAYOFMONTH - MARGIN_BELOW_DAY) /
                (PADDING_TOP + FONTSIZE_ITEMTITLE + PADDING_BOTTOM + MARGIN_BETWEEN_TILES);
        painter->save();
        painter->translate(rect.x(), rect.y());
        painter->translate(MARGIN_TILE_SIDE, FONTSIZE_DAYOFMONTH + MARGIN_BELOW_DAY);
        auto dayList = m_monthEventList[date.day()];
        for (int i = 0; i < dayList.size(); ++i) {
            if (i == maxLine - 1 && i < dayList.size() - 1) {
                painter->setPen(Qt::gray);
                painter->drawText(PADDING_LEFT, FONTSIZE_ITEMTITLE + PADDING_TOP,
                                  tr("%1 More").arg(dayList.size() - i));
                break;
            }
            QString sha1 = dayList[i];
            const CalendarEvent* event = m_dataAdapter->getEvent(sha1);
            painter->setPen(Qt::NoPen);
            painter->setBrush(event->color);
            painter->drawRect(0, 0, rect.width() - 2 * MARGIN_TILE_SIDE,
                              PADDING_TOP + FONTSIZE_ITEMTITLE + PADDING_BOTTOM);
            painter->save();
            painter->translate(PADDING_LEFT, PADDING_TOP + FONTSIZE_ITEMTITLE);
            painter->setPen(Qt::black);
            QString elidedTitle = itemTitleMetrics.elidedText(event->title, Qt::ElideRight,
                                                              rect.width() - PADDING_LEFT - PADDING_RIGHT - 2 * MARGIN_TILE_SIDE);
            painter->drawText(0, 0, elidedTitle);
            painter->restore();
            painter->translate(0, PADDING_TOP + FONTSIZE_ITEMTITLE + PADDING_BOTTOM + MARGIN_BETWEEN_TILES);
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
