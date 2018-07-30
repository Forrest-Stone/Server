#ifndef CONNECTION_H
#define CONNECTION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("car"); //这里输入你的数据库名
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("363677052");   //这里输入你的密码
    if (!db.open()) {
     qDebug()<<"hahahh";
     qDebug()<<db.isOpen();
     //QMessageBox::warning(0,"error","error");
     return false;
    }
    else
    {
        qDebug()<<"success";
    }

    return true;
}
#endif // CONNECTION_H
