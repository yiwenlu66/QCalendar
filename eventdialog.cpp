#include "eventdialog.h"
#include "ui_eventdialog.h"
#include <QtGlobal>

EventDialog::EventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);
    for (int i = 0; i <= (int)Qt::transparent; ++i) {
        ui->comboBox_color->addItem(QString::number(i));
    }
    ui->lineEdit_repeatCount->setText(QString::number(1));
    ui->lineEdit_repeatInterval->setText(QString::number(0));
    connect(ui->comboBox_repeat, SIGNAL(currentIndexChanged(int)), this, SLOT(repeatModeChanged()));
    repeatModeChanged();
}

EventDialog::EventDialog(const QDate &date, QWidget *parent) :
    EventDialog(parent)
{
    ui->dateEdit->setDate(date);
    ui->timeEdit_start->setTime(QTime(8, 0));
    ui->timeEdit_end->setTime(QTime(9, 0));
}

EventDialog::EventDialog(const CalendarEvent& event, QWidget *parent) :
    EventDialog(parent)
{
    ui->lineEdit_title->setText(event.title);
    ui->dateEdit->setDate(event.dates[0]);
    ui->timeEdit_start->setTime(event.startTime);
    ui->timeEdit_end->setTime(event.endTime);
    ui->comboBox_color->setCurrentIndex(event.color);
    ui->lineEdit_location->setText(event.location);
    ui->textEdit_comments->setText(event.comments);

    RepeatMode repeatMode = getRepeatMode(event.dates);
    int repeatInterval = (repeatMode == CUSTOM_EVEN) ? event.dates[0].daysTo(event.dates[1]) : 0;
    int repeatModeIndex;
    switch (repeatMode) {
    case ONCE:
        repeatModeIndex = 0;
        break;
    case DAILY:
        repeatModeIndex = 1;
        break;
    case WEEKLY:
        repeatModeIndex = 2;
        break;
    case MONTHLY:
        repeatModeIndex = 3;
        break;
    case YEARLY:
        repeatModeIndex = 4;
        break;
    default:
        repeatModeIndex = 5;
    }
    // do not show repeat count in uneven custom mode
    int repeatCount = (repeatMode == CUSTOM_UNEVEN) ? 0 : event.dates.size();

    ui->comboBox_repeat->setCurrentIndex(repeatModeIndex);
    ui->lineEdit_repeatInterval->setText(QString::number(repeatInterval));
    ui->lineEdit_repeatCount->setText(QString::number(repeatCount));
    repeatModeChanged();
}

void EventDialog::repeatModeChanged()
{
    if (ui->lineEdit_repeatInterval->text().toInt()) {
        ui->label_repeatInterval_1->show();
        ui->label_repeatInterval_2->show();
        ui->lineEdit_repeatInterval->show();
    } else {
        ui->label_repeatInterval_1->hide();
        ui->label_repeatInterval_2->hide();
        ui->lineEdit_repeatInterval->hide();
    }

    if (ui->lineEdit_repeatCount->text().toInt() >= 2) {
        ui->label_repeatCount_1->show();
        ui->label_repeatCount_2->show();
        ui->lineEdit_repeatCount->show();
    } else {
        ui->label_repeatCount_1->hide();
        ui->label_repeatCount_2->hide();
        ui->lineEdit_repeatCount->hide();
    }
}


EventDialog::RepeatMode EventDialog::getRepeatMode(const QList<QDate> &dateList)
{
    if (dateList.isEmpty()) {
        return NONE;
    }

    if (dateList.size() == 1) {
        return ONCE;
    }

    int i;
    bool legal;

    for (i = 0, legal = true; i < dateList.size() - 1 && legal; ++i) {
        if (dateList[i].addDays(1) != dateList[i + 1]) {
            legal = false;
        }
    }
    if (legal) {
        return DAILY;
    }

    for (i = 0, legal = true; i < dateList.size() - 1 && legal; ++i) {
        if (dateList[i].addDays(7) != dateList[i + 1]) {
            legal = false;
        }
    }
    if (legal) {
        return WEEKLY;
    }

    for (i = 0, legal = true; i < dateList.size() - 1 && legal; ++i) {
        if (dateList[i].addMonths(1) != dateList[i + 1]) {
            legal = false;
        }
    }
    if (legal) {
        return MONTHLY;
    }

    for (i = 0, legal = true; i < dateList.size() - 1 && legal; ++i) {
        if (dateList[i].addYears(1) != dateList[i + 1]) {
            legal = false;
        }
    }
    if (legal) {
        return YEARLY;
    }

    qint64 interval = dateList[0].daysTo(dateList[1]);
    for (i = 1, legal = true; i < dateList.size() - 1 && legal; ++i) {
        if (dateList[i].daysTo(dateList[i + 1]) != interval) {
            legal = false;
        }
    }
    if (legal) {
        return CUSTOM_EVEN;
    }
    return CUSTOM_UNEVEN;
}

EventDialog::~EventDialog()
{
    delete ui;
}
