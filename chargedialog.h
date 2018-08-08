#ifndef CHARGEDIALOG_H
#define CHARGEDIALOG_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class ChargeDialog;
}

class ChargeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChargeDialog(QWidget *parent = 0);
    ~ChargeDialog();

private slots:
    void on_pushButton_query_clicked();

    void on_pushButton_start_sel_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_end_sel_clicked();

private:
    Ui::ChargeDialog *ui;
    QSqlQuery m_query;
    QSqlTableModel *m_model;
    int m_calendarFlag;
};

#endif // CHARGEDIALOG_H
