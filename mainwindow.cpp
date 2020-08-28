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
    ui->customPlot->plotLayout()->insertRow(0);
    QCPTextElement *title = new QCPTextElement(ui->customPlot, "QoS Graphs", QFont("sans", 17, QFont::Bold));
    connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
    connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
    ui->customPlot->xAxis->setLabel("Time");
    ui->customPlot->yAxis->setLabel("Parameter");
    ui->customPlot->plotLayout()->addElement(0, 0, title);
    ui->actionChange_Scale_Log->setCheckable(true);
    ui->actionChange_Scale_Linear->setCheckable(true);
    ui->actionSubgrid->setCheckable(true);
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


void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->customPlot->replot();
    }
  }
}


void MainWindow::makePlot()
{
  if(chart_checker == 0)
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
      ui->customPlot->graph(qtd_graph_variable-1)->setPen(pen);
      ui->customPlot->graph(qtd_graph_variable-1)->setScatterStyle(QCPScatterStyle::ssNone);
      ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                        QCP::iSelectLegend | QCP::iSelectPlottables);



      ui->customPlot->legend->setVisible(true);
      QFont legendFont = font();
      legendFont.setPointSize(10);
      ui->customPlot->legend->setFont(legendFont);
      ui->customPlot->legend->setSelectedFont(legendFont);
      ui->customPlot->legend->setSelectableParts(QCPLegend::spItems);
      QSharedPointer <QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
      fixedTicker->setTickStep(ticker_spacing_x);
      ui->customPlot->xAxis->setTicker(fixedTicker);
      ui->customPlot->rescaleAxes(true);
      ui->customPlot->xAxis->rescale(true);
      ui->customPlot->xAxis2->rescale(true);
      ui->customPlot->yAxis->rescale(true);
      ui->customPlot->yAxis2->rescale(true);
      ui->customPlot->xAxis->setTicks(true);
      ui->customPlot->replot();
      time.clear();
      parameters.clear();

  }

  else
  {
    if(node == -1)
    {
        ui->customPlot->clearPlottables();
        QCPBars *bar_parameters = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
        bar_parameters->setAntialiased(true);
        bar_parameters->setStackingGap(1);
        bar_parameters->setName(parameter);
        QColor bar_color = QColorDialog::getColor(Qt::yellow, this);
        bar_parameters->setPen(QPen(bar_color));
        bar_parameters->setBrush(bar_color);
        QVector<QString> labels;
        QVector<double> ticks;
        QVector<double> bar_parameterdata;
        for(int i = 0; i<alz->getNodes().size();i++)
        {
            ticks.append(i);
            labels.append("Node "+QString::number(i));
            bar_parameterdata = (convert(alz->getValues(parameter)));
        }
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTicks(ticks, labels);
        ui->customPlot->xAxis->setTicker(textTicker);
        ui->customPlot->xAxis->setTickLabelRotation(60);
        ui->customPlot->xAxis->setSubTicks(false);
        ui->customPlot->xAxis->setTickLength(0, 4);
        ui->customPlot->xAxis->setRange(0, 8);
        ui->customPlot->xAxis->setBasePen(QPen(Qt::black));
        ui->customPlot->xAxis->setTickPen(QPen(Qt::black));
        ui->customPlot->xAxis->grid()->setVisible(true);
        ui->customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
        ui->customPlot->xAxis->setTickLabelColor(Qt::black);
        ui->customPlot->xAxis->setLabelColor(Qt::black);

        ui->customPlot->yAxis->setRange(0, 12.1);
        ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
        ui->customPlot->yAxis->setBasePen(QPen(Qt::black));
        ui->customPlot->yAxis->setTickPen(QPen(Qt::black));
        ui->customPlot->yAxis->setSubTickPen(QPen(Qt::black));
        ui->customPlot->yAxis->grid()->setSubGridVisible(true);
        ui->customPlot->yAxis->setTickLabelColor(Qt::black);
        ui->customPlot->yAxis->setLabelColor(Qt::black);
        ui->customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
        ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

        bar_parameters->setData(ticks, bar_parameterdata);

        ui->customPlot->legend->setVisible(true);
        ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
        ui->customPlot->legend->setBrush(QColor(0, 0, 0, 100));
        ui->customPlot->legend->setBorderPen(Qt::NoPen);
        QFont legendFont = font();
        legendFont.setPointSize(10);
        ui->customPlot->legend->setFont(legendFont);
        ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
    else
    {
        ui->customPlot->clearPlottables();
        QCPBars *bar_node = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
        bar_node->setAntialiased(true);
        bar_node->setStackingGap(1);
        bar_node->setName(parameter);
        QColor bar_color = QColorDialog::getColor(Qt::yellow, this);
        bar_node->setPen(QPen(bar_color));
        bar_node->setBrush(bar_color);
        QVector<QString> labels;
        labels<<"Lambda"<<"Throughput"<<"Goodput"<<"Sent Data"<<"Received Data"<<"Jitter Sum"<<"Dropped Packets";
        QVector<double> ticks;
        QVector<double> bar_nodedata;
        for(int i = 0; i<alz->getValues_nodes(node).size();i++)
        {
            ticks.append(i);
            bar_nodedata = (convert(alz->getValues_nodes(node)));
        }
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTicks(ticks, labels);
        ui->customPlot->xAxis->setTicker(textTicker);
        ui->customPlot->xAxis->setTickLabelRotation(60);
        ui->customPlot->xAxis->setSubTicks(false);
        ui->customPlot->xAxis->setTickLength(0, 4);
        ui->customPlot->xAxis->setRange(0, 8);
        ui->customPlot->xAxis->setBasePen(QPen(Qt::black));
        ui->customPlot->xAxis->setTickPen(QPen(Qt::black));
        ui->customPlot->xAxis->grid()->setVisible(true);
        ui->customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
        ui->customPlot->xAxis->setTickLabelColor(Qt::black);
        ui->customPlot->xAxis->setLabelColor(Qt::black);

//        double spacing = 0.25;
//        for(int x=1; x < 11; x++){
//            double y = x+2;
//            //Creating and configuring an item
//            QCPItemText *textLabel = new QCPItemText(ui->customPlot);
//            ui->customPlot->add;
//            textLabel->setClipToAxisRect(false);
//            textLabel->position->setAxes(ui->customPlot->xAxis,ui->customPlot->yAxis);
//            textLabel->position->setType(QCPItemPosition::ptPlotCoords);
//            //placing the item over the bar with a spacing of 0.25
//            textLabel->position->setCoords(x,y+spacing);
//            //Customizing the item
//            textLabel->setText(QString::number(y));
//            textLabel->setFont(QFont(font().family(), 9));
//            textLabel->setPen(QPen(Qt::black));
//        }

        ui->customPlot->yAxis->setRange(0, 12.1);
        ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
        ui->customPlot->yAxis->setBasePen(QPen(Qt::black));
        ui->customPlot->yAxis->setTickPen(QPen(Qt::black));
        ui->customPlot->yAxis->setSubTickPen(QPen(Qt::black));
        ui->customPlot->yAxis->grid()->setSubGridVisible(true);
        ui->customPlot->yAxis->setTickLabelColor(Qt::black);
        ui->customPlot->yAxis->setLabelColor(Qt::black);
        ui->customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
        ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

        bar_node->setData(ticks, bar_nodedata);

        ui->customPlot->legend->setVisible(true);
        ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
        ui->customPlot->legend->setBrush(QColor(0, 0, 0, 100));
        ui->customPlot->legend->setBorderPen(Qt::NoPen);
        QFont legendFont = font();
        legendFont.setPointSize(10);
        ui->customPlot->legend->setFont(legendFont);
        ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        ui->customPlot->rescaleAxes(true);
        ui->customPlot->replot();
    }
  }

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
  if(chart_checker == 0)
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
      ui->comboBox->setCurrentIndex(ui->comboBox->findData("Lambda", Qt::DisplayRole));
      ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findData("0", Qt::DisplayRole));
  }
  else
  {
      ui->comboBox->clear();
      ui->comboBox_2->clear();
      ui->comboBox->addItem("");
      ui->comboBox_2->addItem("");
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
      ui->comboBox->addItem("Jitter Sum");
      ui->comboBox->addItem("Dropped Packets");
      ui->comboBox->setCurrentIndex(ui->comboBox->findData("Lambda", Qt::DisplayRole));
  }

}

void MainWindow::on_actionOpen_File_triggered()
{
  QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),tr("/home/"),tr("Tracer File(*.tr)"));
  arq = new QFile(filename);
  if(arq->open(QFile::ReadOnly | QFile::Text))
  {
    alz = new analyzer(arq);
    ui->actionChange_Scale_Log->setChecked(false);
    ui->actionChange_Scale_Linear->setChecked(true);
    ui->actionSubgrid->setChecked(false);
    int aux = qtd_graph_variable;
    for(int i=0;i<aux;i++)
      {
        qtd_graph_variable--;
        qtd_graphs_vector.pop_front();
      }
    ui->customPlot->clearPlottables();
    ui->customPlot->clearGraphs();
    scale_checker = 0;
    ui->customPlot->xAxis->setTicks(false);
    ajust_table();
    combobox_config();

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
  if(chart_checker == 0)
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
  else
  {
      if(index>-1)
      {
        node = index;
      }
      ui->comboBox->setCurrentIndex(ui->comboBox->findData("", Qt::DisplayRole));
      parameter = "";
      if (node != 0)
        makePlot();
  }

}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
   if(chart_checker == 0)
   {
       parameter = arg1;
       if (parameter != "")
         {
           makePlot();
         }
   }
   else
   {
       parameter = arg1;
       if (parameter != "")
         {
           ui->comboBox_2->setCurrentIndex(ui->comboBox_2->findData("", Qt::DisplayRole));
           node = -1;
           makePlot();
         }
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
    if(chart_checker == 0)
    {
        CustomLinesdialog *dialog = new CustomLinesdialog;
        dialog->ajust_Graphcombo(qtd_graphs_vector);
        dialog->exec();
        if(dialog->result())
          {
            QPen pen;
            pen.setColor(dialog->get_Color());
            if (QString::compare(dialog->get_LineStyle(), "Solid", Qt::CaseInsensitive) == 0)
              pen.setStyle(Qt::SolidLine);
            else if (QString::compare(dialog->get_LineStyle(), "Dash", Qt::CaseInsensitive) == 0)
              pen.setStyle(Qt::DashLine);
            else if (QString::compare(dialog->get_LineStyle(), "Dash Dot", Qt::CaseInsensitive) == 0)
              pen.setStyle(Qt::DashDotLine);
            else if (QString::compare(dialog->get_LineStyle(), "Dash Dot Dot", Qt::CaseInsensitive) == 0)
              pen.setStyle(Qt::DashDotDotLine);
            else
              pen.setStyle(Qt::DotLine);
            ui->customPlot->graph(dialog->getgraphindex())->setPen(pen);
            set_Scatter(dialog->get_ScatterStyle(),dialog->getgraphindex(),dialog->get_ScatterSize());
            ui->customPlot->replot();

          }
    }
    else
    {
        makePlot();

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

void MainWindow::on_actionExport_File_triggered()
{
    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(this,tr("Open File"),tr("/home/"),tr("CSV(,) files (*.csv);;PRN files (*.prn);; CSV files(;) (*.tsv)"), &selectedFilter);

    if (!filename.isEmpty())
    {
      if(QString::compare(selectedFilter, "CSV(,) files (*.csv)", Qt::CaseInsensitive) == 0)
        alz->transform2file("csv",filename+".csv");
      else if(QString::compare(selectedFilter, "PRN files (*.prn)", Qt::CaseInsensitive) == 0)
        alz->transform2file("prn",filename+".prn");
      else
      {
        alz->transform2file("csv",filename+".csv");
      }
    }

}

void MainWindow::on_pushButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Open File"),tr("/home/"),tr("PDF files (*.pdf)"));
    const int columns = ui->orderTable->columnCount();
    const int rows = ui->orderTable->rowCount();
    QTextDocument doc;
    QTextCursor cursor(&doc);
    QTextTableFormat tableFormat;
    tableFormat.setHeaderRowCount(1);
    tableFormat.setAlignment(Qt::AlignHCenter);
    tableFormat.setCellPadding(0);
    tableFormat.setCellSpacing(0);
    tableFormat.setBorder(1);
    tableFormat.setBorderBrush(QBrush(Qt::SolidPattern));
    tableFormat.clearColumnWidthConstraints();
    QTextTable *textTable = cursor.insertTable(rows + 1, columns, tableFormat);
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setBackground(QColor("#DADADA"));
    for (int i = 0; i < columns; i++) {
        QTextTableCell cell = textTable->cellAt(0, i);
        cell.setFormat(tableHeaderFormat);
        QTextCursor cellCursor = cell.firstCursorPosition();
        cellCursor.insertText(ui->orderTable->horizontalHeaderItem(i)->data(Qt::DisplayRole).toString());
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            QTableWidgetItem *item = ui->orderTable->item(i, j);
            if (!item || item->text().isEmpty()) {
                ui->orderTable->setItem(i, j, new QTableWidgetItem("0"));
            }

            QTextTableCell cell = textTable->cellAt(i+1, j);
            QTextCursor cellCursor = cell.firstCursorPosition();
            cellCursor.insertText(ui->orderTable->item(i, j)->text());
        }
    }
    cursor.movePosition(QTextCursor::End);
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName(filename);
    doc.setDocumentMargin(0);
    doc.setTextWidth(5);
    doc.print(&printer);
}

void MainWindow::on_actionChange_Bar_Line_Chart_triggered()
{
    if (chart_checker != 1)
    {
        QMessageBox msgBox;
        msgBox.setText("Do you want to change to Bar?");
        msgBox.setStandardButtons(QMessageBox::Ok  | QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == 1024)
        {
            ui->customPlot->clearPlottables();
            int aux = qtd_graph_variable;
            for(int i=0;i<aux;i++)
              {
                qtd_graph_variable--;
                qtd_graphs_vector.pop_front();
              }
            chart_checker = 1;
            combobox_config();
        }

    }
}

void MainWindow::on_actionLine_Chart_triggered()
{
    if(chart_checker != 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Do you want to change to Line?");
        msgBox.setStandardButtons(QMessageBox::Ok  | QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == 1024)
        {
            chart_checker = 0;
            ui->customPlot->clearPlottables();
            combobox_config();

        }
    }

}

void MainWindow::on_actionChange_Scale_Log_triggered()
{
    scale_checker = 1;
    ui->actionChange_Scale_Log->setChecked(true);
    ui->actionChange_Scale_Linear->setChecked(false);
    ui->customPlot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    ui->customPlot->yAxis2->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    ui->customPlot->yAxis->setTicker(logTicker);
    ui->customPlot->yAxis2->setTicker(logTicker);
    ui->customPlot->yAxis->setNumberFormat("eb"); // e = exponential, b = beautiful decimal powers
    ui->customPlot->yAxis->setNumberPrecision(0);
    ui->customPlot->replot();
}

void MainWindow::on_actionChange_Scale_Linear_triggered()
{
    ui->actionChange_Scale_Log->setChecked(false);
    ui->actionChange_Scale_Linear->setChecked(true);
    scale_checker = 0;
    ui->customPlot->yAxis->setScaleType(QCPAxis::stLinear);
    ui->customPlot->yAxis2->setScaleType(QCPAxis::stLinear);
    QSharedPointer <QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    fixedTicker->setTickStep(ticker_spacing_y);
    ui->customPlot->yAxis->setTicker(fixedTicker);

    ui->customPlot->yAxis->setNumberFormat("fb");
    ui->customPlot->xAxis->setTicks(true);
    ui->customPlot->xAxis->rescale();
    ui->customPlot->replot();
}

void MainWindow::on_actionSubgrid_triggered()
{
    if(ui->actionSubgrid->isChecked())
    {
        ui->actionSubgrid->setChecked(true);
        ui->customPlot->xAxis->grid()->setSubGridVisible(true);
        ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    }
    else
    {
        ui->actionSubgrid->setChecked(false);
        ui->customPlot->xAxis->grid()->setSubGridVisible(false);
        ui->customPlot->yAxis->grid()->setSubGridVisible(false);
    }
    ui->customPlot->replot();
}

void MainWindow::on_actionChange_Xticks_triggered()
{
    if(chart_checker == 0)
    {
        inputicksDialog *dialog = new inputicksDialog(ticker_spacing_x,ticker_spacing_y);
        dialog->exec();
        if(dialog->result())
        {
            ticker_spacing_x = dialog->get_xticks();
            ticker_spacing_y = dialog->get_yticks();
            qDebug()<<ticker_spacing_x<<","<<ticker_spacing_y;
            QSharedPointer <QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
            fixedTicker->setTickStep(ticker_spacing_x);
            ui->customPlot->xAxis->setTicker(fixedTicker);
            fixedTicker->setTickStep(ticker_spacing_y);
            ui->customPlot->yAxis->setTicker(fixedTicker);
            ui->customPlot->xAxis->rescale();
            ui->customPlot->yAxis->rescale();
            ui->customPlot->replot();
        }
    }
    else
    {
        bool ok;
        double d = QInputDialog::getDouble(this, tr("YTicks"),
                                             tr("YStick Step:"), ticker_spacing_y, 1, 100000000, 2, &ok,
                                             Qt::WindowFlags(), 1);
       if (ok)
       {
           ticker_spacing_y = d;
           QSharedPointer <QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
           fixedTicker->setTickStep(ticker_spacing_y);
           ui->customPlot->yAxis->setTicker(fixedTicker);
           ui->customPlot->yAxis->rescale();
           ui->customPlot->replot();

       }
    }

}
