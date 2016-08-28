#ifndef FILEDETAILDIALOG_H
#define FILEDETAILDIALOG_H

#include <QDialog>
#include <QPoint>

namespace Ui {
class FileDetailDialog;
}

class FileDetailDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileDetailDialog(const QString& sha1, const QString& filename, int color, QWidget *parent = 0);
    bool toDelete = false;
    int color();
    ~FileDetailDialog();

public slots:
    void confirmDelete();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::FileDetailDialog *ui;

    QPoint dragStartPos;
    QString sha1;
    QString filename;
};

#endif // FILEDETAILDIALOG_H
