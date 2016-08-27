#include "configloader.h"
#include <QApplication>
#include <QtDebug>
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
    m_jsonFile = new QFile(dataDir.filePath(FILENAME));
    if (!m_jsonFile->open(QIODevice::ReadWrite)) {
        QMessageBox::critical(0, tr("Error"), tr("Cannot open config file!"));
        QApplication::quit();
    }
    QJsonDocument jsonDocument = QJsonDocument::fromJson(m_jsonFile->readAll());
    if (jsonDocument.isObject()) {
        QJsonObject jsonObject = jsonDocument.object();
        QJsonValue prefValue = jsonObject.value(KEY_PREF);
        if (prefValue.isObject()) {
            m_pref = new PreferenceManager(prefValue.toObject());
        }
        QJsonValue itemsValue = jsonObject.value(DataAdapter::KEY_ITEMS);
        QJsonValue datesValue = jsonObject.value(DataAdapter::KEY_DATES);
        if (itemsValue.isObject() && datesValue.isArray()) {
            QJsonObject dataObject;
            dataObject.insert(DataAdapter::KEY_ITEMS, itemsValue);
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
    connect(m_data, SIGNAL(updateData()), this, SLOT(configChanged()));
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
