#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T16:30:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileMenedger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    recursiv.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    recursiv.h

FORMS    += mainwindow.ui \
    dialog.ui
