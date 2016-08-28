#include "eventlistdialog.h"
#include "color.h"
#include "ui_eventlistdialog.h"
#include <QAbstractItemView>
#include <QSize>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QPushButton>

const int ColoredItemDelegate::FONT_SIZE;
const int ColoredItemDelegate::PADDING_TOP;
const int ColoredItemDelegate::PADDING_DOWN;
const int ColoredItemDelegate::PADDING_LEFT;
const int ColoredItemDelegate::PADDING_RIGHT;
const int ColoredItemDelegate::MARGIN;

EventListDialog::EventListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventListDialog)
{
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

void EventListDialog::setModel(QStandardItemModel &model)
{
    ui->listView->setItemDelegate(new ColoredItemDelegate);
    ui->listView->setModel(&model);
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

int EventListDialog::getSelectedIndex()
{
    return ui->listView->selectionModel()->selectedIndexes()[0].row();
}

EventListDialog::~EventListDialog()
{
    delete ui;
}

ColoredItemDelegate::ColoredItemDelegate(QObject *parent)
{
}

QSize ColoredItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(0, PADDING_TOP + FONT_SIZE + PADDING_DOWN + MARGIN);
}

void ColoredItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString item = index.model()->data(index, Qt::DisplayRole).toString();
    int splitIndex = item.lastIndexOf(",");
    QString title = item.left(splitIndex);
    QColor color = Color::getColor(item.mid(splitIndex + 1).toInt());
    if (option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect.adjusted(0, 0, 0, -MARGIN), option.palette.highlight().color().darker());
    } else {
        painter->fillRect(option.rect.adjusted(0, 0, 0, -MARGIN), color);
    }
    painter->save();
    painter->translate(option.rect.x(), option.rect.y());
    painter->translate(PADDING_LEFT, PADDING_TOP + FONT_SIZE);
    painter->setPen(option.state & QStyle::State_Selected ? Qt::white : Qt::black);
    QFont font = QFont("Sans", FONT_SIZE);
    QFontMetrics metrics(font);
    painter->setFont(font);
    QString elidedTitle = metrics.elidedText(title, Qt::ElideRight, option.rect.width() - PADDING_LEFT - PADDING_RIGHT);
    painter->drawText(0, 0, elidedTitle);
    painter->restore();
}
