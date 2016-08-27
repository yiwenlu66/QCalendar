#include "dataadapter.h"
#include <QJsonArray>
#include <algorithm>

constexpr char DataAdapter::KEY_ITEMS[];
constexpr char DataAdapter::KEY_DATES[];
constexpr char DataAdapter::KEY_ITEM_TYPE[];
constexpr char DataAdapter::KEY_ITEM_COLOR[];
constexpr char DataAdapter::KEY_ITEM_DATES[];
constexpr char DataAdapter::KEY_ITEM_START_TIME[];
constexpr char DataAdapter::KEY_ITEM_END_TIME[];
constexpr char DataAdapter::KEY_ITEM_TITLE[];
constexpr char DataAdapter::KEY_ITEM_LOC[];
constexpr char DataAdapter::KEY_ITEM_COMMENT[];
constexpr char DataAdapter::KEY_DATE_DATE[];
constexpr char DataAdapter::KEY_DATE_ITEMS[];
constexpr char DataAdapter::DATE_FORMAT[];
constexpr char DataAdapter::TIME_FORMAT[];

DataAdapter::DataAdapter() :
    m_events(QHash<QString, CalendarEvent*>()),
    m_dates(QMap<QDate, QStringList>())
{
}

DataAdapter::DataAdapter(const QJsonObject & json)
{
    QJsonObject items = json.value(KEY_ITEMS).toObject();
    QJsonArray dates = json.value(KEY_DATES).toArray();
    for (auto it = items.begin(); it != items.end(); ++it) {
        QString sha1 = it.key();
        if (sha1.isEmpty()) {
            continue;
        }
        QJsonObject jsonItem = it.value().toObject();

        int typeInt = jsonItem.value(KEY_ITEM_TYPE).toInt(-1);
        ItemType type;
        if (EVENT <= typeInt && typeInt <= FILE) {
            type = (ItemType)typeInt;
        } else {
            continue;
        }

        int color = jsonItem.value(KEY_ITEM_COLOR).toInt(-1);
        if (color == -1) {
            continue;
        }

        QJsonArray datesJsonArray = jsonItem.value(KEY_ITEM_DATES).toArray();
        QList<QDate> dates;
        foreach (const QJsonValue& dateValue, datesJsonArray) {
            QString dateString = dateValue.toString();
            QDate date = QDate::fromString(dateString, DATE_FORMAT);
            if (date.isValid()) {
                dates.append(date);
            }
        }
        std::sort(dates.begin(), dates.end());
        if (dates.isEmpty()) {
            continue;
        }

        QTime startTime = QTime::fromString(jsonItem.value(KEY_ITEM_START_TIME).toString(), TIME_FORMAT);
        QTime endTime = QTime::fromString(jsonItem.value(KEY_ITEM_END_TIME).toString(), TIME_FORMAT);
        if (!startTime.isValid() || !endTime.isValid()) {
            continue;
        }

        QString title = jsonItem.value(KEY_ITEM_TITLE).toString();
        if (title.isEmpty()) {
            continue;
        }

        QString location = jsonItem.value(KEY_ITEM_LOC).toString();
        QString comments = jsonItem.value(KEY_ITEM_COMMENT).toString();

        if (type == EVENT) {
            m_events.insert(sha1, new CalendarEvent(color, dates, startTime, endTime, title, location, comments));
        }
        // TODO: support files

    }

    foreach (const QJsonValue& dateObjectValue, dates) {
        QJsonObject dateObject = dateObjectValue.toObject();
        QDate date = QDate::fromString(dateObject.value(KEY_DATE_DATE).toString(), DATE_FORMAT);
        if (!date.isValid()) {
            continue;
        } else {
            QJsonArray itemsJson = dateObject.value(KEY_DATE_ITEMS).toArray();
            QStringList items;
            foreach (const QJsonValue& itemValue, itemsJson) {
                QString sha1 = itemValue.toString();
                if (m_events.contains(sha1)) {
                    items.append(sha1);
                }
            }
            if (items.isEmpty()) {
                continue;
            }
            m_dates.insert(date, items);
        }
    }
}

const CalendarEvent* DataAdapter::getEvent(const QString &sha1) const
{
    if (m_events.contains(sha1)) {
        return m_events.value(sha1);
    }
    return nullptr;
}


void DataAdapter::addOrUpdateEvent(const QString& sha1, CalendarEvent* event)
{
    if (m_events.contains(sha1)) {
        const CalendarEvent* oldEvent = m_events.value(sha1);
        for (auto date : oldEvent->dates) {
            if (m_dates.contains(date)) {
                m_dates[date].removeAll(sha1);
            }
        }
    }
    m_events.insert(sha1, event);
    for (auto date : event->dates) {
        if (m_dates.contains(date)) {
            if (!m_dates[date].contains(sha1)) {
                m_dates[date].append(sha1);
            }
        } else {
            QStringList* dayList = new QStringList;
            dayList->append(sha1);
            m_dates.insert(date, *dayList);
        }
    }
    emit updateData();
}

void DataAdapter::deleteSingleEvent(const QString& sha1, const QDate& date)
{
    if (m_dates.contains(date) && m_events.contains(sha1)) {
        if (m_dates.value(date).contains(sha1)) {
            m_dates[date].removeAll(sha1);
            if (m_dates.value(date).isEmpty()) {
                m_dates.remove(date);
            }
        }
        if (m_events.value(sha1)->dates.contains(date)) {
            m_events.value(sha1)->dates.removeAll(date);
            if (m_events.value(sha1)->dates.isEmpty()) {
                m_events.remove(sha1);
            }
        }
    }
    emit updateData();
}

void DataAdapter::deleteSeries(const QString& sha1)
{
    if (m_events.contains(sha1)) {
        for (QDate date : m_events.value(sha1)->dates) {
            if (m_dates[date].contains(sha1)) {
                m_dates[date].removeAll(sha1);
            }
            if (m_dates[date].isEmpty()) {
                m_dates.remove(date);
            }
        }
        m_events.remove(sha1);
    }
    emit updateData();
}

QList<QStringList> DataAdapter::getEventsForMonth(int year, int month) const
{
    QList<QStringList> result;
    QDate firstDayOfMonth(year, month, 1);
    if (m_dates.isEmpty()) {
        for (int i = 0; i <= firstDayOfMonth.daysInMonth(); ++i) {
            result.append(QStringList());
        }
        return result;
    }
    auto lo = m_dates.lowerBound(firstDayOfMonth);
    result.append(QStringList());	// leave the element at index 0 blank
    for (int i = 1; i <= firstDayOfMonth.daysInMonth(); ++i) {
        if (i == lo.key().day() && lo.key().month() == month) {
            QStringList dayList;
            for (QString sha1 : lo.value()) {
                dayList.append(sha1);
            }
            result.append(dayList);
            ++lo;
        } else {
            result.append(QStringList());
        }
    }
    return result;
}

QJsonObject DataAdapter::toJson() const
{
    QJsonObject itemsObject;
    QJsonArray datesArray;

    for (auto it = m_events.begin(); it != m_events.end(); ++it) {
        QJsonObject eventObject;
        eventObject.insert(KEY_ITEM_TYPE, EVENT);
        eventObject.insert(KEY_ITEM_COLOR, it.value()->color);
        QJsonArray datesArray;
        for (auto date : it.value()->dates) {
            datesArray.append(date.toString(DATE_FORMAT));
        }
        eventObject.insert(KEY_ITEM_DATES, datesArray);
        eventObject.insert(KEY_ITEM_START_TIME, it.value()->startTime.toString(TIME_FORMAT));
        eventObject.insert(KEY_ITEM_END_TIME, it.value()->endTime.toString(TIME_FORMAT));
        eventObject.insert(KEY_ITEM_TITLE, it.value()->title);
        eventObject.insert(KEY_ITEM_LOC, it.value()->location);
        eventObject.insert(KEY_ITEM_COMMENT, it.value()->comments);

        itemsObject.insert(it.key(), eventObject);
    }
    // TODO: support files

    for (auto it = m_dates.begin(); it != m_dates.end(); ++it) {
        QJsonObject dateObject;
        dateObject.insert(KEY_DATE_DATE, it.key().toString(DATE_FORMAT));
        QJsonArray itemsArray;
        for (auto item : it.value()) {
            itemsArray.append(item);
        }
        dateObject.insert(KEY_DATE_ITEMS, itemsArray);

        datesArray.append(dateObject);
    }

    QJsonObject result;
    result.insert(KEY_ITEMS, itemsObject);
    result.insert(KEY_DATES, datesArray);

    return result;
}

DataAdapter::~DataAdapter()
{
    for (auto it = m_events.begin(); it != m_events.end(); ++it) {
        delete it.value();
    }
}
