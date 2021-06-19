#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->setWindowTitle("QoS Trace");
    qtd_graphs_vector.clear();
    qtd_graph_variable = 0;
    on_actionWhite_triggered();
    close_rectangle = 0;

}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actionOpen_File_triggered()
{
  if (count_file_entries == 0)
  {
      directory = QFileDialog::getOpenFileName(this,tr("Open File"),tr("/home/"),tr("Tracer File(*.tr)"));
      arq = new QFile(directory);
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
        ui->customPlot_bars->clearPlottables();
        ui->customPlot_bars->clearGraphs();

        scale_checker = 0;
        ui->customPlot->xAxis->setTicks(false);
        combobox_config();
        ui->customPlot->plotLayout()->insertRow(0);
        ui->customPlot_bars->plotLayout()->insertRow(0);
        QCPTextElement *title = new QCPTextElement(ui->customPlot, "QoS Graphs", QFont("sans", 17, QFont::Bold));
        QCPTextElement *title_bars = new QCPTextElement(ui->customPlot_bars, "QoS Graphs", QFont("sans", 17, QFont::Bold));

        connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
        connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
        connect(title_bars, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick_bars(QMouseEvent*)));
        connect(ui->customPlot_bars, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick_bars(QCPAxis*,QCPAxis::SelectablePart)));

        ui->customPlot->xAxis->setLabel("Time");
        ui->customPlot->yAxis->setLabel("Parameter");
        ui->customPlot->plotLayout()->addElement(0, 0, title);
        ui->customPlot_bars->plotLayout()->addElement(0,0, title_bars);
        ui->actionChange_Scale_Log->setCheckable(true);
        ui->actionChange_Scale_Linear->setCheckable(true);
        ui->actionSubgrid->setCheckable(true);
        ui->actionBlack->setCheckable(true);
        ui->actionWhite->setCheckable(true);


        // connect slot that ties some axis selections together (especially opposite axes):
       connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
       connect(ui->customPlot_bars, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged_bars()));
       // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
       connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
       connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));

       connect(ui->customPlot_bars, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress_bars()));
       connect(ui->customPlot_bars, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel_bars()));

       // make bottom and left axes transfer their ranges to top and right axes:
       connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
       connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));


       // setup policy and connect slot for context menu popup:
       ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

       ui->customPlot_bars->setContextMenuPolicy(Qt::CustomContextMenu);
       connect(ui->customPlot_bars, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest_bars(QPoint)));


       ui->actionWhite->setChecked(true);
       ui->customPlot->replot();
       ui->customPlot_bars->replot();
       count_file_entries++;
      }
  }
  else
  {
      QString filename = QFileDialog::getOpenFileName(this,tr("Open File"),tr(directory.toUtf8().constData()),tr("Tracer File(*.tr)"));
      directory = filename;
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
        combobox_config();


  }
  }

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
  if(chart_checker == 0)
  {
      QVector<double> time, parameters;
      time = convert(alz->getTime(node,parameter));
      parameters = convert(alz->getVector(node,parameter));
    //   create graph and assign data to it:
      ui->customPlot->addGraph();
      QString graph_name;
      if(node == ui->comboBox_2->count()-1)
        graph_name.append(parameter + " Average");
      else
        graph_name.append(parameter + " x" +" Node " + QString::number(node));
      ui->customPlot->graph()->setName(graph_name);
      ui->customPlot->graph()->setData(time, parameters);
      qtd_graph_variable++;
      qtd_graphs_vector.push_back(graph_name);
      QPen pen;
      pen.setWidth(1);
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
    if(node_bar == -1)
    {
        ui->customPlot_bars->clearPlottables();
        QCPBars *bar_parameters = new QCPBars(ui->customPlot_bars->xAxis, ui->customPlot_bars->yAxis);
        ui->customPlot_bars->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                          QCP::iSelectLegend | QCP::iSelectPlottables);
        bar_parameters->setAntialiased(true);
        bar_parameters->setStackingGap(1);
        bar_parameters->setName(parameter_bar);
        QVector<QString> labels;
        QVector<double> ticks;
        QVector<double> bar_parameterdata;
        for(int i = 0; i<alz->getNodes().size();i++)
        {
            ticks.append(i);
            labels.append("Node "+QString::number(i));
            bar_parameterdata = (convert(alz->getValues(parameter_bar)));
        }
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTicks(ticks, labels);
        ui->customPlot_bars->xAxis->setTicker(textTicker);
        ui->customPlot_bars->xAxis->setTickLabelRotation(60);
        ui->customPlot_bars->xAxis->setSubTicks(false);
        ui->customPlot_bars->xAxis->setTickLength(0, 4);
        ui->customPlot_bars->xAxis->setRange(0, 8);
        ui->customPlot_bars->xAxis->setBasePen(QPen(Qt::black));
        ui->customPlot_bars->xAxis->setTickPen(QPen(Qt::black));
        ui->customPlot_bars->xAxis->grid()->setVisible(true);
        ui->customPlot_bars->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
        ui->customPlot_bars->xAxis->setTickLabelColor(Qt::black);
        ui->customPlot_bars->xAxis->setLabelColor(Qt::black);

        ui->customPlot_bars->yAxis->setRange(0, 12.1);
        ui->customPlot_bars->yAxis->setPadding(5); // a bit more space to the left border
        ui->customPlot_bars->yAxis->setBasePen(QPen(Qt::black));
        ui->customPlot_bars->yAxis->setTickPen(QPen(Qt::black));
        ui->customPlot_bars->yAxis->setSubTickPen(QPen(Qt::black));
        ui->customPlot_bars->yAxis->grid()->setSubGridVisible(true);
        ui->customPlot_bars->yAxis->setTickLabelColor(Qt::black);
        ui->customPlot_bars->yAxis->setLabelColor(Qt::black);
        ui->customPlot_bars->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
        ui->customPlot_bars->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

        bar_parameters->setData(ticks, bar_parameterdata);

        ui->customPlot_bars->legend->setVisible(true);
        ui->customPlot_bars->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
        ui->customPlot_bars->legend->setBrush(QColor(0, 0, 0, 100));
        ui->customPlot_bars->legend->setBorderPen(Qt::NoPen);
        QFont legendFont = font();
        legendFont.setPointSize(10);
        ui->customPlot_bars->legend->setFont(legendFont);
        ui->customPlot_bars->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        ui->customPlot_bars->rescaleAxes(true);
        ui->customPlot_bars->replot();
    }
    else
    {
        ui->customPlot_bars->clearPlottables();
        ui->customPlot_bars->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
        QCPBars *bar_node = new QCPBars(ui->customPlot_bars->xAxis, ui->customPlot_bars->yAxis);
        bar_node->setAntialiased(true);
        bar_node->setStackingGap(1);
        bar_node->setName(parameter_bar);
        QVector<QString> labels;
        labels<<"Lambda"<<"Throughput"<<"Goodput"<<"Sent Data"<<"Received Data"<<"Jitter Sum"<<"Dropped Packets";
        QVector<double> ticks;
        QVector<double> bar_nodedata;
        for(int i = 0; i<alz->getValues_nodes(node_bar).size();i++)
        {
            ticks.append(i);
            bar_nodedata = (convert(alz->getValues_nodes(node_bar)));
        }
        QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
        textTicker->addTicks(ticks, labels);
        ui->customPlot_bars->xAxis->setTicker(textTicker);
        ui->customPlot_bars->xAxis->setTickLabelRotation(60);
        ui->customPlot_bars->xAxis->setSubTicks(false);
        ui->customPlot_bars->xAxis->setTickLength(0, 4);
        ui->customPlot_bars->xAxis->setRange(0, 8);
        ui->customPlot_bars->xAxis->setBasePen(QPen(Qt::black));
        ui->customPlot_bars->xAxis->setTickPen(QPen(Qt::black));
        ui->customPlot_bars->xAxis->grid()->setVisible(true);
        ui->customPlot_bars->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
        ui->customPlot_bars->xAxis->setTickLabelColor(Qt::black);
        ui->customPlot_bars->xAxis->setLabelColor(Qt::black);


        ui->customPlot_bars->yAxis->setRange(0, 12.1);
        ui->customPlot_bars->yAxis->setPadding(5); // a bit more space to the left border
        ui->customPlot_bars->yAxis->setBasePen(QPen(Qt::black));
        ui->customPlot_bars->yAxis->setTickPen(QPen(Qt::black));
        ui->customPlot_bars->yAxis->setSubTickPen(QPen(Qt::black));
        ui->customPlot_bars->yAxis->grid()->setSubGridVisible(true);
        ui->customPlot_bars->yAxis->setTickLabelColor(Qt::black);
        ui->customPlot_bars->yAxis->setLabelColor(Qt::black);
        ui->customPlot_bars->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
        ui->customPlot_bars->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

        bar_node->setData(ticks, bar_nodedata);

        ui->customPlot_bars->legend->setVisible(true);
        ui->customPlot_bars->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
        ui->customPlot_bars->legend->setBrush(QColor(0, 0, 0, 100));
        ui->customPlot_bars->legend->setBorderPen(Qt::NoPen);
        QFont legendFont = font();
        legendFont.setPointSize(10);
        ui->customPlot_bars->legend->setFont(legendFont);
        ui->customPlot_bars->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
        ui->customPlot_bars->rescaleAxes(true);
        ui->customPlot_bars->replot();

    }
  }

}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    axis_inputDialog *dialog = new axis_inputDialog(axis->label(),axis->axisType());
    if (ui->actionBlack->isChecked())
        dialog->setStyleSheet("background-color: rgb(50, 50, 50)");
    else
        dialog->setStyleSheet("background-color: rgb(200, 200, 200)");
    dialog->exec();
    if(dialog->result())
    {
        if(axis->axisType() == dialog->get_position())
        {
            axis->setLabel(dialog->get_new_label());
        }
        else
        {
            axis->setLabel("");
            if(dialog->get_position() == 1)
            {
                ui->customPlot->yAxis->setLabel(dialog->get_new_label());
            }
            else if(dialog->get_position() == 2)
            {
                ui->customPlot->yAxis2->setLabel(dialog->get_new_label());
            }
            else if(dialog->get_position() == 4)
            {
                ui->customPlot->xAxis2->setLabel(dialog->get_new_label());
            }
            else
            {
                ui->customPlot->xAxis->setLabel(dialog->get_new_label());
            }

        }
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
    QString newTitle = QInputDialog::getText(this, "New Title", "New plot title:", QLineEdit::Normal, title->text(), &ok);
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
  if(ui->tabWidget->currentIndex() == 0)
  {
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
  else
  {
      ui->customPlot_bars->graph(0)->setPen(color);
      ui->customPlot_bars->replot();
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

void MainWindow::axisLabelDoubleClick_bars(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "Change axis label", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->customPlot_bars->replot();
    }
  }
}

void MainWindow::titleDoubleClick_bars(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title_bars = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "New Title", "New plot title:", QLineEdit::Normal, title_bars->text(), &ok);
    if (ok)
    {
      title_bars->setText(newTitle);
      ui->customPlot_bars->replot();
    }
  }
}

void MainWindow::selectionChanged_bars()
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
  if (ui->customPlot_bars->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot_bars->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot_bars->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot_bars->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot_bars->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot_bars->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot_bars->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot_bars->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot_bars->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot_bars->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot_bars->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot_bars->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }

  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot_bars->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot_bars->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot_bars->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::mousePress_bars()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged

  if (ui->customPlot_bars->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot_bars->axisRect()->setRangeDrag(ui->customPlot_bars->xAxis->orientation());
  else if (ui->customPlot_bars->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot_bars->axisRect()->setRangeDrag(ui->customPlot_bars->yAxis->orientation());
  else
    ui->customPlot_bars->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel_bars()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed

  if (ui->customPlot_bars->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot_bars->axisRect()->setRangeZoom(ui->customPlot_bars->xAxis->orientation());
  else if (ui->customPlot_bars->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot_bars->axisRect()->setRangeZoom(ui->customPlot_bars->yAxis->orientation());
  else
    ui->customPlot_bars->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::contextMenuRequest_bars(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);

  if (ui->customPlot_bars->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend_bars()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend_bars()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend_bars()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend_bars()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend_bars()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {

    if (ui->customPlot_bars->selectedGraphs().size() > 0)
      {
        menu->addAction("Change Color", this, SLOT(changeColor()));
      }
  }

  menu->popup(ui->customPlot_bars->mapToGlobal(pos));
}

void MainWindow::moveLegend_bars()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->customPlot_bars->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->customPlot_bars->replot();
    }
  }
}

void MainWindow::combobox_config()
{

      ui->comboBox->clear();
      ui->comboBox_2->clear();
      ui->comboBox_3->clear();
      ui->comboBox_4->clear();
      ui->comboBox_3->addItem("All");
      ui->comboBox_3->setItemData(0, 0, Qt::UserRole - 1);
      ui->comboBox_4->addItem("All");
      ui->comboBox_4->setItemData(0, 0, Qt::UserRole - 1);

      QVector<int> node = alz->getNodes();
      for(int i = 0; i<node.size();i++)
        {
         QString nodes = QString::number(node[i]);
         ui->comboBox_2->addItem(nodes);
         ui->comboBox_4->addItem(nodes);
        }

      ui->comboBox_2->addItem("Average");

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


      ui->comboBox_3->addItem("Lambda");
      ui->comboBox_3->addItem("Throughput");
      ui->comboBox_3->addItem("Goodput");
      ui->comboBox_3->addItem("Sent Data");
      ui->comboBox_3->addItem("Received Data");
      ui->comboBox_3->addItem("Jitter Sum");
      ui->comboBox_3->addItem("Dropped Packets");
      ui->comboBox_3->setCurrentIndex(ui->comboBox_3->findData("Lambda", Qt::DisplayRole));
      ui->comboBox_4->setCurrentIndex(ui->comboBox_4->findData("All", Qt::DisplayRole));
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
  chart_checker = 0;
  if(index>-1)
  {
    node = index;
  }
  if (parameter != "")
  {
    makePlot();
  }
}

void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    chart_checker = 1;
    if(index>-1)
    {
      node_bar = index;
    }
    ui->comboBox_3->setCurrentIndex(ui->comboBox_3->findData("All", Qt::DisplayRole));
    parameter_bar = "All";
    if (node_bar != 0)
      makePlot();

}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{

   chart_checker = 0;
   parameter = arg1;
   if (parameter != "")
   {
     makePlot();
   }
}

void MainWindow::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
  chart_checker = 1;
  if (arg1 != "All")
    {
      ui->comboBox_4->setCurrentIndex(ui->comboBox_4->findData("All", Qt::DisplayRole));
      node_bar = -1;
      parameter_bar = arg1;
      ui->comboBox_3->setCurrentIndex(ui->comboBox_3->findData(arg1, Qt::DisplayRole));
      makePlot();
    }
}

void MainWindow::on_actionSave_Graph_2_triggered()
{
  QString selectedFilter;
  QString filename = QFileDialog::getSaveFileName(this,tr("Open File"),tr("/home/"),tr("JPG files (*.jpg);;BMP files (*.bmp);;PNG files (*.png);;PDF files(*.pdf)"), &selectedFilter);

  if (!filename.isEmpty())
  {
    if(ui->tabWidget->currentIndex() == 0)
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
    else if(ui->tabWidget->currentIndex() == 1)
    {
        if(QString::compare(selectedFilter, "JPG files (*.jpg)", Qt::CaseInsensitive) == 0)
          ui->customPlot_bars->saveJpg(filename+".jpg", 900,720);
        else if(QString::compare(selectedFilter, "BMP files (*.bmp)", Qt::CaseInsensitive) == 0)
          ui->customPlot_bars->saveJpg(filename+".bmp", 900,720);
        else if(QString::compare(selectedFilter, "PNG files (*.png)", Qt::CaseInsensitive) == 0)
         ui->customPlot_bars->savePng(filename+".png", 900,720);
        else
          ui->customPlot_bars->savePdf(filename+".pdf", 900,720);
    }

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
    if(ui->tabWidget->currentIndex()==0)
    {
        CustomLinesdialog *dialog = new CustomLinesdialog;
        dialog->ajust_Graphcombo(qtd_graphs_vector);
        if (ui->actionBlack->isChecked())
            dialog->setStyleSheet("background-color: rgb(50, 50, 50)");
        else
            dialog->setStyleSheet("background-color: rgb(200, 200, 200)");
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
            pen.setWidth(dialog->get_linewidth());
            set_Scatter(dialog->get_ScatterStyle(),dialog->getgraphindex(),dialog->get_ScatterSize());
            ui->customPlot->graph(dialog->getgraphindex())->setPen(pen);
            ui->customPlot->replot();

          }
    }
    else if(ui->tabWidget->currentIndex()==1)
    {
        if(node_bar == -1)
        {
            ui->customPlot_bars->clearPlottables();
            QCPBars *bar_parameters = new QCPBars(ui->customPlot_bars->xAxis, ui->customPlot_bars->yAxis);
            bar_parameters->setAntialiased(true);
            bar_parameters->setStackingGap(1);
            bar_parameters->setName(parameter_bar);
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
                bar_parameterdata = (convert(alz->getValues(parameter_bar)));
            }
            QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
            textTicker->addTicks(ticks, labels);
            ui->customPlot_bars->xAxis->setTicker(textTicker);
            ui->customPlot_bars->xAxis->setTickLabelRotation(60);
            ui->customPlot_bars->xAxis->setSubTicks(false);
            ui->customPlot_bars->xAxis->setTickLength(0, 4);
            ui->customPlot_bars->xAxis->setRange(0, 8);
            ui->customPlot_bars->xAxis->setBasePen(QPen(Qt::black));
            ui->customPlot_bars->xAxis->setTickPen(QPen(Qt::black));
            ui->customPlot_bars->xAxis->grid()->setVisible(true);
            ui->customPlot_bars->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
            ui->customPlot_bars->xAxis->setTickLabelColor(Qt::black);
            ui->customPlot_bars->xAxis->setLabelColor(Qt::black);

            ui->customPlot_bars->yAxis->setRange(0, 12.1);
            ui->customPlot_bars->yAxis->setPadding(5); // a bit more space to the left border
            ui->customPlot_bars->yAxis->setBasePen(QPen(Qt::black));
            ui->customPlot_bars->yAxis->setTickPen(QPen(Qt::black));
            ui->customPlot_bars->yAxis->setSubTickPen(QPen(Qt::black));
            ui->customPlot_bars->yAxis->grid()->setSubGridVisible(true);
            ui->customPlot_bars->yAxis->setTickLabelColor(Qt::black);
            ui->customPlot_bars->yAxis->setLabelColor(Qt::black);
            ui->customPlot_bars->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
            ui->customPlot_bars->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

            bar_parameters->setData(ticks, bar_parameterdata);

            ui->customPlot_bars->legend->setVisible(true);
            ui->customPlot_bars->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
            ui->customPlot_bars->legend->setBrush(QColor(0, 0, 0, 100));
            ui->customPlot_bars->legend->setBorderPen(Qt::NoPen);
            QFont legendFont = font();
            legendFont.setPointSize(10);
            ui->customPlot_bars->legend->setFont(legendFont);
            ui->customPlot_bars->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
            ui->customPlot_bars->rescaleAxes(true);
            ui->customPlot_bars->replot();
        }
        else
        {
            ui->customPlot_bars->clearPlottables();
            QCPBars *bar_node = new QCPBars(ui->customPlot_bars->xAxis, ui->customPlot_bars->yAxis);
            bar_node->setAntialiased(true);
            bar_node->setStackingGap(1);
            bar_node->setName(parameter_bar);
            QColor bar_color = QColorDialog::getColor(Qt::yellow, this);
            bar_node->setPen(QPen(bar_color));
            bar_node->setBrush(bar_color);
            QVector<QString> labels;
            labels<<"Lambda"<<"Throughput"<<"Goodput"<<"Sent Data"<<"Received Data"<<"Jitter Sum"<<"Dropped Packets";
            QVector<double> ticks;
            QVector<double> bar_nodedata;
            for(int i = 0; i<alz->getValues_nodes(node_bar).size();i++)
            {
                ticks.append(i);
                bar_nodedata = (convert(alz->getValues_nodes(node_bar)));
            }
            QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
            textTicker->addTicks(ticks, labels);
            ui->customPlot_bars->xAxis->setTicker(textTicker);
            ui->customPlot_bars->xAxis->setTickLabelRotation(60);
            ui->customPlot_bars->xAxis->setSubTicks(false);
            ui->customPlot_bars->xAxis->setTickLength(0, 4);
            ui->customPlot_bars->xAxis->setRange(0, 8);
            ui->customPlot_bars->xAxis->setBasePen(QPen(Qt::black));
            ui->customPlot_bars->xAxis->setTickPen(QPen(Qt::black));
            ui->customPlot_bars->xAxis->grid()->setVisible(true);
            ui->customPlot_bars->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
            ui->customPlot_bars->xAxis->setTickLabelColor(Qt::black);
            ui->customPlot_bars->xAxis->setLabelColor(Qt::black);


            ui->customPlot_bars->yAxis->setRange(0, 12.1);
            ui->customPlot_bars->yAxis->setPadding(5); // a bit more space to the left border
            ui->customPlot_bars->yAxis->setBasePen(QPen(Qt::black));
            ui->customPlot_bars->yAxis->setTickPen(QPen(Qt::black));
            ui->customPlot_bars->yAxis->setSubTickPen(QPen(Qt::black));
            ui->customPlot_bars->yAxis->grid()->setSubGridVisible(true);
            ui->customPlot_bars->yAxis->setTickLabelColor(Qt::black);
            ui->customPlot_bars->yAxis->setLabelColor(Qt::black);
            ui->customPlot_bars->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
            ui->customPlot_bars->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

            bar_node->setData(ticks, bar_nodedata);

            ui->customPlot_bars->legend->setVisible(true);
            ui->customPlot_bars->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
            ui->customPlot_bars->legend->setBrush(QColor(0, 0, 0, 100));
            ui->customPlot_bars->legend->setBorderPen(Qt::NoPen);
            QFont legendFont = font();
            legendFont.setPointSize(10);
            ui->customPlot_bars->legend->setFont(legendFont);
            ui->customPlot_bars->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
            ui->customPlot_bars->rescaleAxes(true);
            ui->customPlot_bars->replot();

        }

    }

}

void MainWindow::on_actionExport_File_triggered()
{
    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(this,tr("Open File"),tr("/home/"),tr("CSV(;) files (*.csv);;Excel files (*.xlsx)"), &selectedFilter);

    if (!filename.isEmpty())
    {
      if(QString::compare(selectedFilter, "CSV(;) files (*.csv)", Qt::CaseInsensitive) == 0)
        alz->transform2file("csv",filename+".csv");
      else
        alz->transform2file("xlsx",filename+".xlsx");

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
        ui->customPlot->yAxis->setNumberFormat("eb"); // e = exponential, b = decimal pow
        ui->customPlot->yAxis->setNumberPrecision(0);
        ui->customPlot->replot();


        ui->customPlot_bars->yAxis->setScaleType(QCPAxis::stLogarithmic);
        ui->customPlot_bars->yAxis2->setScaleType(QCPAxis::stLogarithmic);
        ui->customPlot_bars->yAxis->setTicker(logTicker);
        ui->customPlot_bars->yAxis2->setTicker(logTicker);
        ui->customPlot_bars->yAxis->setNumberFormat("eb"); // e = exponential, b = decimal pow
        ui->customPlot_bars->yAxis->setNumberPrecision(0);
        ui->customPlot_bars->replot();
}

void MainWindow::on_actionChange_Scale_Linear_triggered()
{
    scale_checker = 0;
    ui->actionChange_Scale_Log->setChecked(false);
    ui->actionChange_Scale_Linear->setChecked(true);
    ui->customPlot->yAxis->setScaleType(QCPAxis::stLinear);
    ui->customPlot->yAxis2->setScaleType(QCPAxis::stLinear);
    QSharedPointer <QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    fixedTicker->setTickStep(ticker_spacing_y);
    ui->customPlot->yAxis->setTicker(fixedTicker);
    ui->customPlot->yAxis->setNumberFormat("f");
    ui->customPlot->xAxis->setTicks(true);
    ui->customPlot->xAxis->rescale();
    ui->customPlot->replot();

    ui->customPlot_bars->yAxis->setScaleType(QCPAxis::stLinear);
    ui->customPlot_bars->yAxis2->setScaleType(QCPAxis::stLinear);
    fixedTicker->setTickStep(ticker_spacing_y);
    ui->customPlot_bars->yAxis->setTicker(fixedTicker);
    ui->customPlot_bars->yAxis->setNumberFormat("f");
    ui->customPlot_bars->xAxis->setTicks(true);
    ui->customPlot_bars->xAxis->rescale();
    ui->customPlot_bars->replot();
}

void MainWindow::on_actionSubgrid_triggered()
{
    if(ui->actionSubgrid->isChecked())
    {
        ui->actionSubgrid->setChecked(true);
        ui->customPlot->xAxis->grid()->setSubGridVisible(true);
        ui->customPlot->yAxis->grid()->setSubGridVisible(true);

        ui->customPlot_bars->xAxis->grid()->setSubGridVisible(true);
        ui->customPlot_bars->yAxis->grid()->setSubGridVisible(true);
    }
    else
    {
        ui->actionSubgrid->setChecked(false);
        ui->customPlot->xAxis->grid()->setSubGridVisible(false);
        ui->customPlot->yAxis->grid()->setSubGridVisible(false);

        ui->customPlot_bars->xAxis->grid()->setSubGridVisible(false);
        ui->customPlot_bars->yAxis->grid()->setSubGridVisible(false);
    }
    ui->customPlot->replot();
    ui->customPlot_bars->replot();

}

void MainWindow::on_actionChange_Xticks_triggered()
{
    if(ui->tabWidget->currentIndex() == 0)
    {
        inputicksDialog *dialog = new inputicksDialog(ticker_spacing_x,ticker_spacing_y);
        if (ui->actionBlack->isChecked())
            dialog->setStyleSheet("background-color: rgb(50, 50, 50)");
        else
            dialog->setStyleSheet("background-color: rgb(200, 200, 200)");
        dialog->exec();
        if(dialog->result())
        {
            ticker_spacing_x = dialog->get_xticks();
            ticker_spacing_y = dialog->get_yticks();
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
        double d = QInputDialog::getDouble(this, tr("YTicks"), tr("YStick Step:"), ticker_spacing_y, 1, 100000000, 2, &ok, Qt::WindowFlags(), 1);
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

void MainWindow::on_actionBlack_triggered()
{
    ui->centralWidget->setStyleSheet("background-color: rgb(75, 75, 75);");
    ui->menuBar->setStyleSheet("background-color: rgb(50, 50, 50)");
    ui->toolBar->setStyleSheet("background-color: rgb(50, 50, 50)");
    ui->statusBar->setStyleSheet("background-color: rgb(50, 50, 50)");
    ui->customPlot->setBackground(QBrush(QColor(85, 85, 85)));
    ui->customPlot_bars->setBackground(QBrush(QColor(85, 85, 85)));
    this->setStyleSheet("background-color: rgb(50, 50, 50)");
    ui->actionBlack->setChecked(true);
    ui->actionWhite->setChecked(false);
}

void MainWindow::on_actionWhite_triggered()
{
    ui->centralWidget->setStyleSheet("background-color: rgb(234, 234, 234);");
    ui->menuBar->setStyleSheet("background-color: rgb(200, 200, 200)");
    ui->toolBar->setStyleSheet("background-color: rgb(200, 200, 200)");
    ui->statusBar->setStyleSheet("background-color: rgb(200, 200, 200)");
    ui->customPlot->setBackground(QBrush(QColor(245, 245, 245)));
    ui->customPlot_bars->setBackground(QBrush(QColor(245, 245, 245)));
    this->setStyleSheet("background-color: rgb(200,200,200)");
    ui->actionBlack->setChecked(true);
    ui->actionWhite->setChecked(false);
}


void MainWindow::on_actionStatistics_Results_triggered()
{
    table_dialog *dialog = new table_dialog(alz);
    if (ui->actionBlack->isChecked())
        dialog->setStyleSheet("background-color: rgb(50, 50, 50)");
    else
        dialog->setStyleSheet("background-color: rgb(200, 200, 200)");
    dialog->exec();
}

void MainWindow::on_actionClose_Rectangle_triggered()
{
    if(close_rectangle == 1)
    {
        ui->customPlot->xAxis2->setVisible(false);
        ui->customPlot->yAxis2->setVisible(false);
        ui->customPlot->xAxis2->setTickLabels(false);
        ui->customPlot->yAxis2->setTickLabels(false);
        ui->customPlot_bars->xAxis2->setVisible(false);
        ui->customPlot_bars->yAxis2->setVisible(false);
        ui->customPlot_bars->xAxis2->setTickLabels(false);
        ui->customPlot_bars->yAxis2->setTickLabels(false);
        close_rectangle = 0;
    }
    else
    {
        ui->customPlot->xAxis2->setVisible(true);
        ui->customPlot->yAxis2->setVisible(true);
        ui->customPlot->xAxis2->setTickLabels(false);
        ui->customPlot->yAxis2->setTickLabels(false);
        ui->customPlot_bars->xAxis2->setVisible(true);
        ui->customPlot_bars->yAxis2->setVisible(true);
        ui->customPlot_bars->xAxis2->setTickLabels(false);
        ui->customPlot_bars->yAxis2->setTickLabels(false);
        close_rectangle = 1;
    }
    ui->customPlot->replot();
}
