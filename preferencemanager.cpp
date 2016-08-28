#include "preferencemanager.h"
#include <QLocale>

constexpr char PreferenceManager::KEY_LANG[];
constexpr char PreferenceManager::KEY_STARTOFWEEK[];

PreferenceManager::PreferenceManager() :
    language(getDefaultLanguage()),
    startOfWeek(Qt::Sunday)
{
}

PreferenceManager::PreferenceManager(const QJsonObject & json)
{
    QJsonValue langValue = json.value(KEY_LANG);
    switch (langValue.toInt(-1)) {
    case ENGLISH:
        language = ENGLISH;
        break;
    case CHINESE:
        language = CHINESE;
        break;
    default:
        language = getDefaultLanguage();
        break;
    }

    QJsonValue startOfWeekValue = json.value(KEY_STARTOFWEEK);
    switch (startOfWeekValue.toInt(-1)) {
    case Qt::Monday:
    case Qt::Tuesday:
    case Qt::Wednesday:
    case Qt::Thursday:
    case Qt::Friday:
    case Qt::Saturday:
    case Qt::Sunday:
        startOfWeek = (Qt::DayOfWeek)startOfWeekValue.toInt();
        break;
    default:
        startOfWeek = Qt::Sunday;
        break;
    }
}

QJsonObject PreferenceManager::toJson()
{
    QJsonObject result;
    result.insert(KEY_LANG, QJsonValue(language));
    result.insert(KEY_STARTOFWEEK, QJsonValue(startOfWeek));
    return result;
}

PreferenceManager::Language PreferenceManager::getDefaultLanguage()
{
    if (QLocale::system() == QLocale::Chinese) {
        return CHINESE;
    }
    return ENGLISH;
}
