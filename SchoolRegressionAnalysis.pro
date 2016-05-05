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
    tablewindow.cpp \
    selectdatasource.cpp

HEADERS += mainwindow.h \
    qcustomplot.h \
    tablewindow.h \
    selectdatasource.h

FORMS += mainwindow.ui \
    tablewindow.ui \
    selectdatasource.ui
