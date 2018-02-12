#-------------------------------------------------
#
# Project created by QtCreator 2017-11-15T17:38:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Model
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    video.cpp \
    trackergui.cpp \
    tracker.cpp \
    calibrationgui.cpp \
    calibration.cpp

HEADERS  += mainwindow.h \
    readerinterface.h \
    readerxml.h \
    video.h \
    trackergui.h \
    tracker.h \
    detectorinterface.h \
    threshold.h \
    writerinterface.h \
    writercsv.h \
    directorycreator.h \
    homographyfileconstants.h \
    filesanddirectoriesconstants.h \
    quadrantsfileconstants.h \
    fastmodeguiconstants.h \
    normalmodeguiconstants.h \
    calibrationgui.h \
    writerxml.h \
    calibration.h \
    settingsfileconstants.h

FORMS    += mainwindow.ui \
    trackergui.ui \
    calibrationgui.ui

INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config opencv --libs`

RESOURCES += \
    icons.qrc
