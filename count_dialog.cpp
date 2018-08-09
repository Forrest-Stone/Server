

#include "count_dialog.h"
#include "ui_count_dialog.h"
#include "chargemanage.h"
#define MAX_COUNT 0
#define MIN_COUNT 100000

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
    ui->Count_label->setText(QString::number(ChargeManage::carsFlow(ui->start_dateTimeEdit->dateTime(),
                                                                    ui->end_dateTimeEdit->dateTime())));

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
    ui->Count_label->setText(QString::number(ChargeManage::carsFlow(start,end)));
    New_lineChart_XAaxis(start,end);
    New_lineSeries(start,end);
    New_barSeries(start,end);
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
    lineSeries->setPointLabelsClipping();

    lineChart->addSeries(lineSeries);
    //lineChart->createDefaultAxes();  // 基于已添加到图表的 series 来创建轴
    lineChart->setTitle("日车流量折线图");
    //lineChart->legend()->setAlignment(Qt::AlignBottom);
    lineChart->legend()->hide();
    lineChart->setAnimationOptions(QChart::SeriesAnimations);

    lineChart->setAxisX(lineChart_XAxis,lineSeries);
    lineChart->setAxisY(lineChart_YAxis,lineSeries);

}

void Count_Dialog::New_lineChart_XAaxis(const QDateTime &start, const QDateTime &end)
{
    int count;
    int format= Time_Format(start,end,count);
    QDateTime temps,tempe;
    if(format == 0){
        qDebug()<<"error";
    }else if(format == 1){
        lineChart_XAxis->setFormat("yyyy");
        temps.setDate(QDate(start.date().year(),1,1));
        temps.setTime(QTime(0,0));
        tempe.setDate(QDate(end.date().year(),1,1));
        tempe.setTime(QTime(0,0));
    }else if(format == 2){
        lineChart_XAxis->setFormat("yyyy/MM");
        temps.setDate(QDate(start.date().year(),start.date().month(),1));
        temps.setTime(QTime(0,0));
        tempe.setDate(QDate(end.date().year(),end.date().month(),1));
        tempe.setTime(QTime(0,0));
    }else if(format == 3){
        lineChart_XAxis->setFormat("yyyy/MM/dd");
        temps.setDate(start.date());
        temps.setTime(QTime(0,0));
        tempe.setDate(end.date());
        tempe.setTime(QTime(0,0));
    }else{
        qDebug()<<"error";
    }

    lineChart_XAxis->setTickCount(count);
    lineChart_XAxis->setRange(temps,tempe);
}

void Count_Dialog::New_lineChart_YAaxis(int min, int max)
{
    lineChart_YAxis->setRange(min,max);
    lineChart_YAxis->setTickCount(10);
}

void Count_Dialog::New_lineSeries(const QDateTime &start, const QDateTime &end)
{
    QDateTime temps,tempe;
    int max,min,value;
    lineSeries->clear();
    //从数据库查数据
    int count;
    int format = Time_Format(start,end,count);
    max = MAX_COUNT;
    min = MIN_COUNT;
    if(format == 0){
        qDebug()<<"error";
    }else if(format == 1){
        temps.setDate(QDate(start.date().year(),1,1));
        temps.setTime(QTime(0,0));
        tempe.setDate(QDate(start.date().year()+1,1,1));
        tempe.setTime(QTime(0,0));
        for(int i=0;i<count;++i){
            value = ChargeManage::carsFlow(temps,tempe);
            lineSeries->append(temps.toMSecsSinceEpoch(),
                              value );
            //                   temps.date().year());
            temps = temps.addYears(1);
            tempe = tempe.addYears(1);
            if(value > max) max = value;
            if(value < min) min = value;
        }
    }else if(format == 2){
        temps.setDate(QDate(start.date().year(),start.date().month(),1));
        temps.setTime(QTime(0,0));
        tempe.setDate(QDate(start.date().year(),start.date().month()+1,1));
        tempe.setTime(QTime(0,0));
        for(int i=0;i<count;++i){
            value = ChargeManage::carsFlow(temps,tempe);
            lineSeries->append(temps.toMSecsSinceEpoch(),
                              value );
            //                   temps.date().year());
            temps = temps.addMonths(1);
            tempe = tempe.addMonths(1);
            if(value > max) max = value;
            if(value < min) min = value;
        }
    }else if(format == 3){
        temps.setDate(start.date());
        temps.setTime(QTime(0,0));
        tempe.setDate(start.addDays(1).date());
        tempe.setTime(QTime(0,0));
        for(int i=0;i<count;++i){
            value = ChargeManage::carsFlow(temps,tempe);
            lineSeries->append(temps.toMSecsSinceEpoch(),
                              value );
            //                   temps.date().year());
            temps = temps.addDays(1);
            tempe = tempe.addDays(1);
            if(value > max) max = value;
            if(value < min) min = value;
        }
    }


    //添加到数据


    New_lineChart_YAaxis(min,max);
    //New_lineChart_YAaxis(2000,2018);
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
    barChart->setTitle("各站点日车流量柱状图");
    barChart->legend()->setAlignment(Qt::AlignBottom);
    //lineChart->legend()->hide();
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    barChart->setAxisX(barChart_XAxis,barSeries);
    barChart->setAxisY(barChart_YAxis,barSeries);
}

void Count_Dialog::New_barChart_XAaxis()
{
    QStringList categories;
    //添加站信息
    categories<<ChargeManage::getAddrs();
    barChart_XAxis->append(categories);
}

void Count_Dialog::New_barChart_YAaxis(int min, int max)
{
    barChart_YAxis->setRange(min,max);
    barChart_YAxis->setTickCount(10);
}

void Count_Dialog::New_barSeries(const QDateTime &start, const QDateTime &end)
{
    QStringList lists;
    lists<<ChargeManage::getAddrs();
    int max,min,value,value2=0;
    QDateTime temps,tempe;
    temps.setDate(start.date());
    temps.setTime(QTime(0,0));
    tempe.setDate(end.addDays(1).date());
    temps.setTime(QTime(0,0));

    barSeries->clear();

    QBarSet* barsets = new QBarSet("进站车流量");
    QBarSet* barsete = new QBarSet("出站车流量");
    max = MAX_COUNT;
    min = MIN_COUNT;
    for(QStringList::iterator iter=lists.begin();iter!=lists.end();++iter){
        value=ChargeManage::carsFlow(temps,tempe,*iter);
        value2;
        if(value > max) max = value;
        if(value2 > max) max = value2;
        if(value < min) min = value;
        if(value < min) min = value2;
        *barsets<<value;
        *barsete<<value2;
    }
    barSeries->append(barsets);
    barSeries->append(barsete);
    New_barChart_YAaxis(min,max);
    ui->max_Count_label->setText(QString::number(max));
}

int Count_Dialog::Time_Format(const QDateTime &start, const QDateTime &end, int &count)
{
    int years=end.date().year() - start.date().year();
    if(years < 0){
        return 0;
    }
    if(years > 0){
        count = years+1;
        return 1;
    }else{
        int months=end.date().month() - start.date().month();
        if(months < 0){
            return 0;
        }
        if(months > 0){
            count = months+1;
            return 2;
        }else{
            int days = end.date().day()-start.date().day();
            if(days < 0){
                return 0;
            }
            count = days+1;
            return 3;
        }
    }
}
