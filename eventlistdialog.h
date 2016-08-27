#ifndef EVENTLISTDIALOG_H
#define EVENTLISTDIALOG_H

#include <QDialog>
#include <QAbstractItemDelegate>
#include <QStandardItemModel>
#include <QSize>

namespace Ui {
class EventListDialog;
}

class EventListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EventListDialog(QWidget *parent = 0);
    void setModel(QStandardItemModel&);
    int getSelectedIndex();
    ~EventListDialog();

private:
    Ui::EventListDialog *ui;
};

class ColoredItemDelegate: public QAbstractItemDelegate
{
public:
    ColoredItemDelegate(QObject* parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    static const int FONT_SIZE = 12;
    static const int PADDING_TOP = 2;
    static const int PADDING_DOWN = 2;
    static const int PADDING_LEFT = 3;
    static const int PADDING_RIGHT = 3;
    static const int MARGIN = 1;
};


#endif // EVENTLISTDIALOG_H
