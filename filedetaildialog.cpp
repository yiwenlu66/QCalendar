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
#include <QMessageBox>

FileDetailDialog::FileDetailDialog(const QString &sha1, const QString &filename, int color, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDetailDialog),
    sha1(sha1),
    filename(filename)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    ui->label_filename->setText(filename);
    Color colorObject(this);
    for (int i = 0; i <= Color::MAX_COLOR; ++i) {
        ui->comboBox->addItem(Color::getIcon(i), colorObject.getName(i));
    }
    ui->comboBox->setCurrentIndex(color);
    ui->label_icon->setPixmap(QPixmap(":/icons/file.png"));
    ui->label_filename->setText(filename);
    ui->label_icon->installEventFilter(this);

    connect(ui->pushButton_delete, SIGNAL(clicked(bool)), this, SLOT(confirmDelete()));
}

int FileDetailDialog::color()
{
    return ui->comboBox->currentIndex();
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

void FileDetailDialog::confirmDelete()
{
    if (QMessageBox::warning(this, tr("Warning"), tr("Are you sure to delete?"), QMessageBox::Ok, QMessageBox::Cancel)
            == QMessageBox::Ok) {
        toDelete = true;
        accept();
    }
}


FileDetailDialog::~FileDetailDialog()
{
    delete ui;
}
