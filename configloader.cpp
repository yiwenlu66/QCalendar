#include "configloader.h"
#include <QApplication>
#include <QtDebug>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMessageBox>

ConfigLoader::ConfigLoader(QObject *parent) :
    QObject(parent),
    m_pref(nullptr)
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dataDir = QDir(dataPath);
    if (!dataDir.exists()) {
        dataDir.mkpath(dataPath);
    }
    m_jsonFile = new QFile(dataDir.filePath("config.json"));
    if (!m_jsonFile->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(0, tr("Error"), tr("Cannot open config file!"));
        QApplication::quit();
    }
    QJsonDocument jsonDocument = QJsonDocument::fromJson(m_jsonFile->readAll());
    if (jsonDocument.isObject()) {
        QJsonObject jsonObject = jsonDocument.object();
        QJsonValue prefValue = jsonObject.value("preferences");
        if (prefValue.isObject()) {
            m_pref = new PreferenceManager(prefValue.toObject());
        }
    }
    if (m_pref == nullptr) {
        m_pref = new PreferenceManager;
    }
}

void ConfigLoader::configChanged()
{
    QJsonObject jsonObject;
    jsonObject.insert("preferences", m_pref->toJson());
    QJsonDocument jsonDocument(jsonObject);
    if (m_jsonFile->isOpen()) {
        m_jsonFile->close();
        m_jsonFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    }
    m_jsonFile->write(jsonDocument.toJson());
    m_jsonFile->flush();
}

ConfigLoader::~ConfigLoader()
{
    delete m_pref;
    m_jsonFile->close();
    delete m_jsonFile;
}
