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
}

QJsonObject PreferenceManager::toJson()
{
    QJsonObject result;
    result.insert("language", QJsonValue(language));
    return result;
}

PreferenceManager::Language PreferenceManager::getDefaultLanguage()
{
    if (QLocale::system() == QLocale::Chinese) {
        return CHINESE;
    }
    return ENGLISH;
}
