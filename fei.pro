#-------------------------------------------------
#
# Project created by QtCreator 2011-12-02T19:59:30
#
#-------------------------------------------------

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += debug
TARGET = fei
TEMPLATE = app
SOURCES += main.cpp\
    chatform.cpp \
    mainform.cpp \
    useritem.cpp \
    feiproto.cpp \
    global.cpp \
    rsfileitem.cpp \
    rsfilemodel.cpp \
    rsfilesocket.cpp \
    logindialog.cpp

HEADERS  += \
    chatform.h \
    mainform.h \
    useritem.h \
    feiproto.h \
    platform.h \
    ipmsg.h \
    global.h \
    rsfileitem.h \
    rsfilemodel.h \
    rsfilesocket.h \
    logindialog.h
FORMS    +=   mainform.ui \
    chatform.ui \
    logindialog.ui
#windows
win32 {
    SOURCES +=    platform_win.cpp
}
#linux
unix:!macx {
    SOURCES += platform_linux.cpp
}
#mac
macx {
    SOURCES += platform_mac.cpp
}
contains(QT_CONFIG,embedded) {
LIBS += -L$(HISISDK)/lib -lhi_common
LIBS += -L$(HISISDK)directfb4/lib -lpng14 -ljpeg -lz -ltde
}

RESOURCES += \
    image.qrc





































