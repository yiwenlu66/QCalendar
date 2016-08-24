#ifndef PREFERENCEMANAGER_H
#define PREFERENCEMANAGER_H

#include <QJsonObject>

class PreferenceManager
{
public:
    enum Language {
        ENGLISH,
        CHINESE
    };
    PreferenceManager();
    PreferenceManager(const QJsonObject&);
    Language language;
    Qt::DayOfWeek startOfWeek;
    QJsonObject toJson();

private:
    static Language getDefaultLanguage();
};

#endif // PREFERENCEMANAGER_H
