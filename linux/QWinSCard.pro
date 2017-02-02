#-------------------------------------------------
#
# Project created by QtCreator 2016-11-02T15:18:41
#
#-------------------------------------------------

QT       -= gui

TARGET = QWinSCard
TEMPLATE = lib
CONFIG += staticlib

DEFINES += QWINSCARD_LIBRARY

SOURCES += \
    APDUCommand.cpp \
    APDUResponse.cpp \
    baseexceptions.cpp \
    cardevent.cpp \
    NativeSCard.cpp \
    scardexception.cpp

HEADERS +=\
    APDUCommand.h \
    APDUResponse.h \
    baseexceptions.h \
    cardevent.h \
    NativeSCard.h \
    scardexception.h \
    winscarderr.h \
    stdafx.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32: LIBS += -lwinscard
