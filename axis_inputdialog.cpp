#include "axis_inputdialog.h"
#include "ui_axis_inputdialog.h"

axis_inputDialog::axis_inputDialog(QString label, int pos,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::axis_inputDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Axis Input");
    ui->newaxislabel_lineEdit->setText(label);
    new_label = label;
    position = pos;
    ui->no_radioButton->setChecked(true);
}

axis_inputDialog::~axis_inputDialog()
{
    delete ui;
}

void axis_inputDialog::on_newaxislabel_lineEdit_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        new_label = arg1;
    }
}


void axis_inputDialog::on_buttonBox_accepted()
{
    new_label = (ui->newaxislabel_lineEdit->text());
    if(ui->yes_radioButton->isChecked())
    {
        if(position == 1)
        {
            position = 2;
        }
        else if(position == 2)
        {
            position = 1;
        }
        else if(position == 4)
        {
            position = 8;
        }
        else
        {
            position = 4;
        }
    }
}


void axis_inputDialog::on_yes_radioButton_toggled(bool checked)
{
    if(checked)
    {
        ui->no_radioButton->setChecked(false);
    }
}

void axis_inputDialog::on_no_radioButton_toggled(bool checked)
{
    if(checked)
    {
        ui->yes_radioButton->setChecked(false);
    }
}

void axis_inputDialog::on_buttonBox_rejected()
{
    this->close();
    this->setResult(QDialog::Rejected);
}

QString axis_inputDialog::get_new_label()
{
   return new_label;
}

int axis_inputDialog::get_position()
{
    return position;
}
