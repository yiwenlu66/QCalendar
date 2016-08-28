#ifndef DATAADAPTER_H
#define DATAADAPTER_H

#include "calendarevent.h"
#include "calendarfile.h"
#include <QObject>
#include <QList>
#include <QJsonObject>
#include <QHash>
#include <QMap>
#include <QPair>

class DataAdapter : public QObject
{

    Q_OBJECT

public:
    static constexpr char const KEY_EVENTS[] = "events";
    static constexpr char const KEY_FILES[] = "files";
    static constexpr char const KEY_DATES[] = "dates";

    DataAdapter();
    DataAdapter(const QJsonObject&);

    const CalendarEvent* getEvent(const QString& sha1) const;
    void addOrUpdateEvent(const QString& sha1, CalendarEvent* event);
    void deleteSingleEvent(const QString& sha1, const QDate& date);
    void deleteSeries(const QString& sha1);
    QList<QStringList> getEventsForMonth(int year, int month) const;

    const CalendarFile* getFile(const QString& itemSha1) const;
    void addFile(const QString& itemSha1, CalendarFile* file);
    void changeFileColor(const QString& itemSha1, int color);
    void deleteFile(const QString& itemSha1);
    QList<QStringList> getFilesForMonth(int year, int month) const;

    QJsonObject toJson() const;
    ~DataAdapter();

signals:
    void updateData();

private:
    static constexpr char const KEY_EVENT_COLOR[] = "color";
    static constexpr char const KEY_EVENT_DATES[] = "dates";
    static constexpr char const KEY_EVENT_START_TIME[] = "startTime";
    static constexpr char const KEY_EVENT_END_TIME[] = "endTime";
    static constexpr char const KEY_EVENT_TITLE[] = "title";
    static constexpr char const KEY_EVENT_LOC[] = "location";
    static constexpr char const KEY_EVENT_COMMENT[] = "comments";
    static constexpr char const KEY_FILE_COLOR[] = "color";
    static constexpr char const KEY_FILE_DATE[] = "date";
    static constexpr char const KEY_FILE_TITLE[] = "filename";
    static constexpr char const KEY_FILE_HASH[] = "contentSha1";
    static constexpr char const KEY_DATE_DATE[] = "date";
    static constexpr char const KEY_DATE_EVENTS[] = "events";
    static constexpr char const KEY_DATE_FILES[] = "files";

    static constexpr char const DATE_FORMAT[] = "yyyy-MM-dd";
    static constexpr char const TIME_FORMAT[] = "hh:mm";

    QHash<QString, CalendarEvent*> m_events;
    QHash<QString, CalendarFile*> m_files;
    QMap<QDate, QPair<QStringList, QStringList>> m_dates;   // each item is (events, files)

};

#endif // DATAADAPTER_H
