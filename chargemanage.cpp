#pragma execution_character_set("utf-8")
#include "chargemanage.h"
#include <chargemanage.h>
#include <QDebug>
#include <QSqlError>

ChargeManage::ChargeManage()
{
    // 读取站点计费规则
    m_chargeInfo.geGraph();
    // m_chargeInfo.show();
    // 读取进站暂存表
    m_query.exec("select * from In_cars");
    while (m_query.next()) {
        QString carID = m_query.value("carID").toString();
        QString inAddr = m_query.value("inAddr").toString();
        QDateTime inTime = m_query.value("InTime").toDateTime();
        InRecord *in = new InRecord(carID, inAddr, inTime);
        m_carsMap.insert(carID, in);
    }
}

ChargeManage::~ChargeManage()
{
    QMap<QString, InRecord*>::iterator it;
    it = m_carsMap.begin();
    while (it != m_carsMap.end()) {
        InRecord *in = it.value();
        it = m_carsMap.erase(it);
        delete in;
        in = NULL;
    }
}

// 车辆进站时进行处理
void ChargeManage::carIn(QString carID, int inAddrID, QDateTime inTime)
{
    QString inAddr = getAddrName(inAddrID);
    InRecord *in = new InRecord(carID, inAddr, inTime);
    m_carsMap.insert(carID, in);
    // 添加进站车辆记录
    m_query.exec(QString("insert into In_cars values('%1','%2','%3')").arg(
                     carID, inAddr, inTime.toString("yyyy-MM-dd hh:mm:ss")));
    if (m_query.lastError().isValid()) {
        qDebug() << "ChargeManage::carIn" << m_query.lastError().text();
    }
}

// 出站时计费
void ChargeManage::carOut(QString carID, int outAddrID, QDateTime outTime)
{
    // showCarsMap();
    QString outAddr = getAddrName(outAddrID);
    if (!m_carsMap.contains(carID)) {
        qDebug() << "ChargeManage::carOut id error";
        return;
    }
    InRecord *in = m_carsMap.value(carID);
    QString inAddr = in->inAddr;
    QString inTimeStr = in->inTime.toString("yyyy-MM-dd hh:mm:ss");
    QString outTimeStr = outTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << inAddr << outAddr;
    double price = m_chargeInfo.getPrice(inAddr, outAddr);
    if (price == -1) {
        qDebug() << "ChargeManage::carOut price error";
        return;
    }
    // 添加收费记录
    m_query.exec(QString("insert into Charge_record "
                         "values(NULL,'%1','%2','%3','%4','%5',%6)").arg(
                     carID, inAddr, outAddr, inTimeStr, outTimeStr, QString::number(price)));
    if (m_query.lastError().isValid()) {
        qDebug() << "ChargeManage::carout insert record error" << m_query.lastError().text();
    }
    // 删除进站暂存记录
    m_carsMap.remove(carID);
    delete in;
    in = NULL;
    m_query.exec(QString("delete from In_cars where carID='%1'").arg(carID));
    if (m_query.lastError().isValid()) {
        qDebug() << "ChargeManage::carout delete record error" << m_query.lastError().text();
    }
}

// 统计车流量
int ChargeManage::carsFlow(QDateTime start, QDateTime end, QString addr)
{
    int flow = 0;
    QSqlQuery query;
    QString startStr = start.toString("yyyy-MM-dd hh:mm:ss");
    QString endStr = end.toString("yyyy-MM-dd hh:mm:ss");
    if (addr == "") {
        query.exec(QString("select count(*) from Charge_record "
                           "where inTime between '%1' and '%2'").arg(startStr, endStr));
    } else {
        query.exec(QString("select count(*) from Charge_record "
                           "where inAddr='%1' and inTime between '%2' and '%3'").arg(
                       addr, startStr, endStr));
    }
    if (query.next()) {
        flow = query.value(0).toInt();
        qDebug() << "traffic flow: " << flow;
    }
    return flow;
}

// 根据ID获取站点名
QString ChargeManage::getAddrName(int addr){
    m_query.exec(QString("select addrName from Addr_name where addrID=%1").arg(
                     QString::number(addr)));
    if (m_query.next()) {
        return m_query.value(0).toString();
    }
    return "";
}

void ChargeManage::showCarsMap()
{
    QMap<QString, InRecord*>::const_iterator it;
    it = m_carsMap.constBegin();
    while (it != m_carsMap.constEnd()) {
        InRecord *in = it.value();
        qDebug() << it.key() << in->inAddr << in->inTime;
        ++it;
    }
}

// 获取站点名
QStringList ChargeManage::getAddrs()
{
    QStringList names;
    QSqlQuery query;
    query.exec("select addrName from addr_name");
    while (query.next()) {
        names.append(query.value(0).toString());
    }
    return names;
}
