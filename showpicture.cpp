#include "showpicture.h"
#include "ui_showpicture.h"

ShowPicture::ShowPicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowPicture)
{
    ui->setupUi(this);

    QPixmap pixmap(":/DSCF2912.jpg");

     ui->label->setPixmap(pixmap);

//    ui->label->show();

//    QPixmap pixmap(normalIcon);
//    QPixmap fitpixmap=pixmap.scaled(labelIcon->width(), labelIcon->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    labelIcon->setPixmap(fitpixmap);

//    ui->label->setStyleSheet("QLabel{border-image:url(:/DSCF2912.jpg)}");
//    ui->label_4->setStyleSheet("QLabel{border-image:url(:/DSCF2912111.jpg)}");
}

ShowPicture::~ShowPicture()
{
    delete ui;
}
