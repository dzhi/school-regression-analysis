#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T17:24:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SchoolRegressionAnalysis
TEMPLATE = app


CONFIG(release, debug|release) {
    win32 {
        QMAKE_POST_LINK += ../school-regression-analysis/winbuild.sh
    }
    unix {
        QMAKE_POST_LINK += ../school-regression-analysis/macbuild.sh
    }
}


SOURCES += main.cpp\
    qcustomplot.cpp \
    selectdatasource.cpp \
    tablewindow.cpp \
    alglib/alglibinternal.cpp \
    alglib/alglibmisc.cpp \
    alglib/ap.cpp \
    alglib/dataanalysis.cpp \
    alglib/diffequations.cpp \
    alglib/fasttransforms.cpp \
    alglib/integration.cpp \
    alglib/interpolation.cpp \
    alglib/linalg.cpp \
    alglib/optimization.cpp \
    alglib/solvers.cpp \
    alglib/specialfunctions.cpp \
    alglib/statistics.cpp \
    csvparser.cpp \
    qcpdocumentobject.cpp \
    matrix.cpp

HEADERS  += \
    qcustomplot.h \
    selectdatasource.h \
    tablewindow.h \
    alglib/alglibinternal.h \
    alglib/alglibmisc.h \
    alglib/ap.h \
    alglib/dataanalysis.h \
    alglib/diffequations.h \
    alglib/fasttransforms.h \
    alglib/integration.h \
    alglib/interpolation.h \
    alglib/linalg.h \
    alglib/optimization.h \
    alglib/solvers.h \
    alglib/specialfunctions.h \
    alglib/statistics.h \
    alglib/stdafx.h \
    csvparser.h \
    qcpdocumentobject.h \
    matrix.h

FORMS    += \
    selectdatasource.ui \
    tablewindow.ui \
    matrix.ui
