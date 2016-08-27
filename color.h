#ifndef COLOR_H
#define COLOR_H

#include <QColor>
#include <QString>
#include <QIcon>

class Color
{
public:
    Color();
    static QColor getColor(int);
    static QString getName(int);
    static QIcon getIcon(int i, int size = 12);
    const static int MAX_COLOR = 6;
};

#endif // COLOR_H
