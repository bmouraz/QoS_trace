#include "customlinesdialog.h"
#include "ui_customlinesdialog.h"
#include <QDebug>

CustomLinesdialog::CustomLinesdialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CustomLinesdialog)
{
  ui->setupUi(this);
  this->setWindowTitle("Customize Graphics");
  ajust_LineStylecombo();
  ajust_ScatterStylecombo();
  ajust_Scatterslider();
  ajust_linewidthslider();
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
  QPixmap solid(":/images/line-solid.png");

  ui->Line_Combo->clear();
  ui->Line_Combo->addItem(QIcon(":/images/lines/line-solid.png"),"Solid");
  ui->Line_Combo->addItem(QIcon(":/images/lines/line-dash.png"),"Dash");
  ui->Line_Combo->addItem(QIcon(":/images/lines/line-dashdot.png"),"Dash Dot");
  ui->Line_Combo->addItem(QIcon(":/images/lines/line-dashdotdot.png"),"Dash Dot Dot");
  ui->Line_Combo->addItem(QIcon(":/images/lines/line-dot.png"),"Dot");

  ui->Line_Combo->setIconSize(QSize(100,20));
}

void CustomLinesdialog::ajust_ScatterStylecombo()
{
    ui->scatter_Combo->clear();
    QVector<QCPScatterStyle::ScatterShape> shapes;
    QVector<QString> shapes_names;

    shapes << QCPScatterStyle::ssCross;
    shapes << QCPScatterStyle::ssPlus;
    shapes << QCPScatterStyle::ssCircle;
    shapes << QCPScatterStyle::ssDisc;
    shapes << QCPScatterStyle::ssSquare;
    shapes << QCPScatterStyle::ssDiamond;
    shapes << QCPScatterStyle::ssStar;
    shapes << QCPScatterStyle::ssTriangle;
    shapes << QCPScatterStyle::ssTriangleInverted;
    shapes << QCPScatterStyle::ssCrossSquare;
    shapes << QCPScatterStyle::ssPlusSquare;
    shapes << QCPScatterStyle::ssCrossCircle;
    shapes << QCPScatterStyle::ssPlusCircle;
    shapes << QCPScatterStyle::ssPeace;

    shapes_names << "Cross";
    shapes_names << "Plus";
    shapes_names << "Circle";
    shapes_names << "Disc";
    shapes_names << "Square";
    shapes_names << "Diamond";
    shapes_names << "Star";
    shapes_names << "Triangle";
    shapes_names << "Inverted Triangle";
    shapes_names << "Cross Square";
    shapes_names << "Plus Square";
    shapes_names << "Cross Circle";
    shapes_names << "Plus Circle";
    shapes_names << "Peace";


    for(int i=0;i<shapes.count();i++)
    {
        // fill QComboBox

        QCPScatterStyle ss = shapes.at(i);
        QPixmap *pm = new QPixmap(15,15);
        QCPPainter *qp = new QCPPainter(pm);
        qp->fillRect(0,0,15,15,QBrush(Qt::white, Qt::SolidPattern));
        ss.applyTo(qp, QPen(Qt::black));
        ss.drawShape(qp,7,7);
        QIcon   icon = QIcon(*pm);
        ui->scatter_Combo->addItem(icon, shapes_names[i]);
    }


}


void CustomLinesdialog::on_radioButton_clicked(bool checked)
{
  if(checked == 1)
    {
      ui->scatter_Combo->setEnabled(true);
      ui->scatter_Slider->setEnabled(true);
      ui->ssize_Label->setEnabled(true);
      ui->sstyle_Label->setEnabled(true);
      scatterstyleret = "Cross";
      scattersizeret = 10;
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
    if(ui->radioButton->isChecked())
        scatterstyleret = arg1;
    else
        scatterstyleret = "None";
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

void CustomLinesdialog::ajust_Graphcombo(QVector<QString> graphs)
{
  ui->graph_Combo->clear();
  for(int i =0;i<graphs.size();i++)
    {
      ui->graph_Combo->addItem(graphs.at(i));
    }
}

int CustomLinesdialog::getgraphindex()
{
  return graphindex;
}

void CustomLinesdialog::ajust_linewidthslider()
{
    ui->linewidthSlider->setTickInterval(2);
    ui->linewidthSlider->setMinimum(0);
    ui->linewidthSlider->setValue(0);
    linewidthret = 0;
    ui->linewidthSlider->setMaximum(8);
}


void CustomLinesdialog::on_graph_Combo_currentIndexChanged(int index)
{
    if (index>-1)
      graphindex = index;
}



void CustomLinesdialog::on_linewidthSlider_sliderMoved(int position)
{
    linewidthret = position;
}

int CustomLinesdialog::get_linewidth()
{
    return linewidthret;
}
