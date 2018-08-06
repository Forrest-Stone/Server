#ifndef SHOWPICTURE_H
#define SHOWPICTURE_H

#include <QDialog>

namespace Ui {
class ShowPicture;
}

class ShowPicture : public QDialog
{
    Q_OBJECT

public:
    explicit ShowPicture(QWidget *parent = 0);
    ~ShowPicture();

private:
    Ui::ShowPicture *ui;
};

#endif // SHOWPICTURE_H
