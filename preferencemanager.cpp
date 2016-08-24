#include "preferencemanager.h"
#include <QLocale>
#include <QtDebug>

PreferenceManager::PreferenceManager() :
    language(getDefaultLanguage())
{
}

PreferenceManager::PreferenceManager(const QJsonObject & json)
{
    QJsonValue langValue = json.value("language");
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

    QJsonValue startOfWeekValue = json.value("startOfWeek");
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
    result.insert("language", QJsonValue(language));
    result.insert("startOfWeek", QJsonValue(startOfWeek));
    return result;
}

PreferenceManager::Language PreferenceManager::getDefaultLanguage()
{
    if (QLocale::system() == QLocale::Chinese) {
        return CHINESE;
    }
    return ENGLISH;
}
