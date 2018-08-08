#ifndef SHOWPICTURE_H
#define SHOWPICTURE_H

#include <QDialog>
#include <QString>
#include <unordered_map>
using namespace std;
namespace Ui {
class ShowPicture;
}

class ShowPicture : public QDialog
{
    Q_OBJECT

public:
    explicit ShowPicture(QWidget *parent = 0);
    static void recognize(QString &file_path, int row_num);
    ~ShowPicture();

    void myshow(int row_num);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

signals:
    int has_check(int row_num);

private:
    Ui::ShowPicture *ui;
    static unordered_map<int,QString>  license;
    static unordered_map<int,QString>  final_res;
    static unordered_map<int,QString>  path;
    static int row_num;
    void check_out();
};

#endif // SHOWPICTURE_H
