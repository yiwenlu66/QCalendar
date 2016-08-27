#ifndef COLOR_H
#define COLOR_H

#include <QColor>
#include <QString>

class Color
{
public:
    Color();
    static QColor getColor(int);
    static QString getName(int);
    const static int MAX_COLOR = 6;
};

#endif // COLOR_H
