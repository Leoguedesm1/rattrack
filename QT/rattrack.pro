#-------------------------------------------------
#
# Project created by QtCreator 2017-01-07T16:14:46
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rattrack
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    controlpainel.cpp \
    dialogconfig.cpp \
    dialogsave.cpp \
    dialogfindcircle.cpp

HEADERS  += mainwindow.h \
    controlpainel.h \
    dialogconfig.h \
    dialogsave.h \
    dialogfindcircle.h

FORMS    += mainwindow.ui \
    controlpainel.ui \
    dialogconfig.ui \
    dialogsave.ui \
    dialogfindcircle.ui

INCLUDEPATH += /usr/local/include/opencv
LIBS += `pkg-config opencv --libs`

DISTFILES += \
    font-awesome-4.7.0/fonts/fontawesome-webfont.eot \
    font-awesome-4.7.0/fonts/fontawesome-webfont.woff \
    font-awesome-4.7.0/fonts/fontawesome-webfont.woff2 \
    font-awesome-4.7.0/fonts/FontAwesome.otf \
    font-awesome-4.7.0/fonts/fontawesome-webfont.ttf \
    font-awesome-4.7.0/fonts/fontawesome-webfont.svg \
    font-awesome-4.7.0/css/font-awesome.css \
    font-awesome-4.7.0/css/font-awesome.min.css \
    font-awesome-4.7.0/less/animated.less \
    font-awesome-4.7.0/less/bordered-pulled.less \
    font-awesome-4.7.0/less/core.less \
    font-awesome-4.7.0/less/fixed-width.less \
    font-awesome-4.7.0/less/font-awesome.less \
    font-awesome-4.7.0/less/icons.less \
    font-awesome-4.7.0/less/larger.less \
    font-awesome-4.7.0/less/list.less \
    font-awesome-4.7.0/less/mixins.less \
    font-awesome-4.7.0/less/path.less \
    font-awesome-4.7.0/less/rotated-flipped.less \
    font-awesome-4.7.0/less/screen-reader.less \
    font-awesome-4.7.0/less/stacked.less \
    font-awesome-4.7.0/less/variables.less \
    font-awesome-4.7.0/scss/_animated.scss \
    font-awesome-4.7.0/scss/_bordered-pulled.scss \
    font-awesome-4.7.0/scss/_core.scss \
    font-awesome-4.7.0/scss/_fixed-width.scss \
    font-awesome-4.7.0/scss/_icons.scss \
    font-awesome-4.7.0/scss/_larger.scss \
    font-awesome-4.7.0/scss/_list.scss \
    font-awesome-4.7.0/scss/_mixins.scss \
    font-awesome-4.7.0/scss/_path.scss \
    font-awesome-4.7.0/scss/_rotated-flipped.scss \
    font-awesome-4.7.0/scss/_screen-reader.scss \
    font-awesome-4.7.0/scss/_stacked.scss \
    font-awesome-4.7.0/scss/_variables.scss \
    font-awesome-4.7.0/scss/font-awesome.scss \
    font-awesome-4.7.0/HELP-US-OUT.txt \
    1484781976_ic_play_arrow_48px.png \
    1484782138_settings.png \
    pause.png \
    play.png \
    config.png \
    reset.png \
    config.ico \
    pause.ico \
    play.ico \
    reset.ico

RESOURCES += \
    icons.qrc
