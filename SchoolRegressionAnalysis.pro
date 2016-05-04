#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T17:24:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SchoolRegressionAnalysis
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    qcustomplot.cpp \
    explorerwindow.cpp \
    tablewindow.cpp

HEADERS += mainwindow.h \
    qcustomplot.h \
    explorerwindow.h \
    tablewindow.h

FORMS += mainwindow.ui \
    explorerwindow.ui \
    tablewindow.ui
