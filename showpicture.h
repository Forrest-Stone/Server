#ifndef SHOWPICTURE_H
#define SHOWPICTURE_H

#include <QDialog>
#include <QString>
#include <unordered_map>
#include <QDateTime>
namespace Ui {
class ShowPicture;
}

class ShowPicture : public QDialog
{
    Q_OBJECT

public:
    explicit ShowPicture(QWidget *parent = 0);
    static void recognize(const QString &file_path, int row_num,unsigned int ip_addr);
    ~ShowPicture();

    void myshow(int row_num);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

signals:
    int has_check(int row_num);
    int car_in(QString license,int addr_id,QDateTime time);
    int car_out(QString license,int addr_id,QDateTime time);
private:
    Ui::ShowPicture *ui;
    static std::unordered_map<int,QString>  license;
    static std::unordered_map<int,QString>  final_res;
    static std::unordered_map<int,QString>  path;
    static std::unordered_map<int,unsigned int> row_ip;
    static int row_num;
    void check_out();
};

#endif // SHOWPICTURE_H
