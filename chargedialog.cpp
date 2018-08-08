#pragma execution_character_set("utf-8")
#include "chargedialog.h"
#include "ui_chargedialog.h"
#include <QString>
#include <QDebug>

ChargeDialog::ChargeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChargeDialog)
{
    ui->setupUi(this);

    m_model = new QSqlTableModel(this);

    m_model->setTable("charge_record");
    m_model->removeColumn(0);
    m_model->setHeaderData(0, Qt::Horizontal, "车牌号");
    m_model->setHeaderData(1, Qt::Horizontal, "进站口");
    m_model->setHeaderData(2, Qt::Horizontal, "出站口");
    m_model->setHeaderData(3, Qt::Horizontal, "进站时间");
    m_model->setHeaderData(4, Qt::Horizontal, "出站时间");
    m_model->setHeaderData(5, Qt::Horizontal, "费用");

    ui->tableView->setModel(m_model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setColumnWidth(0, 70);
    ui->tableView->setColumnWidth(1, 50);
    ui->tableView->setColumnWidth(2, 50);
    ui->tableView->setColumnWidth(3, 100);
    ui->tableView->setColumnWidth(4, 100);
    ui->tableView->setColumnWidth(5, 50);

    ui->lineEdit_start->setReadOnly(true);
    ui->lineEdit_end->setReadOnly(true);
    ui->lineEdit_end->setText(QDate::currentDate().toString("yyyy-MM-dd"));

    ui->calendarWidget->hide();
    m_calendarFlag = 0;
}

ChargeDialog::~ChargeDialog()
{
    delete ui;
}

// 查询
void ChargeDialog::on_pushButton_query_clicked()
{
    QString carID = ui->lineEdit_id->text();
    QString start = ui->lineEdit_start->text();
    QString end = ui->lineEdit_end->text();

    QString filter;
    if (carID != "") {
        filter = QString("carID='%1'").arg(carID);
    }
    if (start != "") {
        if (filter != "") {
            filter += " and ";
        }
        start += " 00:00:00";
        end += " 00:00:00";
        filter += QString("inTime between '%1' and '%2'").arg(start, end);
    }
    m_model->setFilter(filter);
    m_model->select();
}

// 选择开始时间
void ChargeDialog::on_pushButton_start_sel_clicked()
{
    if (ui->calendarWidget->isVisible()) {
        ui->calendarWidget->hide();
    } else {
        m_calendarFlag = 0;
        ui->calendarWidget->show();
    }
}

// 选择结束时间
void ChargeDialog::on_pushButton_end_sel_clicked()
{
    if (ui->calendarWidget->isVisible()) {
        ui->calendarWidget->hide();
    } else {
        m_calendarFlag = 1;
        ui->calendarWidget->show();
    }
}

// 选择时间
void ChargeDialog::on_calendarWidget_clicked(const QDate &date)
{
    if (m_calendarFlag == 0) {
        ui->lineEdit_start->setText(date.toString("yyyy-MM-dd"));
    } else {
        ui->lineEdit_end->setText(date.toString("yyyy-MM-dd"));
    }
    ui->calendarWidget->hide();
}


