#-------------------------------------------------
#
# Project created by QtCreator 2016-02-28T14:14:40
#
#-------------------------------------------------

QT       += core gui opengl sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpaceModel
TEMPLATE = app
LIBS += -lOpenGL32

SOURCES += main.cpp\
        mainwindow.cpp \
    scene.cpp \
    loader.cpp \
    spaceobject.cpp \
    planet.cpp \
    systemfactory.cpp \
    help.cpp \
    satelite.cpp \
    star.cpp \
    text2d.cpp

HEADERS  += mainwindow.h \
    scene.h \
    loader.h \
    spaceobject.h \
    planet.h \
    systemfactory.h \
    help.h \
    satelite.h \
    star.h \
    text2d.h \
    types.h

FORMS    += mainwindow.ui \
    help.ui

RESOURCES += \
    shaders.qrc
