#include "login_handler.h"
#include <QDebug>
#include <QHostAddress>
#include <QByteArray>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#pragma execution_character_set("utf-8")
int login_handler::cnt=0;
login_handler::login_handler()
{

}
void login_handler::operator ()(QTcpSocket* a)
{
    QByteArray array;
    string str_array;
    User_Detail user_detail;
    cnt++;
    unsigned int ip_addr=a->peerAddress().toIPv4Address();
    array=a->readAll();
    str_array=array.toStdString();
    if(str_array.size()!=0&&str_array[0]=='!'&&str_array.size()==1)//字符串首字符为！，且长度为1，就表明客户登出
    {
        if(user_map.find(ip_addr)!=user_map.end())
        {
            qDebug()<<"user "<<user_map[ip_addr].user_id<<" logout!";
            user_map.erase(ip_addr);
        }
        qDebug()<<"接收了"<<QString::number(cnt)<<" 次。";
        return;
    }
    temp_data[ip_addr]+=str_array;//下面的内容表明客户登录
    string&s=temp_data[ip_addr];
    size_t size=s.size();
    string user_name;
    string user_pwd;
    if(temp_data[ip_addr][size-1]=='$')
    {
        int i=0;
        for(;i<size;i++)
        {
            if(s[i]=='#')
            {
                break;
            }
        }
        user_name=s.substr(0,i);
        user_pwd=s.substr(i+1,size-i-2);
        temp_data.erase(ip_addr);
        QByteArray bytes(user_pwd.c_str());
        int res=sql_judge(user_name,bytes.toHex().toUpper().toStdString(),user_detail);
        if(res==0)
        {
            user_map[ip_addr]=user_detail;
        }
        QByteArray send_array(QString::number(res).toStdString().c_str());
        a->write(send_array);
    }
    qDebug()<<"接收了"<<QString::number(cnt)<<" 次。";
}

int login_handler::sql_judge(string user_name, string pwd,User_Detail&user_detail)
{
    QSqlDatabase db=QSqlDatabase::database();
    QSqlQuery query;
    string user_pwd;
    query.exec(QString("select * from user where user_name='%1'").arg(QString(user_name.c_str())));
    if(query.next())
    {
        user_pwd=query.value(2).toString().toStdString();
        if(user_pwd==pwd)
        {
            user_detail.user_id=query.value(0).toInt();
            user_detail.user_flag=query.value(3).toInt();
            user_detail.user_addr=query.value(4).toInt();
            return 0;
        }
        else return 2;
    }
    else
    {
       return 1;
    }
}
