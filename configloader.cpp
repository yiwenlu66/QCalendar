#include "configloader.h"
#include <QApplication>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonValue>
#include <QMessageBox>

constexpr char ConfigLoader::KEY_PREF[];
constexpr char ConfigLoader::FILENAME[];

ConfigLoader::ConfigLoader(QObject *parent) :
    QObject(parent),
    m_pref(nullptr),
    m_data(nullptr)
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dataDir = QDir(dataPath);
    if (!dataDir.exists()) {
        dataDir.mkpath(dataPath);
    }
    m_jsonFilePath = dataDir.filePath(FILENAME);
    m_jsonFile = new QFile(m_jsonFilePath);
    if (!m_jsonFile->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(0, tr("Error"), tr("Cannot open config file!"));
        QApplication::quit();
    }
    readJson(m_jsonFile->readAll());
    connect(m_data, SIGNAL(updateData()), this, SLOT(configChanged()));
}

void ConfigLoader::readJson(const QByteArray &json)
{
    if (m_pref != nullptr) {
        delete m_pref;
    }
    if (m_data != nullptr) {
        delete m_data;
    }
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json);
    if (jsonDocument.isObject()) {
        QJsonObject jsonObject = jsonDocument.object();
        QJsonValue prefValue = jsonObject.value(KEY_PREF);
        if (prefValue.isObject()) {
            m_pref = new PreferenceManager(prefValue.toObject());
        }
        QJsonValue eventsValue = jsonObject.value(DataAdapter::KEY_EVENTS);
        QJsonValue filesValue = jsonObject.value(DataAdapter::KEY_FILES);
        QJsonValue datesValue = jsonObject.value(DataAdapter::KEY_DATES);
        if (eventsValue.isObject() && filesValue.isObject() && datesValue.isArray()) {
            QJsonObject dataObject;
            dataObject.insert(DataAdapter::KEY_EVENTS, eventsValue);
            dataObject.insert(DataAdapter::KEY_FILES, filesValue);
            dataObject.insert(DataAdapter::KEY_DATES, datesValue);
            m_data = new DataAdapter(dataObject);
        }
    }
    if (m_pref == nullptr) {
        m_pref = new PreferenceManager;
    }
    if (m_data == nullptr) {
        m_data = new DataAdapter;
    }
    configChanged();
}

void ConfigLoader::configChanged()
{
    QJsonObject jsonObject;
    jsonObject = m_data->toJson();
    jsonObject.insert(KEY_PREF, m_pref->toJson());
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
