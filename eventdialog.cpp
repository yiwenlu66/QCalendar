#include "eventdialog.h"
#include "ui_eventdialog.h"
#include <QtGlobal>
#include <QRegExp>
#include <QRegExpValidator>
#include <QIntValidator>
#include <QDate>

EventDialog::EventDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDialog)
{
    ui->setupUi(this);
    for (int i = 0; i <= (int)Qt::transparent; ++i) {
        ui->comboBox_color->addItem(QString::number(i));
    }

    // title must contain at least 1 non-space character
    ui->lineEdit_title->setValidator(new QRegExpValidator(*(new QRegExp("^(?!\\s*$).+")), this));
    ui->lineEdit_repeatCount->setValidator(new QIntValidator(1, 1000, this));
    ui->lineEdit_repeatInterval->setValidator(new QIntValidator(1, 1000, this));
    connect(ui->lineEdit_title, SIGNAL(textChanged(QString)), this, SLOT(checkInputsLegal()));
    connect(ui->lineEdit_repeatCount, SIGNAL(textChanged(QString)), this, SLOT(checkInputsLegal()));
    connect(ui->lineEdit_repeatInterval, SIGNAL(textChanged(QString)), this, SLOT(checkInputsLegal()));

    ui->lineEdit_repeatCount->setText(QString::number(1));
    ui->lineEdit_repeatInterval->setText(QString::number(1));
    connect(ui->comboBox_repeat, SIGNAL(currentIndexChanged(int)), this, SLOT(repeatModeChanged()));
    repeatModeChanged();
    checkInputsLegal();
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
    repeatModeSet();
    checkInputsLegal();
}

void EventDialog::repeatModeSet()
{
    if (ui->lineEdit_repeatInterval->text().toInt()) {
        showRepeatIntervalUi();
    } else {
        hideRepeatIntervalUi();
        ui->lineEdit_repeatInterval->setText(QString::number(1));
    }

    if (ui->lineEdit_repeatCount->text().toInt() >= 2) {
        showRepeatCountUi();
    } else {
        hideRepeatCountUi();
    }
}

void EventDialog::repeatModeChanged()
{
    switch (ui->comboBox_repeat->currentIndex()) {
    case 0:
        // once
        ui->lineEdit_repeatCount->setText(QString::number(1));
        hideRepeatIntervalUi();
        hideRepeatCountUi();
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        // daily, weekly, monthly, yearly
        hideRepeatIntervalUi();
        showRepeatCountUi();
        break;
    case 5:
        // custom
        showRepeatIntervalUi();
        showRepeatCountUi();
        break;
    }
}

void EventDialog::hideRepeatIntervalUi()
{
    ui->label_repeatInterval_1->hide();
    ui->label_repeatInterval_2->hide();
    ui->lineEdit_repeatInterval->hide();
}

void EventDialog::showRepeatIntervalUi()
{
    ui->label_repeatInterval_1->show();
    ui->label_repeatInterval_2->show();
    ui->lineEdit_repeatInterval->show();
}

void EventDialog::hideRepeatCountUi()
{
    ui->label_repeatCount_1->hide();
    ui->label_repeatCount_2->hide();
    ui->lineEdit_repeatCount->hide();
}

void EventDialog::showRepeatCountUi()
{
    ui->label_repeatCount_1->show();
    ui->label_repeatCount_2->show();
    ui->lineEdit_repeatCount->show();
}

void EventDialog::checkInputsLegal()
{
    if (ui->lineEdit_title->hasAcceptableInput() &&
            ui->lineEdit_repeatCount->hasAcceptableInput() &&
            ui->lineEdit_repeatInterval->hasAcceptableInput()) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

QString EventDialog::title()
{
    return ui->lineEdit_title->text();
}

QList<QDate> EventDialog::dates()
{
    QList<QDate> result;
    QDate date = ui->dateEdit->date();
    switch (ui->comboBox_repeat->currentIndex()) {
    case 0:
        // once
        result.append(date);
        break;
    case 1:
        // daily
        for (int i = 0; i < ui->lineEdit_repeatCount->text().toInt(); ++i) {
            result.append(date);
            date = date.addDays(1);
        }
        break;
    case 2:
        // weekly
        for (int i = 0; i < ui->lineEdit_repeatCount->text().toInt(); ++i) {
            result.append(date);
            date = date.addDays(7);
        }
        break;
    case 3:
        // monthly
        for (int i = 0; i < ui->lineEdit_repeatCount->text().toInt(); ++i) {
            result.append(date);
            date = date.addMonths(1);
        }
        break;
    case 4:
        // yearly
        for (int i = 0; i < ui->lineEdit_repeatCount->text().toInt(); ++i) {
            result.append(date);
            date = date.addYears(1);
        }
        break;
    case 5:
        // custom
        for (int i = 0; i < ui->lineEdit_repeatCount->text().toInt(); ++i) {
            result.append(date);
            date = date.addDays(ui->lineEdit_repeatInterval->text().toInt());
        }
        break;
    }
    return result;
}

QTime EventDialog::startTime()
{
    return ui->timeEdit_start->time();
}

QTime EventDialog::endTime()
{
    return ui->timeEdit_end->time();
}

Qt::GlobalColor EventDialog::color()
{
    return (Qt::GlobalColor)ui->comboBox_color->currentIndex();
}

QString EventDialog::location()
{
    return ui->lineEdit_location->text();
}

QString EventDialog::comments()
{
    return ui->textEdit_comments->toPlainText();
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
