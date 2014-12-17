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
        mainwindow.cpp \
    spectrograph.cpp \
    fft.cpp \
    controls.cpp \
    fftcalc.cpp \
    mediainfo.cpp \
    medialibrary.cpp \
    playlistmodel.cpp \
    lightcycle.cpp \
    interfacedesigner.cpp \
    interfacecreatordialog.cpp
 
HEADERS  += mainwindow.h \
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
    interfacedesigner.h \
    interfacecreatordialog.h
   fft.h

FORMS    += mainwindow.ui \
    controls.ui \
    mediainfo.ui \
    medialibrary.ui \
    interfacecreatordialog.ui

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
# If you are brave on windows, you should try the following link...
# http://joelverhagen.com/blog/2010/11/how-to-compile-taglib-on-windows/
