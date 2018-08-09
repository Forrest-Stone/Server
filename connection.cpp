#pragma execution_character_set("utf-8")
#include "connection.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QString>
#include <QStringList>

// 建表语句

// 进站车辆暂存表
QString table_in_cars("create table In_cars"
                      "("
                      "carID varchar(10) primary key,"
                      "inAddr varchar(10),"
                      "inTime datetime"
                      ")");

// 收费信息表
QString table_charge_info("create table Charge_info"
                          "("
                          "inAddr varchar(10),"
                          "outAddr varchar(10),"
                          "price float,"
                          "primary key(inAddr,outAddr)"
                          ")");

// 收费记录表
QString table_charge_record("create table Charge_record"
                            "("
                            "recordID integer primary key auto_increment,"
                            "carID varchar(10),"
                            "inAddr varchar(10),"
                            "outAddr varchar(10),"
                            "inTime datetime,"
                            "outTime datetime,"
                            "charge float"
                            ")");

// 站点名表
QString table_addr_name("create table Addr_name"
                        "("
                        "addrID integer primary key,"
                        "addrName varchar(10)"
                        ")");

// 建立数据库连接
void createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("car");
    db.setPort(3306);
    db.setUserName("root");
    db.setPassword("363677052");
   // db.setPassword("zyszuibang");
    if (!db.open()) {
        qDebug() << "数据库连接失败";
    } else {
        qDebug() << "数据库连接成功";
    }
}

// 建表
void createTables() {
    QSqlQuery query;
    QSqlDatabase db= QSqlDatabase::database();
    QStringList tables = db.tables();

    if (!tables.contains("in_cars")) {
        query.exec(table_in_cars);
    }
    if (!tables.contains("charge_info")) {
        query.exec(table_charge_info);
    }
    if (!tables.contains("charge_record")) {
        query.exec(table_charge_record);
        qDebug() << query.lastError().text();
    }
    if (!tables.contains("addr_name")) {
        query.exec(table_addr_name);
    }
}
