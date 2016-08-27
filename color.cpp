#include "color.h"

Color::Color()
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
        return "Purple";
    case 1:
        return "Green";
    case 2:
        return "Orange";
    case 3:
        return "Yellow";
    case 4:
        return "Red";
    case 5:
        return "Blue";
    case 6:
        return "Gray";
    default:
        return "White";
    }
}
