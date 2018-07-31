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
    main.cpp \
    mainwindow.cpp \
    receive_tcpserver.cpp \
    receive_tcpsession.cpp \
    receive_tcpthread.cpp \
    server_login_dialog.cpp \
    sessionthreads.cpp \
    handler.cpp \
    login_handler.cpp \
    login_mythread.cpp \
    login_tcpserver.cpp \
    main.cpp \
    mainwindow.cpp \
    receive_tcpserver.cpp \
    receive_tcpsession.cpp \
    receive_tcpthread.cpp \
    server_login_dialog.cpp \
    sessionthreads.cpp

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
    mainwindow.h \
    receive_tcpserver.h \
    receive_tcpsession.h \
    receive_tcpthread.h \
    server_login_dialog.h \
    sessionthreads.h \
    user_detail.h \
    connection.h \
    handler.h \
    login_handler.h \
    login_mythread.h \
    login_tcpserver.h \
    mainwindow.h \
    receive_tcpserver.h \
    receive_tcpsession.h \
    receive_tcpthread.h \
    server_login_dialog.h \
    sessionthreads.h \
    user_detail.h

FORMS += \
        mainwindow.ui \
    mainwindow.ui \
    server_login_dialog.ui \
    mainwindow.ui \
    server_login_dialog.ui

INCLUDEPATH += E:\Code\opencv\opencv\build\include
LIBS += E:\Code\opencv\opencv\build\x64\vc15\lib\*.lib

SUBDIRS += \
    Server.pro \
    Server.pro

DISTFILES += \
    Server.pro.user \
    Server.pro.user
