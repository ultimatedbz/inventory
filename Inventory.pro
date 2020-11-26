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


SOURCES += main.cpp Inventory.cpp \
    Dialog/dialog+buy.cpp \
    Dialog/dialog+print.cpp \
    Dialog/dialog+search.cpp \
    Dialog/dialog+sell.cpp \
    Dialog/dialog.cpp \
    SearchResultsController.cpp \
    collapsiblesection.cpp \
    controllers/MultiBuyController.cpp \
    controllers/MultiBuyElement.cpp \
    controllers/MultiSellController.cpp \
    controllers/MultiSellElement.cpp \
    models/History.cpp \
    models/Remaining.cpp \
    models/Return.cpp \
    models/Vegetable.cpp \
    models/returnto.cpp \
    searchhistorylist.cpp \
    translator.cpp \
    imenubar.cpp \
    ipreferencespanel.cpp \
    scrolleater.cpp \
    abbreviation.cpp \
    utils.cpp

HEADERS  += dialog.h Vegetable.h Inventory.h History.h \
    Remaining.h \
    Return.h \
    SearchResultsController.h \
    collapsiblesection.h \
    returnto.h \
    IConst.h \
    searchhistorylist.h \
    translator.h \
    imenubar.h \
    ipreferencespanel.h \
    MultiSellElement.h \
    MultiSellController.h \
    MultiBuyController.h \
    MultiBuyElement.h \
    scrolleater.h \
    abbreviation.h \
    utils.h


FORMS    += dialog.ui

#CONFIG-=app_bundle
