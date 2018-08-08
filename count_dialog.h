#ifndef COUNT_DIALOG_H
#define COUNT_DIALOG_H

#pragma execution_character_set("utf-8")
#include <QDialog>
#include <QtCharts>

namespace Ui {
class Count_Dialog;
}

class Count_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Count_Dialog(QWidget *parent = 0);
    ~Count_Dialog();
signals:
    void Time_Change(const QDateTime &,const QDateTime &);
private slots:
    void Change_Chart(const QDateTime &,const QDateTime &);

    void on_start_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

    void on_end_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime);

private:
    Ui::Count_Dialog *ui;
    QChart* lineChart;
    QDateTimeAxis *lineChart_XAxis;
    QValueAxis *lineChart_YAxis;
    QLineSeries * lineSeries;

    QChart* barChart;
    QBarCategoryAxis *barChart_XAxis;
    QValueAxis *barChart_YAxis;
    QBarSeries * barSeries;

    void New_lineChart(const QDateTime &,const QDateTime &);
    void New_lineChart_XAaxis(const QDateTime &,const QDateTime &);
    void New_lineChart_YAaxis(int,int);
    void New_lineSeries(const QDateTime &,const QDateTime &);


    void New_barChart(const QDateTime &,const QDateTime &);
    void New_barChart_XAaxis();
    void New_barChart_YAaxis(int,int);
    void New_barSeries(const QDateTime &,const QDateTime &);

    //判断要显示的格式，年月日
    int Time_Format(const QDateTime &,const QDateTime &,int &);
};

#endif // COUNT_DIALOG_H
