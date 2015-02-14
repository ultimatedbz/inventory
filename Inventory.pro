#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T12:05:26
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Inventory
TEMPLATE = app


SOURCES += main.cpp dialog.cpp Vegetable.cpp Inventory.cpp History.cpp \
    Remaining.cpp \
    Return.cpp \
    returnto.cpp \
    translator.cpp \
    imenubar.cpp \
    returnswidget.cpp

HEADERS  += dialog.h Vegetable.h Inventory.h History.h \
    Remaining.h \
    Return.h \
    returnto.h \
    IConst.h \
    translator.h \
    imenubar.h \
    returnswidget.h


FORMS    += dialog.ui \
    returns.ui

#CONFIG-=app_bundle
