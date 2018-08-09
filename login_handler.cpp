#include "login_handler.h"
#include <QThread>
#include <QDebug>
#include <QHostAddress>
#include <QByteArray>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "server_login_dialog.h"
#pragma execution_character_set("utf-8")
using namespace std;
int login_handler::cnt=0;
static unordered_map<unsigned int,int> ip_row_map;
login_handler::login_handler(QObject*parent):QObject(parent)
{
}

//这个函数负责用户登录
void login_handler::user_login(unsigned int ip_addr,QByteArray&array,string&str_array,QTcpSocket*a)
{

    qDebug()<<"login_handler thread:"<<QThread::currentThreadId();
    lock_guard<mutex> lockg(this->lock);
    User_Detail user_detail;
    temp_data[ip_addr]+=str_array;
    string&s=temp_data[ip_addr];
    int size=s.size();
    string user_name;
    string user_pwd;
    if(temp_data[ip_addr][size-1]=='$')//这部分分割出用户名和密码
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
        QByteArray bytes;
        for(i++;i<array.size()-1;i++)
        {
            bytes.push_back(array[i]);
        }
        qDebug()<<bytes;
        qDebug()<<bytes.toHex().toUpper();
        int res=sql_judge(user_name,bytes.toHex().toUpper().toStdString(),user_detail);
        if(res==0)//登录成功的相应操作
        {
            user_map[ip_addr]=user_detail;
            MainWindow*gp=static_cast<MainWindow*>(this->parent()->parent());
            gp->ui->plainTextEdit->appendPlainText(QString("user %1 login at ip: %2").arg(user_detail.user_id).arg(a->peerAddress().toString().split("::ffff:")[1]));
            gp->ui->label_5->setText(QString::number(user_map.size()));
            int rowNum=gp->ui->tableWidget->rowCount();
            ip_row_map.insert(make_pair(ip_addr,rowNum));
            gp->ui->tableWidget->insertRow(rowNum);
            gp->ui->tableWidget->setItem(rowNum,0,new QTableWidgetItem(a->peerAddress().toString().split("::ffff:")[1]));
            gp->ui->tableWidget->setItem(rowNum,1,new QTableWidgetItem(QString::number(user_detail.user_id)));
            gp->ui->tableWidget->setItem(rowNum,2,new QTableWidgetItem(QString::number(user_detail.user_addr)));
            gp->ui->tableWidget->setItem(rowNum,3,new QTableWidgetItem(QString::number(user_detail.user_flag)));
            qApp->processEvents();
        }
        temp_data.erase(ip_addr);
        QByteArray send_array(QString::number(res).toStdString().c_str());
        a->write(send_array);
    }
}

//这个函数负责用户登出
void login_handler::user_logout(unsigned int ip_addr)
{
    lock_guard<mutex> lockg(this->lock);
    if(user_map.find(ip_addr)!=user_map.end())
    {
        qDebug()<<"user "<<user_map[ip_addr].user_id<<" logout!";
        MainWindow*gp=static_cast<MainWindow*>(this->parent()->parent());
        gp->ui->plainTextEdit->appendPlainText(QString("user %1 loged out!").arg(user_map[ip_addr].user_id));
        int rowNum=ip_row_map[ip_addr];
        gp->ui->tableWidget->removeRow(rowNum);
        user_map.erase(ip_addr);
        ip_row_map.erase(ip_addr);
        gp->ui->label_5->setText(QString::number(user_map.size()));
        qApp->processEvents();
    }
}

void login_handler::operator ()(QTcpSocket* a)
{
    QByteArray array;
    cnt++;
    unsigned int ip_addr=a->peerAddress().toIPv4Address();
    array=a->readAll();
    QString temp_array=array;
    string str_array(temp_array.toStdString().c_str());
    if(str_array.size()!=0&&str_array[0]=='!'&&str_array.size()==1)//字符串首字符为！，且长度为1，就表明客户登出
    {
        user_logout(ip_addr);
        qDebug()<<"接收了"<<QString::number(cnt)<<" 次。";
        return;
    }
    else//这里表明用户登录
    {
       if(user_map.find(ip_addr)!=user_map.end())
       {
           qDebug()<<QString("此ip已经登录，不能再登录!");
           a->disconnectFromHost();
       }
       else
       {
           user_login(ip_addr,array,str_array,a);
           string&s=temp_data[ip_addr];
           if(s.size()>200)//如果收到大于200长度的字符串，就认为对方恶意登录，强行断开
           {
               temp_data.erase(ip_addr);
               a->disconnectFromHost();
           }
       }
    }
    qDebug()<<"接收了"<<QString::number(cnt)<<" 次。";
}

int login_handler::sql_judge(string user_name, string pwd,User_Detail&user_detail)
{
    QSqlDatabase db=QSqlDatabase::database(QString::number((long long)QThread::currentThreadId()));
    QSqlQuery query(db);
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
