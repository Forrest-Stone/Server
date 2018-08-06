#include "showpicture.h"
#include "ui_showpicture.h"
#include "opencv2/opencv.hpp"
#include "easypr.h"
#include <QMessageBox>
using namespace cv;
using namespace easypr;
#pragma execution_character_set("utf-8")
ShowPicture::ShowPicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowPicture)
{
    ui->setupUi(this);
    ui->lineEdit->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit_2->setMaxLength(7);
    ui->pushButton_3->setEnabled(false);
//    ui->label->show();

//    QPixmap pixmap(normalIcon);
//    QPixmap fitpixmap=pixmap.scaled(labelIcon->width(), labelIcon->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    labelIcon->setPixmap(fitpixmap);

//    ui->label->setStyleSheet("QLabel{border-image:url(:/DSCF2912.jpg)}");
//    ui->label_4->setStyleSheet("QLabel{border-image:url(:/DSCF2912111.jpg)}");
}

void ShowPicture::recognize(QString& file_path, int row_num)
{
    Mat src = imread(file_path.toStdString().c_str());
    CPlateRecognize pr;
    pr.setDetectType(PR_DETECT_CMSER|PR_DETECT_COLOR|PR_DETECT_SOBEL);
    vector<CPlate> plateVec;
    pr.setResultShow(false);
    int result = pr.plateRecognize(src,plateVec);
    CPlate plate = plateVec[0];
    QString res =  QString::fromLocal8Bit(plate.getPlateStr().substr(3,7).c_str());
    license.insert(make_pair(row_num,res));
    path.insert(make_pair(row_num,file_path));
}

ShowPicture::~ShowPicture()
{
    delete ui;
}

//等那个DBM搞完再写吧
void ShowPicture::check_out()
{

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

}

void ShowPicture::on_lineEdit_2_textChanged(const QString &arg1)
{
    if(arg1.size()==7) ui->pushButton_3->setEnabled(true);
    else ui->pushButton_3->setEnabled(false);
}
