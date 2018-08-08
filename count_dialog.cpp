
#include "count_dialog.h"
#include "ui_count_dialog.h"

QT_CHARTS_USE_NAMESPACE

Count_Dialog::Count_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Count_Dialog)
{
    ui->setupUi(this);
    lineChart = new QChart();
    lineChart_XAxis = new QDateTimeAxis();
    lineChart_YAxis = new QValueAxis();
    lineSeries = new QLineSeries();

    barChart = new QChart();
    barChart_XAxis = new QBarCategoryAxis();
    barChart_YAxis = new QValueAxis();
    barSeries = new QBarSeries();


    ui->start_dateTimeEdit->setDateTimeRange(QDateTime(),QDateTime::currentDateTime());
    ui->start_dateTimeEdit->setDateTime(QDateTime(QDate(2018,8,1),QTime(0,0)));
    ui->end_dateTimeEdit->setDateTimeRange(ui->start_dateTimeEdit->dateTime(),QDateTime::currentDateTime());
    ui->end_dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    New_lineChart(ui->start_dateTimeEdit->dateTime(),
                  ui->end_dateTimeEdit->dateTime());
    ui->lineChart->setChart(lineChart);

    New_barChart(ui->start_dateTimeEdit->dateTime(),
                 ui->end_dateTimeEdit->dateTime());
    ui->barChart->setChart(barChart);

    connect(this,SIGNAL(Time_Change(QDateTime,QDateTime)),this,SLOT(Change_Chart(QDateTime,QDateTime)));

}

Count_Dialog::~Count_Dialog()
{
    delete ui;
}

void Count_Dialog::Change_Chart(const QDateTime &start, const QDateTime &end)
{
    New_lineChart_XAaxis(start,end);
    New_lineSeries(start,end);
}

void Count_Dialog::on_start_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{

    ui->end_dateTimeEdit->setMinimumDateTime(dateTime);
    emit Time_Change(dateTime,ui->end_dateTimeEdit->dateTime());

}

void Count_Dialog::on_end_dateTimeEdit_dateTimeChanged(const QDateTime &dateTime)
{
    ui->start_dateTimeEdit->setMaximumDateTime(dateTime);
    emit Time_Change(ui->start_dateTimeEdit->dateTime(),dateTime);
}

void Count_Dialog::New_lineChart(const QDateTime &start, const QDateTime &end)
{

    New_lineChart_XAaxis(start,end);
    New_lineSeries(start,end);

    lineSeries->setPointLabelsVisible();
    lineSeries->setPointLabelsFormat("@yPoint");

    lineChart->addSeries(lineSeries);
    //lineChart->createDefaultAxes();  // 基于已添加到图表的 series 来创建轴
    lineChart->setTitle("title");
    //lineChart->legend()->setAlignment(Qt::AlignBottom);
    lineChart->legend()->hide();
    lineChart->setAnimationOptions(QChart::SeriesAnimations);

    lineChart->setAxisX(lineChart_XAxis,lineSeries);
    lineChart->setAxisY(lineChart_YAxis,lineSeries);

}

void Count_Dialog::New_lineChart_XAaxis(const QDateTime &start, const QDateTime &end)
{
    int years=end.date().year() - start.date().year();
    if(years > 0){
        lineChart_XAxis->setFormat("yyyy");
        lineChart_XAxis->setTickCount(years+1);
    }else{
        int months=end.date().month() - start.date().month();
        if(months > 0){
            lineChart_XAxis->setFormat("yyyy/MM");
            lineChart_XAxis->setTickCount(months+1);
        }else{
            int days=start.daysTo(end);
            lineChart_XAxis->setFormat("yyyy/MM/dd");
            lineChart_XAxis->setTickCount(days+1);
        }
    }

    lineChart_XAxis->setRange(start,end);
}

void Count_Dialog::New_lineChart_YAaxis(int min, int max)
{
    lineChart_YAxis->setRange(min,max);
    lineChart_YAxis->setTickCount(10);
}

void Count_Dialog::New_lineSeries(const QDateTime &start, const QDateTime &end)
{

    lineSeries->clear();
    //从数据库查数据

    //添加到数据

    lineSeries->append(start.toMSecsSinceEpoch(),start.toString("MMdd").toInt());
    lineSeries->append(end.toMSecsSinceEpoch(),end.toString("MMdd").toInt());

    New_lineChart_YAaxis(start.toString("MMdd").toInt(),end.toString("MMdd").toInt());
}

void Count_Dialog::New_barChart(const QDateTime &start, const QDateTime &end)
{
    New_barChart_XAaxis();
    New_barSeries(start,end);

    barSeries->setLabelsVisible();
    barSeries->setLabelsFormat("@value");
    //barSeries->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);

    barChart->addSeries(barSeries);
    barChart->createDefaultAxes();  // 基于已添加到图表的 series 来创建轴
    barChart->setTitle("title2");
    barChart->legend()->setAlignment(Qt::AlignBottom);
    //lineChart->legend()->hide();
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    barChart->setAxisX(barChart_XAxis,barSeries);
    barChart->setAxisY(barChart_YAxis,barSeries);
}

void Count_Dialog::New_barChart_XAaxis()
{
    QStringList categories;
    categories<<"0"<<"1";
    barChart_XAxis->append(categories);
}

void Count_Dialog::New_barChart_YAaxis(int min, int max)
{
    barChart_YAxis->setRange(min,max);
    barChart_YAxis->setTickCount(10);
}

void Count_Dialog::New_barSeries(const QDateTime &, const QDateTime &)
{
    barSeries->clear();
    QList<QBarSet*> barsets;
    for(int i=0;i<2;++i){
        QBarSet * set = new QBarSet(QString::number(i));
        *set<<i<<i+1;
        barsets.push_back(set);
    }
    barSeries->append(barsets);

    New_barChart_YAaxis(0,2);
}
