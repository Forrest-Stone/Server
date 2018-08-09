#ifndef CHARGEMANAGE_H
#define CHARGEMANAGE_H

/*
 * 计费管理
 * */

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QMap>
#include <QSqlQuery>
#include <QStringList>
#include "chargeinfo.h"

class ChargeManage : QObject
{
    Q_OBJECT

public:
    ChargeManage();
    ~ChargeManage();
    static int carsFlow(QDateTime start, QDateTime end, QString addr = "");   // 一段时间内的进站车流量
    static int carsFlowOut(QDateTime start, QDateTime end, QString addr);     // 一段时间内出站车流量
    static QStringList getAddrs();                                            // 获取站点名


public slots:
    void carIn(QString carID, int inAddrID, QDateTime inTime);       // 车辆进站时调用
    void carOut(QString carID, int outAddrID, QDateTime outTime);    // 车辆出站时调用

private:
    QString getAddrName(int addr);
    void showCarsMap();

    struct InRecord {                                                // 车辆进站记录结构体
        InRecord(QString id, QString ia, QDateTime t) :
            carID(id), inAddr(ia), inTime(t) {}
        QString carID;
        QString inAddr;
        QDateTime inTime;
    };
    QMap<QString, InRecord*> m_carsMap;                   // 进站车辆暂存表
    ChargeInfo m_chargeInfo;                              // 站点计费信息
    QSqlQuery m_query;
};

#endif // CHARGEMANAGE_H
