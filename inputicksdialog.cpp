#include "inputicksdialog.h"
#include "ui_inputicksdialog.h"
#include <QDoubleValidator>

inputicksDialog::inputicksDialog( int x, int y,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::inputicksDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Input Ticks");
    ui->XlineEdit->setValidator(new QDoubleValidator(1, 10000, 2, this));
    ui->YlineEdit->setValidator(new QDoubleValidator(1, 100000000, 2, this));
    xticks = x;
    yticks = y;
    ui->XlineEdit->setText(QString::number(xticks));
    ui->YlineEdit->setText(QString::number(yticks));
}

inputicksDialog::~inputicksDialog()
{
    delete ui;
}



void inputicksDialog::on_buttonBox_accepted()
{
    xticks = (ui->XlineEdit->text()).toDouble();
    yticks = (ui->YlineEdit->text()).toDouble();
    this->close();
    this->setResult(QDialog::Accepted);
}

void inputicksDialog::on_buttonBox_rejected()
{
    this->close();
    this->setResult(QDialog::Rejected);
}


double inputicksDialog::get_xticks()
{
    return xticks;
}

double inputicksDialog::get_yticks()
{
    return yticks;
}
