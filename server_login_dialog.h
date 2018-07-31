#ifndef SERVER_LOGIN_DIALOG_H
#define SERVER_LOGIN_DIALOG_H

#include <QDialog>
#include "login_handler.h"
#include "login_tcpserver.h"
namespace Ui {
class Server_Login_Dialog;
}
class login_handler;
class Server_Login_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Server_Login_Dialog(QWidget *parent = 0);
    ~Server_Login_Dialog();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Server_Login_Dialog *ui;
    login_handler * fun;
    Login_TcpServer*myserver;
};

#endif // SERVER_LOGIN_DIALOG_H
