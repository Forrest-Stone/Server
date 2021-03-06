﻿#include "showpicture.h"
#include "ui_showpicture.h"
#include "easypr.h" 
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "user_detail.h"
#include <QDateTime>
#include <QStringList>
#pragma execution_character_set("utf-8")
 std::unordered_map<int,QString>  ShowPicture::license;
 std::unordered_map<int,QString>  ShowPicture::final_res;
 std::unordered_map<int,QString>  ShowPicture::path;
 std::unordered_map<int,unsigned int> ShowPicture::row_ip;
 int ShowPicture::row_num;
ShowPicture::ShowPicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowPicture)
{
    ui->setupUi(this);
    ui->lineEdit->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_2->setMaxLength(7);
    ui->pushButton_3->setEnabled(false);
//    connect(this,SIGNAL(has_check(int)),static_cast<MainWindow*>(this->parent()),SLOT(SlotChangeRecState(int)));
//    ui->label->show();

//    QPixmap pixmap(normalIcon);
//    QPixmap fitpixmap=pixmap.scaled(labelIcon->width(), labelIcon->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    labelIcon->setPixmap(fitpixmap);

//    ui->label->setStyleSheet("QLabel{border-image:url(:/DSCF2912.jpg)}");
//    ui->label_4->setStyleSheet("QLabel{border-image:url(:/DSCF2912111.jpg)}");
}

void ShowPicture::recognize(const QString &file_path, int row_num,unsigned int ip_addr)
{
    qDebug()<<file_path.toStdString().c_str();
    cv::Mat src = cv::imread(file_path.toStdString().c_str());
    easypr::CPlateRecognize pr;
    QString res;
    pr.setDetectType(easypr::PR_DETECT_CMSER|easypr::PR_DETECT_COLOR|easypr::PR_DETECT_SOBEL);
    vector<easypr::CPlate> plateVec;
    pr.setResultShow(false);
    //int result = pr.plateRecognize(src,plateVec);
    if(plateVec.size()>0)
    {
        easypr::CPlate plate = plateVec[0];
        res =  QString::fromLocal8Bit(plate.getPlateStr().substr(3,10).c_str());
    }
    else
    {
        res = QString("无法识别");
    }
    qDebug()<<QString("结果： ")<<res;
    license.insert(make_pair(row_num,res));
    path.insert(make_pair(row_num,file_path));
    row_ip.insert(make_pair(row_num,ip_addr));
}

ShowPicture::~ShowPicture()
{
    delete ui;
}

void ShowPicture::myshow(int row_num)
{
    ShowPicture::row_num = row_num;
    ui->label->setScaledContents(true);
    ui->label->setPixmap(path[row_num]);
    qDebug()<<path[row_num];
    ui->lineEdit->setText(license[row_num]);
    qDebug()<<license[row_num];
    this->show();
}

void ShowPicture::check_out()
{
    final_res[row_num] = ui->lineEdit_2->text();
    QSqlQuery query;
    query.exec(QString("insert into picture(license,picture_path) values('%1','%2')").arg(final_res[row_num]).arg(path[row_num]));
    qDebug()<<query.lastError();
    emit has_check(row_num);
    extern unordered_map<unsigned int,User_Detail> user_map;
    User_Detail detail = user_map[row_ip[row_num]];
    if(!detail.user_flag)
    {
        emit car_in(final_res[row_num],detail.user_addr,QDateTime::fromString(path[row_num].split('.').at(0).right(17),"yyyyMMddhhmmsszzz"));
    }
    else
    {
        emit car_out(final_res[row_num],detail.user_addr,QDateTime::fromString(path[row_num].split('.').at(0).right(17),"yyyyMMddhhmmsszzz"));
    }
    this->close();

}

void ShowPicture::on_pushButton_clicked()
{
    ui->lineEdit_2->setText(license[row_num]);
    if(ui->lineEdit_2->text()==7)
    ui->pushButton_3->setEnabled(true);
}

void ShowPicture::on_pushButton_2_clicked()
{
    QMessageBox::warning(this,QString("提醒"),QString("请在最终结果处输入正确车牌号。"));
    ui->lineEdit_2->setFocus();
}


void ShowPicture::on_pushButton_3_clicked()
{
     check_out();
}

void ShowPicture::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(arg1.size()==7) ui->pushButton_3->setEnabled(true);
    else ui->pushButton_3->setEnabled(false);
}
