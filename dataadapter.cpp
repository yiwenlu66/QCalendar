#include "dataadapter.h"
#include <QJsonArray>
#include <algorithm>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

constexpr char DataAdapter::KEY_EVENTS[];
constexpr char DataAdapter::KEY_FILES[];
constexpr char DataAdapter::KEY_DATES[];
constexpr char DataAdapter::KEY_EVENT_COLOR[];
constexpr char DataAdapter::KEY_EVENT_DATES[];
constexpr char DataAdapter::KEY_EVENT_START_TIME[];
constexpr char DataAdapter::KEY_EVENT_END_TIME[];
constexpr char DataAdapter::KEY_EVENT_TITLE[];
constexpr char DataAdapter::KEY_EVENT_LOC[];
constexpr char DataAdapter::KEY_EVENT_COMMENT[];
constexpr char DataAdapter::KEY_FILE_COLOR[];
constexpr char DataAdapter::KEY_FILE_DATE[];
constexpr char DataAdapter::KEY_FILE_TITLE[];
constexpr char DataAdapter::KEY_FILE_HASH[];
constexpr char DataAdapter::KEY_DATE_DATE[];
constexpr char DataAdapter::KEY_DATE_EVENTS[];
constexpr char DataAdapter::KEY_DATE_FILES[];
constexpr char DataAdapter::DATE_FORMAT[];
constexpr char DataAdapter::TIME_FORMAT[];

DataAdapter::DataAdapter() :
    m_events(QHash<QString, CalendarEvent*>()),
    m_dates(QMap<QDate, QPair<QStringList, QStringList>>())
{
}

DataAdapter::DataAdapter(const QJsonObject & json)
{
    QJsonObject events = json.value(KEY_EVENTS).toObject();
    QJsonObject files = json.value(KEY_FILES).toObject();
    QJsonArray dates = json.value(KEY_DATES).toArray();

    for (auto it = events.begin(); it != events.end(); ++it) {
        QString sha1 = it.key();
        if (sha1.isEmpty()) {
            continue;
        }
        QJsonObject jsonEvent = it.value().toObject();

        int color = jsonEvent.value(KEY_EVENT_COLOR).toInt(-1);
        if (color == -1) {
            continue;
        }

        QJsonArray datesJsonArray = jsonEvent.value(KEY_EVENT_DATES).toArray();
        QList<QDate> dates;
        foreach (const QJsonValue& dateValue, datesJsonArray) {
            QString dateString = dateValue.toString();
            QDate date = QDate::fromString(dateString, DATE_FORMAT);
            if (date.isValid()) {
                dates.append(date);
            }
        }
        if (dates.isEmpty()) {
            continue;
        }
        std::sort(dates.begin(), dates.end());

        QTime startTime = QTime::fromString(jsonEvent.value(KEY_EVENT_START_TIME).toString(), TIME_FORMAT);
        QTime endTime = QTime::fromString(jsonEvent.value(KEY_EVENT_END_TIME).toString(), TIME_FORMAT);
        if (!startTime.isValid() || !endTime.isValid()) {
            continue;
        }

        QString title = jsonEvent.value(KEY_EVENT_TITLE).toString();
        if (title.isEmpty()) {
            continue;
        }

        QString location = jsonEvent.value(KEY_EVENT_LOC).toString();
        QString comments = jsonEvent.value(KEY_EVENT_COMMENT).toString();

        m_events.insert(sha1, new CalendarEvent(color, dates, startTime, endTime, title, location, comments));
    }

    for (auto it = files.begin(); it != files.end(); ++it) {
        QString sha1 = it.key();
        if (sha1.isEmpty()) {
            continue;
        }
        QJsonObject jsonFileObject = it.value().toObject();

        int color = jsonFileObject.value(KEY_FILE_COLOR).toInt(-1);
        if (color == -1) {
            continue;
        }

        QString dateString = jsonFileObject.value(KEY_FILE_DATE).toString();
        QDate date = QDate::fromString(dateString, DATE_FORMAT);
        if (!date.isValid()) {
            continue;
        }

        QString title = jsonFileObject.value(KEY_FILE_TITLE).toString();
        if (title.isEmpty()) {
            continue;
        }

        QString contentSha1 = jsonFileObject.value(KEY_FILE_HASH).toString();
        if (contentSha1.isEmpty()) {
            continue;
        }

        m_files.insert(sha1, new CalendarFile(color, date, title, contentSha1));
    }

    foreach (const QJsonValue& dateObjectValue, dates) {
        QJsonObject dateObject = dateObjectValue.toObject();
        QDate date = QDate::fromString(dateObject.value(KEY_DATE_DATE).toString(), DATE_FORMAT);
        if (!date.isValid()) {
            continue;
        } else {
            QJsonArray eventsJson = dateObject.value(KEY_DATE_EVENTS).toArray();
            QStringList events;
            foreach (const QJsonValue& eventValue, eventsJson) {
                QString sha1 = eventValue.toString();
                if (m_events.contains(sha1)) {
                    events.append(sha1);
                }
            }

            QJsonArray filesJson = dateObject.value(KEY_DATE_FILES).toArray();
            QStringList files;
            foreach (const QJsonValue& fileValue, filesJson) {
                QString itemSha1 = fileValue.toString();
                if (m_files.contains(itemSha1)) {
                    files.append(itemSha1);
                }
            }

            if (events.isEmpty() && files.isEmpty()) {
                continue;
            }
            m_dates.insert(date, QPair<QStringList, QStringList>(events, files));
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

const CalendarFile* DataAdapter::getFile(const QString &itemSha1) const
{
    if (m_files.contains(itemSha1)) {
        return m_files.value(itemSha1);
    }
    return nullptr;
}


void DataAdapter::addOrUpdateEvent(const QString& sha1, CalendarEvent* event)
{
    if (m_events.contains(sha1)) {
        const CalendarEvent* oldEvent = m_events.value(sha1);
        for (auto date : oldEvent->dates) {
            if (m_dates.contains(date)) {
                m_dates[date].first.removeAll(sha1);
            }
        }
    }
    m_events.insert(sha1, event);
    for (auto date : event->dates) {
        if (m_dates.contains(date)) {
            if (!m_dates[date].first.contains(sha1)) {
                m_dates[date].first.append(sha1);
            }
        } else {
            QStringList* dayList = new QStringList;
            dayList->append(sha1);
            m_dates.insert(date, QPair<QStringList, QStringList>(*dayList, QStringList()));
        }
    }
    emit updateData();
}

void DataAdapter::addFile(const QString &itemSha1, CalendarFile *file)
{
    if (!m_files.contains(itemSha1)) {
        m_files.insert(itemSha1, file);
        QDate date = file->date;
        if (m_dates.contains(date)) {
            if (!m_dates[date].second.contains(itemSha1)) {
                m_dates[date].second.append(itemSha1);
            }
        } else {
            QStringList* dayList = new QStringList;
            dayList->append(itemSha1);
            m_dates.insert(date, QPair<QStringList, QStringList>(QStringList(), *dayList));
        }
    }
    emit updateData();
}

void DataAdapter::changeFileColor(const QString &itemSha1, int color)
{
    if (m_files.contains(itemSha1)) {
        m_files[itemSha1]->color = color;
    }
    emit updateData();
}

void DataAdapter::deleteSingleEvent(const QString& sha1, const QDate& date)
{
    if (m_dates.contains(date) && m_events.contains(sha1)) {
        if (m_dates.value(date).first.contains(sha1)) {
            m_dates[date].first.removeAll(sha1);
            if (m_dates.value(date).first.isEmpty() && m_dates.value(date).second.isEmpty()) {
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
            if (m_dates[date].first.contains(sha1)) {
                m_dates[date].first.removeAll(sha1);
            }
            if (m_dates[date].first.isEmpty() && m_dates[date].second.isEmpty()) {
                m_dates.remove(date);
            }
        }
        m_events.remove(sha1);
    }
    emit updateData();
}

void DataAdapter::deleteFile(const QString &itemSha1)
{
    if (m_files.contains(itemSha1)) {
        QDate date = m_files.value(itemSha1)->date;
        QString contentSha1 = m_files.value(itemSha1)->contentSha1;
        if (m_dates[date].second.contains(itemSha1)) {
            m_dates[date].second.removeAll(itemSha1);
        }
        if (m_dates[date].first.isEmpty() && m_dates[date].second.isEmpty()) {
            m_dates.remove(date);
        }
        m_files.remove(itemSha1);

        // garbage-collect disk file
        bool hasReference = false;
        for (auto file : m_files) {
            if (file->contentSha1 == contentSha1) {
                hasReference = true;
                break;
            }
        }
        if (!hasReference) {
            QString filePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)
                    + QDir::separator() + "files" + QDir::separator() + contentSha1;
            QFile::remove(filePath);
        }
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
            for (QString sha1 : lo.value().first) {
                dayList.append(sha1);
            }
            std::sort(dayList.begin(), dayList.end(), [this](const QString& k1, const QString& k2) {
                return getEvent(k1)->startTime < getEvent(k2)->startTime;
            });
            result.append(dayList);
            ++lo;
        } else {
            result.append(QStringList());
        }
    }
    return result;
}

QList<QStringList> DataAdapter::getFilesForMonth(int year, int month) const
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
            for (QString itemSha1 : lo.value().second) {
                dayList.append(itemSha1);
            }
            std::sort(dayList.begin(), dayList.end(), [this](const QString& k1, const QString& k2) {
                return getFile(k1)->title < getFile(k2)->title;
            });
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
    QJsonObject eventsObject;
    QJsonObject filesObject;
    QJsonArray datesArray;

    for (auto it = m_events.begin(); it != m_events.end(); ++it) {
        QJsonObject eventObject;
        eventObject.insert(KEY_EVENT_COLOR, it.value()->color);
        QJsonArray datesArray;
        for (auto date : it.value()->dates) {
            datesArray.append(date.toString(DATE_FORMAT));
        }
        eventObject.insert(KEY_EVENT_DATES, datesArray);
        eventObject.insert(KEY_EVENT_START_TIME, it.value()->startTime.toString(TIME_FORMAT));
        eventObject.insert(KEY_EVENT_END_TIME, it.value()->endTime.toString(TIME_FORMAT));
        eventObject.insert(KEY_EVENT_TITLE, it.value()->title);
        eventObject.insert(KEY_EVENT_LOC, it.value()->location);
        eventObject.insert(KEY_EVENT_COMMENT, it.value()->comments);

        eventsObject.insert(it.key(), eventObject);
    }

    for (auto it = m_files.begin(); it != m_files.end(); ++it) {
        QJsonObject fileObject;
        fileObject.insert(KEY_FILE_COLOR, it.value()->color);
        fileObject.insert(KEY_FILE_DATE, it.value()->date.toString(DATE_FORMAT));
        fileObject.insert(KEY_FILE_TITLE, it.value()->title);
        fileObject.insert(KEY_FILE_HASH, it.value()->contentSha1);

        filesObject.insert(it.key(), fileObject);
    }

    for (auto it = m_dates.begin(); it != m_dates.end(); ++it) {
        QJsonObject dateObject;
        dateObject.insert(KEY_DATE_DATE, it.key().toString(DATE_FORMAT));
        QJsonArray eventsArray, filesArray;
        for (auto item : it.value().first) {
            eventsArray.append(item);
        }
        for (auto item : it.value().second) {
            filesArray.append(item);
        }
        dateObject.insert(KEY_DATE_EVENTS, eventsArray);
        dateObject.insert(KEY_DATE_FILES, filesArray);

        datesArray.append(dateObject);
    }

    QJsonObject result;
    result.insert(KEY_EVENTS, eventsObject);
    result.insert(KEY_FILES, filesObject);
    result.insert(KEY_DATES, datesArray);

    return result;
}

DataAdapter::~DataAdapter()
{
    for (auto it = m_events.begin(); it != m_events.end(); ++it) {
        delete it.value();
    }
}
