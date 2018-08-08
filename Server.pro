#-------------------------------------------------
#
# Project created by QtCreator 2018-07-24T13:19:15
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
QT       += concurrent
QT       += charts

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
    sessioninfolist.cpp \
    chargedialog.cpp \
    chargeinfo.cpp \
    chargemanage.cpp \
    connection.cpp \
    count_dialog.cpp \
    login_mytcpsocket.cpp

FORMS += \
        mainwindow.ui \
    server_login_dialog.ui \
    showpicture.ui \
    chargedialog.ui \
    count_dialog.ui \

INCLUDEPATH += E:\Code\opencv\build\include
INCLUDEPATH += E:\Code\EasyPR\include
LIBS += E:\Code\opencv\build\x64\vc14\lib\*.lib

SUBDIRS += \
    Server.pro \
    Server.pro

DISTFILES += \
    Server.pro.user \
    Server.pro.user \
    DSCF2912.JPG \
    DSCF2912111.jpg \

RESOURCES += \
    pic.qrc \
    pic.qrc

QMAKE_CXXFLAGS+=-std=c++11

HEADERS += \
    chargedialog.h \
    chargeinfo.h \
    chargemanage.h \
    connection.h \
    count_dialog.h \
    handler.h \
    login_handler.h \
    login_mytcpsocket.h \
    login_mythread.h \
    login_tcpserver.h \
    mainwindow.h \
    receive_tcpserver.h \
    receive_tcpsession.h \
    receive_tcpthread.h \
    server_login_dialog.h \
    sessioninfo.h \
    sessioninfolist.h \
    sessionthreads.h \
    showpicture.h \
    user_detail.h


win32:CONFIG(release, debug|release): LIBS += -LE:/Code/opencv/build/x64/vc14/lib/ -lopencv_world320
else:win32:CONFIG(debug, debug|release): LIBS += -LE:/Code/opencv/build/x64/vc14/lib/ -lopencv_world320d

INCLUDEPATH += E:/Code/opencv/build/x64/vc14
DEPENDPATH += E:/Code/opencv/build/x64/vc14

win32:CONFIG(release, debug|release): LIBS += -LE:/Code/EasyPR/x64/release/ -llibeasypr
else:win32:CONFIG(debug, debug|release): LIBS += -LE:/Code/EasyPR/x64/debug/ -llibeasypr

INCLUDEPATH += E:/Code/EasyPR/x64/Debug
DEPENDPATH += E:/Code/EasyPR/x64/Debug

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += E:/Code/EasyPR/x64/release/liblibeasypr.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += E:/Code/EasyPR/x64/debug/liblibeasypr.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += E:/Code/EasyPR/x64/release/libeasypr.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += E:/Code/EasyPR/x64/debug/libeasypr.lib
