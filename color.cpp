#include "color.h"
#include <QPixmap>

Color::Color(QObject* parent) :
    QObject(parent)
{

}

QColor Color::getColor(int i)
{
    switch(i) {
    case 0:
        return QColor(208, 140, 224);
    case 1:
        return QColor(113, 202, 88);
    case 2:
        return QColor(247, 166, 80);
    case 3:
        return QColor(244, 205, 86);
    case 4:
        return QColor(253, 100, 97);
    case 5:
        return QColor(81, 186, 242);
    case 6:
        return QColor(165, 165, 167);
    default:
        return QColor(255, 255, 255);
    }
}

QString Color::getName(int i)
{
    switch(i) {
    case 0:
        return tr("Purple");
    case 1:
        return tr("Green");
    case 2:
        return tr("Orange");
    case 3:
        return tr("Yellow");
    case 4:
        return tr("Red");
    case 5:
        return tr("Blue");
    case 6:
        return tr("Gray");
    default:
        return tr("White");
    }
}

QIcon Color::getIcon(int i, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(getColor(i));
    return QIcon(pixmap);
}
