#pragma execution_character_set("utf-8")
#include "chargeinfo.h"
#include <QVariantMap>
#include <QList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ChargeInfo::ChargeInfo()
{

}

ChargeInfo::~ChargeInfo()
{
    QMap<QString, QList<EdgeNode*>* >::iterator it;
    it = m_priceMap.begin();
    while (it != m_priceMap.end()) {
        QList<EdgeNode*> *edgeList = it.value();
        while (!edgeList->empty()) {
            delete edgeList->takeFirst();
        }
        delete edgeList;
        edgeList = NULL;
        ++it;
    }
}

// 生成邻接表
void ChargeInfo::geGraph()
{
    QSqlQuery query;
    query.exec("select * from Charge_info");
    while (query.next()) {
        QString inAddr(query.value("inAddr").toString());
        if (!m_priceMap.contains(inAddr)) {
            QList<EdgeNode*> *edgeList = new QList<EdgeNode*>;
            m_priceMap.insert(inAddr, edgeList);
        }
        EdgeNode *edge = new EdgeNode(query.value("outAddr").toString(),
                                      query.value("price").toDouble());
        m_priceMap[inAddr]->append(edge);
    }
}

// 获取价格
double ChargeInfo::getPrice(const QString &in, const QString &out)
{
    QList<EdgeNode*> *list = m_priceMap[in];
    for (int i = 0; i < list->size(); ++i) {
        if (list->at(i)->outAddr == out) {
            return list->at(i)->price;
        }
    }
    return -1;
}

// 显示数据
void ChargeInfo::show()
{
    QMap<QString, QList<EdgeNode*>* >::const_iterator it;
    it = m_priceMap.constBegin();
    while (it != m_priceMap.constEnd()) {
        QList<EdgeNode*> *list = it.value();
        for (int i = 0; i < list->size(); ++i) {
            qDebug() << it.key() << list->at(i)->outAddr << list->at(i)->price;
        }
        ++it;
    }
}
