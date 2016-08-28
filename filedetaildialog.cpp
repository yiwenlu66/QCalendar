#include "filedetaildialog.h"
#include "color.h"
#include "ui_filedetaildialog.h"

#include <QPixmap>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QStandardPaths>
#include <QDir>
#include <QDateTime>

FileDetailDialog::FileDetailDialog(const QString &sha1, const QString &filename, int color, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDetailDialog),
    sha1(sha1),
    filename(filename)
{
    ui->setupUi(this);
    ui->label_filename->setText(filename);
    for (int i = 0; i <= Color::MAX_COLOR; ++i) {
        ui->comboBox->addItem(Color::getIcon(i), Color::getName(i));
    }
    ui->comboBox->setCurrentIndex(color);
    ui->label_icon->setPixmap(QPixmap(":/icons/file.png"));
    ui->label_filename->setText(filename);
    ui->label_icon->installEventFilter(this);
}

bool FileDetailDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        dragStartPos = mouseEvent->pos();
        return true;
    }
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if ((mouseEvent->pos() - dragStartPos).manhattanLength() < QApplication::startDragDistance()) {
            return false;
        }
        QDrag* drag = new QDrag(this);
        QMimeData* mime = new QMimeData;
        QList<QUrl> fileList;
        QString originalFilePath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + QDir::separator()
                + "files" + QDir::separator() + sha1;
        QString tmpPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + QDir::separator()
                + "QCalendar" + QDir::separator() + QString::number(QDateTime::currentMSecsSinceEpoch());
        QDir tmpDir = QDir(tmpPath);
        if (!tmpDir.exists()) {
            tmpDir.mkpath(tmpPath);
        }
        QString tmpFilePath = tmpPath + QDir::separator() + filename;
        QFile::copy(originalFilePath, tmpFilePath);
        fileList.append(QUrl::fromLocalFile(tmpFilePath));
        mime->setUrls(fileList);
        drag->setMimeData(mime);
        drag->exec(Qt::CopyAction, Qt::CopyAction);
        return true;
    }
}


FileDetailDialog::~FileDetailDialog()
{
    delete ui;
}
