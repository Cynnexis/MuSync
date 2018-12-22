#-------------------------------------------------
#
# Project created by QtCreator 2018-12-21T18:06:52
#
#-------------------------------------------------

QT       += core gui network

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

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    oauth/webapi.cpp

HEADERS += \
        mainwindow.h \
    oauth/webapi.h

FORMS += \
        mainwindow.ui

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
    lib/.gitignore

RESOURCES += \
    musync.qrc

win32:CONFIG(release, debug|release): LIBS += -L"C:/Program Files/OpenSSL-Win64/lib/VC" -lssl-1_1 -llibcrypto
else:win32:CONFIG(debug, debug|release): LIBS += -L"C:/Program Files/OpenSSL-Win64/lib/VC" -lssl-1_1d -llibcryptod
else:unix: LIBS += -L"C:/Program Files/OpenSSL-Win64/lib/VC" -lssl-1_1 -llibcrypto

INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"
DEPENDPATH += "C:/Program Files/OpenSSL-Win64/include"
