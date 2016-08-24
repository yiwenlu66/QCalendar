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
    static constexpr char KEY_LANG[] = "language";
    static constexpr char KEY_STARTOFWEEK[] = "startOfWeek";

    static Language getDefaultLanguage();
};

#endif // PREFERENCEMANAGER_H
