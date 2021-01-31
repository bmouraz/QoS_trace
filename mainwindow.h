#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QColorDialog>
#include"customlinesdialog.h"
#include "qcustomplot.h"
#include "inputicksdialog.h"
#include "analyzer.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void makePlot();
  void combobox_config();
  void on_actionOpen_File_triggered();
  void on_comboBox_2_currentIndexChanged(int index);
  void on_comboBox_currentIndexChanged(const QString &arg1);
  void on_actionSave_Graph_2_triggered();
  void titleDoubleClick(QMouseEvent* event);
  void selectionChanged();
  void mousePress();
  void mouseWheel();
  void removeSelectedGraph();
  void removeAllGraphs();
  void contextMenuRequest(QPoint pos);
  void moveLegend();
  void changeColor();
  void axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part);

  void on_actionLine_Style_Scatter_Color_triggered();

  void on_actionExport_File_triggered();

  void on_pushButton_clicked();

  void on_actionChange_Scale_Log_triggered();

  void on_actionChange_Scale_Linear_triggered();

  void on_actionSubgrid_triggered();

  void on_actionChange_Xticks_triggered();

  void axisLabelDoubleClick_bars(QCPAxis *axis, QCPAxis::SelectablePart part);

  void titleDoubleClick_bars(QMouseEvent* event);

  void selectionChanged_bars();

  void mousePress_bars();

  void mouseWheel_bars();

  void contextMenuRequest_bars(QPoint pos);

  void moveLegend_bars();

  void on_actionBlack_triggered();

  void on_actionWhite_triggered();

  void on_comboBox_3_currentIndexChanged(const QString &arg1);

  void on_comboBox_4_currentIndexChanged(int index);

private:

  QVector <int> qtd_graphs_vector;
  int qtd_graph_variable=0;
  int node,node_bar;
  int count_file_entries = 0;
  void dark_interface();
  void set_Scatter(QString scatter, int graph_index,int scatterSize);
  void ajust_table();
  enum Coluna{
      NODE, LAMBDA, THROUGHPUT,GOODPUT,SENTDATA,JITTERSUM,DROPPED
  };
  int scale_checker = 0;
  int chart_checker = 0; //if 0 - line 1 - bar
  int ticker_spacing_x = 5;
  int ticker_spacing_y = 1;
  QString parameter,parameter_bar;
  QString directory;
  analyzer *alz;
  QFile *arq;
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
