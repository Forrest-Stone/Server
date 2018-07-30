#include "server_login_dialog.h"
#include "ui_server_login_dialog.h"
#include <unordered_map>
#include <string>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#pragma execution_character_set("utf-8")
static std::unordered_map<std::string,std::string>  server_user_password;

Server_Login_Dialog::Server_Login_Dialog(QWidget *parent) :QDialog(parent),ui(new Ui::Server_Login_Dialog)
{
    ui->setupUi(this);
    myserver=NULL;
    server_user_password.insert(std::make_pair("zys","123"));
    server_user_password.insert(std::make_pair("zzl","123"));
    server_user_password.insert(std::make_pair("lw","123"));
    server_user_password.insert(std::make_pair("zzd","123"));
}

Server_Login_Dialog::~Server_Login_Dialog()
{
    delete ui;
}
/*这个函数负责处理服务器端的登录
 */
void Server_Login_Dialog::on_pushButton_clicked()
{
    std::string user=ui->lineEdit->text().toStdString();
    std::string password=ui->lineEdit_2->text().toStdString();
    if(user=="")
    {
        QMessageBox::warning(this,"错误","用户名不能为空！！！");
        return;
    }
    if(password=="")
    {
        QMessageBox::warning(this,"错误","密码不能为空!!!");
        return;
    }
    if(server_user_password.find(user)==server_user_password.end())
    {
        QMessageBox::warning(this,"错误","用户名不存在!!!");
        return;
    }
    else
    {
        if(password!=server_user_password[user])
        {
            QMessageBox::warning(this,"错误","密码错误!!!");//这里遇到一个神奇的问题，不能用中文符号来结尾，用了就说常量中有换行符,VC编译器的问题
            return;
        }
    }
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    qDebug()<<"server login success  哈哈";
    myserver=new Login_TcpServer(this,7777,100,fun);
    qDebug()<<"Now server is listening at port 7777 waiting for clients to login.";
}
