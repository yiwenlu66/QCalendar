#ifndef CONFIGLOADER_H
#define CONFIGLOADER_H

#include <QObject>
#include <QFile>
#include "preferencemanager.h"
#include "dataadapter.h"

class ConfigLoader : public QObject
{
    Q_OBJECT
public:
    static constexpr char const FILENAME[] = "config.json";	// TODO: support multiple users
    explicit ConfigLoader(QObject *parent = 0);
    PreferenceManager* pref() {
        return m_pref;
    }
    DataAdapter* data() {
        return m_data;
    }
    QString path() const {
        return m_jsonFilePath;
    }
    void readJson(const QByteArray& json);

    ~ConfigLoader();

public slots:
    void configChanged();

private:
    static constexpr char const KEY_PREF[] = "preferences";

    PreferenceManager* m_pref;
    DataAdapter* m_data;
    QFile* m_jsonFile;
    QString m_jsonFilePath;
};

#endif // CONFIGLOADER_H
