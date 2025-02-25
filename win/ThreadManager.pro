#-------------------------------------------------
#
# Project created by QtCreator 2018-12-01T16:46:59
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

TARGET = appmemory_win
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    filtermemory.cpp \
        main.cpp \
        mainwindow.cpp \
    processestablemodel.cpp \
    threadstablemodel.cpp \
    pidfilterproxymodel.cpp \
    updater.cpp

HEADERS += \
    filtermemory.h \
        mainwindow.h \
    processestablemodel.h \
    threadstablemodel.h \
    pidfilterproxymodel.h \
    updater.h

FORMS += \
        mainwindow.ui

LIBS += -lpsapi
