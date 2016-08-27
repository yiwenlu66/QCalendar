#include "calendar.h"
#include "eventdialog.h"
#include "eventlistdialog.h"
#include "color.h"
#include <QLocale>
#include <QFontMetrics>
#include <QDateTime>
#include <QTimer>
#include <QStandardItemModel>
#include <QCryptographicHash>
#include <QByteArray>
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
const int Calendar::FREEZE_PERIOD;

Calendar::Calendar(QWidget* parent):
    QCalendarWidget(parent)
{
    setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    setGridVisible(true);
    setStyleSheet("selection-background-color: white");
    setNavigationBarVisible(false);
    connect(this, SIGNAL(currentPageChanged(int,int)), this, SLOT(loadMonthEventList()));
    connect(this, SIGNAL(currentPageChanged(int,int)), this, SLOT(freezeDoubleClick()));
}

void Calendar::paintCell(QPainter * painter, const QRect & rect, const QDate & date) const
{
    if (xPivots.isEmpty() || rect.x() > xPivots.last()) {
        xPivots.append(rect.x());
        cellWidths.append(rect.width());
    }
    if (yPivots.isEmpty() || rect.y() > yPivots.last()) {
        yPivots.append(rect.y());
        cellHeights.append(rect.height());
    }
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
            painter->setBrush(Color::getColor(event->color));
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
    connect(m_dataAdapter, SIGNAL(updateData()), this, SLOT(loadMonthEventList()));
    loadMonthEventList();
}

void Calendar::freezeDoubleClick()
{
    doubleClickFreezed = true;
    QTimer::singleShot(FREEZE_PERIOD, [this]{ doubleClickFreezed = false; });
}

void Calendar::doubleClicked(int x, int y)
{
    // dirty hack: exclude header and cells from adjacent months
    if (y < yPivots.first() || doubleClickFreezed) {
        return;
    }
    int xInCell, yInCell, cellWidth = 0, cellHeight = 0;
    for (int i = xPivots.size() - 1; i >= 0; --i) {
        xInCell = x - xPivots[i];
        if (xInCell >= 0) {
            cellWidth = cellWidths[i];
            break;
        }
    }
    for (int i = yPivots.size() - 1; i >= 0; --i) {
        yInCell = y - yPivots[i];
        if (yInCell >= 0) {
            cellHeight = cellHeights[i];
            break;
        }
    }
    int index = getTileIndex(xInCell, yInCell, cellWidth, cellHeight);
    int maxLine = (cellHeight - FONTSIZE_DAYOFMONTH - MARGIN_BELOW_DAY) /
        (PADDING_TOP + FONTSIZE_ITEMTITLE + PADDING_BOTTOM + MARGIN_BETWEEN_TILES);
    QStringList dayList = m_monthEventList[selectedDate().day()];
    if (dayList.size() <= maxLine) {
        if (0 <= index && index < dayList.size()) {
            return showEventDialog(dayList[index]);
        }
    } else {
        if (0 <= index && index < maxLine - 1) {
            return showEventDialog(dayList[index]);
        } else if (index == maxLine - 1) {
            return showEventList(dayList);
        }
    }

    // double-click on blank area, create new event
    return showEventDialog(selectedDate());
}

void Calendar::showEventDialog(const QString &sha1)
{
    EventDialog dlg(*(m_dataAdapter->getEvent(sha1)), this);
    execEventDialog(dlg, sha1);
}

void Calendar::showEventDialog(const QDate& date)
{
    EventDialog dlg(date, this);
    execEventDialog(dlg);
}

void Calendar::execEventDialog(EventDialog &dlg, QString sha1)
{
    if (sha1.isEmpty()) {
        sha1 = QString(QCryptographicHash::hash(
                           (dlg.title() + QString::number(QDateTime::currentMSecsSinceEpoch())).toUtf8(),
                           QCryptographicHash::Sha1).toHex());
    }
    if (dlg.exec() == QDialog::Accepted) {
        switch(dlg.deleteStatus) {
        case EventDialog::DO_NOT_DELETE:
            m_dataAdapter->addOrUpdateEvent(sha1, new CalendarEvent(
                                                dlg.color(),
                                                dlg.dates(),
                                                dlg.startTime(),
                                                dlg.endTime(),
                                                dlg.title(),
                                                dlg.location(),
                                                dlg.comments()));
            break;
        case EventDialog::DELETE_SERIES:
            m_dataAdapter->deleteSeries(sha1);
            break;
        case EventDialog::DELETE_INSTANCE:
            m_dataAdapter->deleteSingleEvent(sha1, selectedDate());
            break;
        }
    }
}

void Calendar::showEventList(const QStringList &sha1List)
{
    QStandardItemModel model(sha1List.size(), 1);
    for (int i = 0; i < sha1List.size(); ++i) {
        QStandardItem* item = new QStandardItem(
                    QString("%1,%2")
                    .arg(m_dataAdapter->getEvent(sha1List[i])->title)
                    .arg((int)m_dataAdapter->getEvent(sha1List[i])->color));
        model.setItem(i, 0, item);
    }

    EventListDialog dlg;
    dlg.setModel(model);
    if (dlg.exec() == QDialog::Accepted) {
        int index = dlg.getSelectedIndex();
        emit showEventDialog(sha1List[index]);
    }
}

void Calendar::cellsResized()
{
    xPivots.clear();
    yPivots.clear();
    cellWidths.clear();
    cellHeights.clear();
}

int Calendar::getTileIndex(int x, int y, int cellWidth, int cellHeight)
{
    if (x <= MARGIN_TILE_SIDE || x >= cellWidth - MARGIN_TILE_SIDE) {
        return -1;
    }
    int baseY = FONTSIZE_DAYOFMONTH + MARGIN_BELOW_DAY;
    int dy = PADDING_TOP + FONTSIZE_ITEMTITLE + PADDING_BOTTOM + MARGIN_BETWEEN_TILES;
    for (int i = 0; baseY + dy <= cellHeight; ++i, baseY += dy) {
        if (baseY < y && y < baseY + dy) {
            return i;
        }
    }
    return -1;
}
