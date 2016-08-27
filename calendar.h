#ifndef CALENDAR_H
#define CALENDAR_H

#include <QCalendarWidget>
#include <QPainter>
#include <QLocale>
#include "dataadapter.h"
#include "eventdialog.h"

class Calendar : public QCalendarWidget
{
    Q_OBJECT

public:
    Calendar(QWidget* parent = Q_NULLPTR);
    void setDataAdapter(DataAdapter* dataAdapter);

public slots:
    void startOfWeekChanged(Qt::DayOfWeek);
    void doubleClicked(int x, int y);
    void cellsResized();

private slots:
    void loadMonthEventList();

    // dirty hack: disable double click action for a very short period of time after month changes to avoid over-sensitive actions
    void freezeDoubleClick();
    void showEventList(const QStringList& sha1List);
    void showEventDialog(const QString& sha1);
    void showEventDialog(const QDate& date);    // new event

protected:
    void paintCell(QPainter * painter, const QRect & rect, const QDate & date) const;

private:
    static const int FONTSIZE_DAYOFMONTH = 16;
    static const int FONTSIZE_ITEMTITLE = 12;
    static const int MARGIN_BELOW_DAY = 4;
    static const int MARGIN_BETWEEN_TILES = 2;
    static const int MARGIN_TILE_SIDE = 2;
    static const int PADDING_TOP = 2;
    static const int PADDING_BOTTOM = 2;
    static const int PADDING_LEFT = 2;
    static const int PADDING_RIGHT = 2;
    static constexpr char const COLOR_TODAY[] = "#FFF8DC";
    static const int FREEZE_PERIOD = 500;
    bool doubleClickFreezed = false;
    mutable QList<int> xPivots, yPivots, cellWidths, cellHeights;    // record starting coordinates and metrics of all cells

    /*
     * tell which tile in the cell the position belongs to; x and y are coordinates *within* the cell;
     * return -1 if the position belongs to a blank area.
     */
    int getTileIndex(int x, int y, int cellWidth, int cellHeight);

    void execEventDialog(EventDialog& dlg, QString sha1 = "");

    DataAdapter* m_dataAdapter;
    QList<QStringList> m_monthEventList;
};

#endif // CALENDAR_H
