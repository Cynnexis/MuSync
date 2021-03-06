#-------------------------------------------------
#
# Project created by QtCreator 2018-12-21T18:06:52
#
#-------------------------------------------------

QT       += core gui concurrent network webenginewidgets svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Include O2 library
# (see https://github.com/pipacs/o2)
include($$PWD/lib/O2/src.pri)

TARGET = MuSync
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# The following define will call Visual Leak Detector. If you don't want
# it, please comment the line below.
#DEFINES += DETECT_MEMORY_LEAK

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    oauth/webapi.cpp \
    models/track.cpp \
    preferences.cpp \
    oauth\oauthdialog.cpp \
    models/artist.cpp \
    models/qartistlist.cpp \
    models/album.cpp \
    r.cpp \
    threading/autorefreshapi.cpp \
    ui/dsettings.cpp \
    ui/overlaywidget.cpp \
    ui/loadingoverlay.cpp \
    models/geniustrack.cpp \
    models/spotifytrack.cpp

HEADERS += \
        mainwindow.h \
    oauth/webapi.h \
    models/track.h \
    threading/barrierdata.h \
    threading/barrier.h \
    preferences.h \
    oauth\oauthdialog.h \
    threading/mutexdata.h \
    models/artist.h \
    models/qartistlist.h \
    models/album.h \
    r.h \
    threading/autorefreshapi.h \
    ui/dsettings.h \
    ui/overlaywidget.h \
    ui/loadingoverlay.h \
    models/geniustrack.h \
    models/spotifytrack.h

FORMS += \
        mainwindow.ui \
    oauth/oauthdialog.ui \
    ui/dsettings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    .gitignore \
    LICENSE \
    res/texts/.gitignore \
    res/texts/spotify-client-id.txt \
    res/texts/spotify-client-secret.txt \
    lib/.gitignore \
    res/texts/genius-client-id.txt \
    res/texts/genius-client-secret.txt

RESOURCES += \
    musync.qrc

INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
DEPENDPATH += "C:/Program Files/OpenSSL-Win64/include"

win32: LIBS += -L"$$PWD/lib/SSL/" -lopenssl -llibeay32 -llibssl -llibcrypto

# Add Qt-Frameless-Window-DarkStyle project

include($$PWD/lib/DarkStyle/frameless_window_dark.pri)

# Add Visual Leak Detector for Visual C++
INCLUDEPATH += "D:/Developer/Visual Leak Detector/include"
DEPENDPATH += "D:/Developer/Visual Leak Detector/include"
LIBS += -L"D:/Developer/Visual Leak Detector/lib/Win64"
