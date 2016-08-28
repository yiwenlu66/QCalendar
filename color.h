#ifndef COLOR_H
#define COLOR_H

#include <QColor>
#include <QString>
#include <QIcon>
#include <QObject>

class Color: public QObject
{
    Q_OBJECT

public:
    Color(QObject* parent = 0);
    static QColor getColor(int);
    QString getName(int);
    static QIcon getIcon(int i, int size = 12);
    const static int MAX_COLOR = 6;
};

#endif // COLOR_H
