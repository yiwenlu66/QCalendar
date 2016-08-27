#include "eventlistdialog.h"
#include "ui_eventlistdialog.h"
#include <QAbstractItemView>

EventListDialog::EventListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventListDialog)
{
    ui->setupUi(this);
}

void EventListDialog::setModel(QStringListModel &model)
{
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
