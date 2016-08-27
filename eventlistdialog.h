#ifndef EVENTLISTDIALOG_H
#define EVENTLISTDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class EventListDialog;
}

class EventListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EventListDialog(QWidget *parent = 0);
    void setModel(QStringListModel&);
    int getSelectedIndex();
    ~EventListDialog();

private:
    Ui::EventListDialog *ui;
};

#endif // EVENTLISTDIALOG_H
