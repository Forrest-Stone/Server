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
    ~ShowPicture();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

signals:
    int has_rec(int row_num);

private:
    Ui::ShowPicture *ui;
    unordered_map<int,QString>  license;
    unordered_map<int,QString>  final_res;
    unordered_map<int,QString>  path;
    int row_num;
    void recognize(QString &file_path, int row_num);
    void check_out();
};

#endif // SHOWPICTURE_H
