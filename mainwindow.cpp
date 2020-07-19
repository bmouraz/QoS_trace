#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QTableWidget>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qtd_graphs_vector.clear();
    qtd_graph_variable = 0;

}

MainWindow::~MainWindow()
{
  delete ui;
}
QVector<double> convert(QVector<float> in)
{

  QVector<double> out;
  out.reserve(in.size());
  std::copy(in.cbegin(), in.cend(), std::back_inserter(out));
  return out;
}


void MainWindow::makePlot()
{

  QVector<double> time, parameters;
  time = convert(alz->getTime(node,parameter));
  parameters = convert(alz->getVector(node,parameter));
//   create graph and assign data to it:
  ui->customPlot->addGraph();

  ui->customPlot->graph()->setData(time, parameters);
  qtd_graph_variable++;
  qtd_graphs_vector.push_back(qtd_graph_variable-1);
  QPen pen;
  pen.setWidth(1);
  QColor color = QColorDialog::getColor();
  pen.setColor(color);
  ui->customPlot->graph()->setPen(pen);
  ui->customPlot->graph()->setScatterStyle(QCPScatterStyle::ssNone);
  if(QString::compare(parameter, "Sent Data", Qt::CaseInsensitive) == 0 || QString::compare(parameter, "Throughput", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->xAxis->setRange(time[0], time[time.size()-1]);
      ui->customPlot->yAxis->setRange(parameters[0], parameters[parameters.size()-1]);
    }
  else if(QString::compare(parameter, "Lambda", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->xAxis->setRange(time[0], time[time.size()-1]);
      ui->customPlot->yAxis->setRange(parameters[0], parameters[parameters.size()-1]);
    }
  else if(QString::compare(parameter, "Goodput", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->xAxis->setRange(time[0], time[time.size()-1]);
      ui->customPlot->yAxis->setRange(parameters[0], parameters[parameters.size()-1]);
    }
  else
    {
      ui->customPlot->xAxis->setRange(time[0], time[time.size()-1]);
      ui->customPlot->yAxis->setRange(parameters[0], parameters[parameters.size()-1]);
    }

  ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);

  ui->customPlot->rescaleAxes();
  ui->customPlot->legend->setVisible(true);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->customPlot->legend->setFont(legendFont);
  ui->customPlot->legend->setSelectedFont(legendFont);
  ui->customPlot->legend->setSelectableParts(QCPLegend::spItems);

  ui->customPlot->replot();
  time.clear();
  parameters.clear();


}

void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.

   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.

   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
  */

  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::removeSelectedGraph()
{
  if (ui->customPlot->selectedGraphs().size() > 0)
  {
    for(int i =0; i<ui->customPlot->graphCount();i++)
    {
      if(ui->customPlot->graph(i) == ui->customPlot->selectedGraphs().first())
        {
           qtd_graphs_vector.remove(i);
           qtd_graph_variable--;
        }
    }
    ui->customPlot->removeGraph(ui->customPlot->selectedGraphs().first());
    ui->customPlot->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  ui->customPlot->clearGraphs();
  ui->customPlot->replot();
  int aux = qtd_graph_variable;
  for(int i=0;i<aux;i++)
    {
      qtd_graph_variable--;
      qtd_graphs_vector.pop_front();
    }

}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {

    if (ui->customPlot->selectedGraphs().size() > 0)
      {
        menu->addAction("Change Color", this, SLOT(changeColor()));
        menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
      }
    if (ui->customPlot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }

  menu->popup(ui->customPlot->mapToGlobal(pos));
}

void MainWindow::changeColor()
{

  QColor color = QColorDialog::getColor(Qt::yellow, this);
  if (ui->customPlot->selectedGraphs().size() > 0)
  {
    for(int i =0; i<ui->customPlot->graphCount();i++)
      {
        if(ui->customPlot->graph(i) == ui->customPlot->selectedGraphs().first())
          {
            ui->customPlot->graph(i)->setPen(color);
            ui->customPlot->replot();
          }
      }
  }


}

void MainWindow::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::combobox_config()
{
  ui->comboBox->clear();
  ui->comboBox_2->clear();
  QVector<int> node = alz->getNodes();
  for(int i = 0; i<node.size();i++)
    {
     QString nodes = QString::number(node[i]);
     ui->comboBox_2->addItem(nodes);
    }
  ui->comboBox->addItem("Lambda");
  ui->comboBox->addItem("Throughput");
  ui->comboBox->addItem("Goodput");
  ui->comboBox->addItem("Sent Data");
  ui->comboBox->addItem("Received Data");
  ui->comboBox->addItem("Jitter");
  ui->comboBox->addItem("Jitter Sum");
  ui->comboBox->addItem("Dropped Packets");
}

void MainWindow::on_actionOpen_File_triggered()
{
  QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),tr("/home/"),tr("Tracer File(*.tr)"));
  arq = new QFile(filename);
  if(arq->open(QFile::ReadOnly | QFile::Text))
  {
    alz = new analyzer(arq);
    ajust_table();
    combobox_config();
    ui->customPlot->plotLayout()->insertRow(0);
    QCPTextElement *title = new QCPTextElement(ui->customPlot, "QoS Graphs", QFont("sans", 17, QFont::Bold));
    ui->customPlot->plotLayout()->addElement(0, 0, title);
    // connect slot that ties some axis selections together (especially opposite axes):
   connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
   // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
   connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
   connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

   // make bottom and left axes transfer their ranges to top and right axes:
   connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
   connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));


   // setup policy and connect slot for context menu popup:
   ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

  }
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
  if(index>-1)
  {
    node = index;
  }
  if (parameter != "")
    {
      makePlot();
    }

}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
   parameter = arg1;
   if (parameter != "")
     {
       makePlot();
     }

}

void MainWindow::on_actionSave_Graph_2_triggered()
{
  QString selectedFilter;
  QString filename = QFileDialog::getSaveFileName(this,tr("Open File"),tr("/home/"),tr("JPG files (*.jpg);;BMP files (*.bmp);;PNG files (*.png);;PDF files(*.pdf)"), &selectedFilter);

  if (!filename.isEmpty())
  {
    if(QString::compare(selectedFilter, "JPG files (*.jpg)", Qt::CaseInsensitive) == 0)
      ui->customPlot->saveJpg(filename+".jpg", 900,720);
    else if(QString::compare(selectedFilter, "BMP files (*.bmp)", Qt::CaseInsensitive) == 0)
      ui->customPlot->saveJpg(filename+".bmp", 900,720);
    else if(QString::compare(selectedFilter, "PNG files (*.png)", Qt::CaseInsensitive) == 0)
     ui->customPlot->savePng(filename+".png", 900,720);
    else
      ui->customPlot->savePdf(filename+".pdf", 900,720);
  }
}

void MainWindow::on_actionChage_BG_triggered()
{
  if (BG == 0)
    {
      // set dark background gradient:
      QLinearGradient gradient(0, 0, 0, 400);
      gradient.setColorAt(0, QColor(90, 90, 90));
      gradient.setColorAt(0.38, QColor(105, 105, 105));
      gradient.setColorAt(1, QColor(70, 70, 70));
      ui->customPlot->setBackground(QBrush(gradient));

      BG  = 1;
    }
  else
    {

      ui->customPlot->setBackground(QBrush(QColor(255,255,255)));

      BG  = 0;
    }

}

void MainWindow::set_Scatter(QString scatter, int graph_index, int scatterSize)
{
  if(QString::compare(scatter, "Cross", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCross,scatterSize));
    }
  else if(QString::compare(scatter, "Plus", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlus,scatterSize));
    }
  else if(QString::compare(scatter, "Circle", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,scatterSize));
    }
  else if(QString::compare(scatter, "Disc", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,scatterSize));
    }
  else if(QString::compare(scatter, "Square", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare,scatterSize));
    }
  else if(QString::compare(scatter, "Diamond", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDiamond,scatterSize));
    }
  else if(QString::compare(scatter, "Star", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssStar,scatterSize));
    }
  else if(QString::compare(scatter, "Triangle", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangle,scatterSize));
    }
  else if(QString::compare(scatter, "Inverted Triangle", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssTriangleInverted,scatterSize));
    }
  else if(QString::compare(scatter, "Cross Square", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCrossSquare,scatterSize));
    }
  else if(QString::compare(scatter, "Plus Square", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusSquare,scatterSize));
    }
  else if(QString::compare(scatter, "Cross Circle", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCrossCircle,scatterSize));
    }
  else if(QString::compare(scatter, "Plus Circle", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPlusCircle,scatterSize));
    }
  else if(QString::compare(scatter, "Peace", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssPeace,scatterSize));
    }
  else if(QString::compare(scatter, "Custom", Qt::CaseInsensitive) == 0)
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCustom,scatterSize));
    }
  else
    {
      ui->customPlot->graph(graph_index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone,scatterSize));
    }
}

void MainWindow::on_actionLine_Style_Scatter_Color_triggered()
{
    CustomLinesdialog *dialog = new CustomLinesdialog;
    qDebug()<<qtd_graphs_vector.size();
    dialog->ajust_Graphcombo(qtd_graphs_vector);
    dialog->exec();
    if(dialog->result())
      {
        QPen pen;
        pen.setColor(dialog->get_Color());
        if (QString::compare(dialog->get_LineStyle(), "Solid", Qt::CaseInsensitive) == 0)
          pen.setStyle(Qt::SolidLine);
        if (QString::compare(dialog->get_LineStyle(), "Dash", Qt::CaseInsensitive) == 0)
          pen.setStyle(Qt::DashLine);
        if (QString::compare(dialog->get_LineStyle(), "Dash Dot", Qt::CaseInsensitive) == 0)
          pen.setStyle(Qt::DashDotLine);
        if (QString::compare(dialog->get_LineStyle(), "Dash Dot Dot", Qt::CaseInsensitive) == 0)
          pen.setStyle(Qt::DashDotDotLine);
        else
          pen.setStyle(Qt::DotLine);
        ui->customPlot->graph(dialog->getgraphindex())->setPen(pen);
        set_Scatter(dialog->get_ScatterStyle(),dialog->getgraphindex(),dialog->get_ScatterSize());
        ui->customPlot->replot();

      }
}

void MainWindow::ajust_table()
{
  QStringList header;
  ui->orderTable->setColumnCount(7);
  ui->orderTable->setRowCount(0);
  header << "Node" << "Lambda" << "Throughput"<<"Goodput"<<"Sent Data"<<"Jitter Sum"<<"Dropped Packets";
  ui->orderTable->setHorizontalHeaderLabels(header);

  // insere linha a linha os valores da tabela
  for(int i=0; i<alz->getNodes().size();i++){
      ui->orderTable->insertRow(ui->orderTable->rowCount());
      int currentLine = ui->orderTable->rowCount() -1;

      QTableWidgetItem* itemnode = new QTableWidgetItem;
      itemnode->setData(Qt::DisplayRole,i);
      ui->orderTable->setItem(currentLine,NODE,itemnode);

      QTableWidgetItem* itemlambda = new QTableWidgetItem;
      itemlambda->setData(Qt::DisplayRole,(alz->getLambda(i)));
      ui->orderTable->setItem(currentLine,LAMBDA,itemlambda);

      QTableWidgetItem* itemthrpt = new QTableWidgetItem;
      itemthrpt->setData(Qt::DisplayRole,(alz->getThrpt(i)));
      ui->orderTable->setItem(currentLine,THROUGHPUT,itemthrpt);

      QTableWidgetItem* itemgood = new QTableWidgetItem;
      itemgood->setData(Qt::DisplayRole,(alz->getGood(i)));
      ui->orderTable->setItem(currentLine,GOODPUT,itemgood);

      QTableWidgetItem* itemsent = new QTableWidgetItem;
      int senttotal = alz->getSentDatatotal(i);
      itemsent->setData(Qt::DisplayRole,(senttotal));
      ui->orderTable->setItem(currentLine,SENTDATA,itemsent);


      QTableWidgetItem* itemjitter = new QTableWidgetItem;
      itemjitter->setData(Qt::DisplayRole,(alz->getJitterSum(i)));
      ui->orderTable->setItem(currentLine,JITTERSUM,itemjitter);

      QTableWidgetItem* itemdropped = new QTableWidgetItem;
      itemdropped->setData(Qt::DisplayRole,(alz->getPckt(i)));
      ui->orderTable->setItem(currentLine,DROPPED,itemdropped);

  }
  ui->orderTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//  ui->orderTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
