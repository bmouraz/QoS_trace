#include "customlinesdialog.h"
#include "ui_customlinesdialog.h"
#include <QDebug>

CustomLinesdialog::CustomLinesdialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CustomLinesdialog)
{
  ui->setupUi(this);
  ajust_LineStylecombo();
  ajust_ScatterStylecombo();
  ajust_Scatterslider();
}

CustomLinesdialog::~CustomLinesdialog()
{
  delete ui;
}

void CustomLinesdialog::on_color_Button_clicked()
{
    color = QColorDialog::getColor();
}

QColor CustomLinesdialog::get_Color()
{
  return color;
}

void CustomLinesdialog::ajust_LineStylecombo()
{
  ui->Line_Combo->clear();
  ui->Line_Combo->addItem("Solid");
  ui->Line_Combo->addItem("Dash");
  ui->Line_Combo->addItem("Dash Dot");
  ui->Line_Combo->addItem("Dash Dot Dot");
  ui->Line_Combo->addItem("Dot");
}

void CustomLinesdialog::ajust_ScatterStylecombo()
{
  ui->scatter_Combo->clear();
  ui->scatter_Combo->addItem("Cross");
  ui->scatter_Combo->addItem("Plus");
  ui->scatter_Combo->addItem("Circle");
  ui->scatter_Combo->addItem("Disc");
  ui->scatter_Combo->addItem("Square");
  ui->scatter_Combo->addItem("Diamond");
  ui->scatter_Combo->addItem("Star");
  ui->scatter_Combo->addItem("Triangle");
  ui->scatter_Combo->addItem("Inverted Triangle");
  ui->scatter_Combo->addItem("Cross Square");
  ui->scatter_Combo->addItem("Plus Square");
  ui->scatter_Combo->addItem("Cross Circle");
  ui->scatter_Combo->addItem("Plus Circle");
  ui->scatter_Combo->addItem("Peace");
  ui->scatter_Combo->addItem("Custom");
}


void CustomLinesdialog::on_radioButton_clicked(bool checked)
{
  if(checked == 1)
    {
      ui->scatter_Combo->setEnabled(true);
      ui->scatter_Slider->setEnabled(true);
      ui->ssize_Label->setEnabled(true);
      ui->sstyle_Label->setEnabled(true);

    }
  else
    {
      ui->scatter_Combo->setEnabled(false);
      ui->scatter_Slider->setEnabled(false);
      ui->ssize_Label->setEnabled(false);
      ui->sstyle_Label->setEnabled(false);
      scatterstyleret = "None";
      scattersizeret = 0;
    }
}

void CustomLinesdialog::on_Line_Combo_currentTextChanged(const QString &arg1)
{
    linestyleret = arg1;
}

void CustomLinesdialog::on_scatter_Combo_currentTextChanged(const QString &arg1)
{
    scatterstyleret = arg1;
}

void CustomLinesdialog::ajust_Scatterslider()
{
  ui->scatter_Slider->setTickInterval(5);
  ui->scatter_Slider->setMinimum(5);
  ui->scatter_Slider->setValue(10);
  scattersizeret = 10;
  ui->scatter_Slider->setMaximum(30);
}

QString CustomLinesdialog::get_LineStyle()
{
  return linestyleret;
}

QString CustomLinesdialog::get_ScatterStyle()
{
  return scatterstyleret;
}

void CustomLinesdialog::on_scatter_Slider_sliderMoved(int position)
{
    scattersizeret = position;
}

int CustomLinesdialog::get_ScatterSize()
{
  return scattersizeret;
}


void CustomLinesdialog::on_ok_cancel_apply_buttons_accepted()
{
  this->close();
  this->setResult(QDialog::Accepted);
}

void CustomLinesdialog::on_ok_cancel_apply_buttons_rejected()
{
  this->close();
  this->setResult(QDialog::Rejected);
}

void CustomLinesdialog::ajust_Graphcombo(QVector<int> graphs)
{
  ui->graph_Combo->clear();
  for(int i =0;i<graphs.size();i++)
    {
      ui->graph_Combo->addItem(QString::number(graphs.at(i)));
    }
}

int CustomLinesdialog::getgraphindex()
{
  return graphindex;
}
void CustomLinesdialog::on_graph_Combo_currentIndexChanged(int index)
{
    if (index>-1)
      graphindex = index;
}
