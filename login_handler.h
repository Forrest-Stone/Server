#ifndef LOGIN_HANDLER_H
#define LOGIN_HANDLER_H
#include "handler.h"
#include <unordered_map>
#include <string>
#include "user_detail.h"
using namespace std;
extern unordered_map<unsigned int,User_Detail> user_map;
class login_handler : public handler
{
public:
    login_handler();
    void operator ()(QTcpSocket*);
    static int cnt;
    unordered_map<unsigned int,string> temp_data;
private:
    int sql_judge(string user_name,string pwd,User_Detail&user_detail);
};

#endif // login_handler_H
