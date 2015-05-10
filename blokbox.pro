#-------------------------------------------------
#
# Project created by QtCreator 2013-10-16T09:52:16
#
#-------------------------------------------------

QT       += core gui multimedia opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = blokbox
TEMPLATE = app


SOURCES += main.cpp\
    spectrograph.cpp \
    fft.cpp \
    controls.cpp \
    fftcalc.cpp \
    mediainfo.cpp \
    medialibrary.cpp \
    playlistmodel.cpp \
    lightcycle.cpp \
    mainwindow.cpp
 
HEADERS  += \
    spectrograph.h \
    fft.h \
    controls.h \
    fftcalc.h \
    mediainfo.h \
    abstractcontrol.h \
    abstractspectrograph.h \
    abstractmediainfo.h \
    medialibrary.h \
    playlistmodel.h \
    lightcycle.h \
    mainwindow.h
   fft.h

FORMS    += \
    controls.ui \
    mediainfo.ui \
    medialibrary.ui \
    mainwindow.ui

RESOURCES += \
    resources.qrc


# I know linux has taglig support
#
# If you do not has taglib installed on your system, you should comment
# the following lines
linux{
  LIBS += -ltag
  DEFINES += USE_TAGLIB
}
# If you are brave, try the following link...
# http://joelverhagen.com/blog/2010/11/how-to-compile-taglib-on-windows/
