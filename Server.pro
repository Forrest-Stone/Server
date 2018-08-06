#-------------------------------------------------
#
# Project created by QtCreator 2018-07-24T13:19:15
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
QT       += concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    sessionthreads.cpp \
    receive_tcpserver.cpp \
    receive_tcpthread.cpp \
    receive_tcpsession.cpp \
    handler.cpp \
    login_handler.cpp \
    login_mythread.cpp \
    login_tcpserver.cpp \
    server_login_dialog.cpp \
    showpicture.cpp \
    sessioninfo.cpp \
    sessioninfolist.cpp

HEADERS += \
        mainwindow.h \
    sessionthreads.h \
    receive_tcpserver.h \
    receive_tcpthread.h \
    receive_tcpsession.h \
    connection.h \
    handler.h \
    login_handler.h \
    login_mythread.h \
    login_tcpserver.h \
    user_detail.h \
    server_login_dialog.h \
    showpicture.h \
    sessioninfo.h \
    sessioninfolist.h

FORMS += \
        mainwindow.ui \
    server_login_dialog.ui \
    showpicture.ui


SUBDIRS += \
    Server.pro

DISTFILES += \
    Server.pro.user

RESOURCES += \
    pic.qrc


INCLUDEPATH += F:\opencv-3.2\opencv\build\include
INCLUDEPATH += F:\QT\EasyPR\include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../opencv-3.2/opencv/build/x64/vc14/lib/ -lopencv_world320
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../opencv-3.2/opencv/build/x64/vc14/lib/ -lopencv_world320d

INCLUDEPATH += $$PWD/../../opencv-3.2/opencv/build/x64/vc14
DEPENDPATH += $$PWD/../../opencv-3.2/opencv/build/x64/vc14

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../EasyPR/x64/release/ -llibeasypr
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../EasyPR/x64/debug/ -llibeasypr

INCLUDEPATH += $$PWD/../EasyPR/x64/Debug
DEPENDPATH += $$PWD/../EasyPR/x64/Debug

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../EasyPR/x64/release/liblibeasypr.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../EasyPR/x64/debug/liblibeasypr.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../EasyPR/x64/release/libeasypr.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../EasyPR/x64/debug/libeasypr.lib

