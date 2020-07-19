#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QColorDialog>
#include"customlinesdialog.h"
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
  void on_actionChage_BG_triggered();
  void on_actionLine_Style_Scatter_Color_triggered();

private:
  int BG = 0;
  QVector <int> qtd_graphs_vector;
  int qtd_graph_variable=0;
  int node;
  void dark_interface();
  void set_Scatter(QString scatter, int graph_index,int scatterSize);
  void ajust_table();
  enum Coluna{
      NODE, LAMBDA, THROUGHPUT,GOODPUT,SENTDATA,JITTERSUM,DROPPED
  };
  QString parameter;
  analyzer *alz;
  QFile *arq;
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
