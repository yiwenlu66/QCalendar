######################################################################
# Automatically generated by qmake (3.0) Tue Aug 23 20:08:13 2016
######################################################################

QT += widgets
CONFIG += c++11
TEMPLATE = app
TARGET = QCalendar
INCLUDEPATH += .

# Input
HEADERS += calendar.h mainwindow.h \
    preferencedialog.h \
    preferencemanager.h \
    configloader.h
FORMS += mainwindow.ui \
    preferencedialog.ui
SOURCES += calendar.cpp main.cpp mainwindow.cpp \
    preferencedialog.cpp \
    preferencemanager.cpp \
    configloader.cpp
TRANSLATIONS += QCalendar_zh_CN.ts

RESOURCES += \
    res.qrc
