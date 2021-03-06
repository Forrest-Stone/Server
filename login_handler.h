﻿#ifndef LOGIN_HANDLER_H
#define LOGIN_HANDLER_H
#include "handler.h"
#include <unordered_map>
#include <string>
#include <QObject>
#include <mutex>
#include "user_detail.h"
#include "mainwindow.h"
using namespace std;
extern unordered_map<unsigned int,User_Detail> user_map;
class login_handler :public QObject,public handler
{
    Q_OBJECT
public:
    login_handler(QObject*parent);
    void operator ()(QTcpSocket*);
    static int cnt;
    //QObject*parent;
    unordered_map<unsigned int,string> temp_data;
private:
    int sql_judge(string user_name,string pwd,User_Detail&user_detail);
    mutex lock;
    void user_login(unsigned int ip_addr, QByteArray&array,string &str_array,QTcpSocket*a);
    void user_logout(unsigned int ip_addr);
};

#endif // login_handler_H
