#ifndef CHARGEINFO_H
#define CHARGEINFO_H

/*
 * 计费信息处理类
 * */

#include <QMap>
#include <QString>


class ChargeInfo
{
public:
    ChargeInfo();
    ~ChargeInfo();
    void geGraph();                                             // 生成邻接表
    void show();                                                // 显示数据
    double getPrice(const QString &in, const QString &out);     // 获取费用

private:
    struct EdgeNode{                                            // 邻接表边节点
        EdgeNode(QString oa, double p) :
            outAddr(oa), price(p) {}
        QString outAddr;
        double price;
    };
    QMap<QString, QList<EdgeNode*>* > m_priceMap;                // 邻接表，保存计费信息
};

#endif // CHARGEINFO_H
