#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T12:05:26
#
#-------------------------------------------------

QT       += core gui printsupport

#QMAKE_MACOSX_DEPLOYMENT_TARGET += 10.14.6

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Inventory
TEMPLATE = app


SOURCES += main.cpp dialog.cpp Inventory.cpp \
    Printer.cpp \
    controllers/MultiBuyController.cpp \
    controllers/MultiBuyElement.cpp \
    controllers/MultiSellController.cpp \
    controllers/MultiSellElement.cpp \
    models/History.cpp \
    models/Remaining.cpp \
    models/Return.cpp \
    models/Vegetable.cpp \
    models/returnto.cpp \
    translator.cpp \
    imenubar.cpp \
    ipreferencespanel.cpp \
    scrolleater.cpp \
    abbreviation.cpp

HEADERS  += dialog.h Vegetable.h Inventory.h History.h \
    Printer.h \
    Remaining.h \
    Return.h \
    returnto.h \
    IConst.h \
    translator.h \
    imenubar.h \
    ipreferencespanel.h \
    MultiSellElement.h \
    MultiSellController.h \
    MultiBuyController.h \
    MultiBuyElement.h \
    scrolleater.h \
    abbreviation.h


FORMS    += dialog.ui

#CONFIG-=app_bundle
