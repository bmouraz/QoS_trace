#ifndef AXIS_INPUTDIALOG_H
#define AXIS_INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class axis_inputDialog;
}

class axis_inputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit axis_inputDialog(QString label, int pos,QWidget *parent = nullptr);
    ~axis_inputDialog();

    QString get_new_label();
    int get_position();

private slots:
    void on_newaxislabel_lineEdit_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_yes_radioButton_toggled(bool checked);

    void on_no_radioButton_toggled(bool checked);

    void on_buttonBox_rejected();

private:
    Ui::axis_inputDialog *ui;
    QString new_label;
    int position;
};

#endif // AXIS_INPUTDIALOG_H
