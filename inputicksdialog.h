#ifndef INPUTICKSDIALOG_H
#define INPUTICKSDIALOG_H

#include <QDialog>

namespace Ui {
class inputicksDialog;
}

class inputicksDialog : public QDialog
{
    Q_OBJECT

public:
    explicit inputicksDialog(int x, int y,QWidget *parent = nullptr);
    ~inputicksDialog();
    double get_xticks();
    double get_yticks();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();



private:
    Ui::inputicksDialog *ui;
    double xticks;
    double yticks;
};

#endif // INPUTICKSDIALOG_H
