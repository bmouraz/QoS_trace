#ifndef CUSTOMLINESDIALOG_H
#define CUSTOMLINESDIALOG_H

#include <QDialog>
#include<QColorDialog>
#include "qcustomplot.h"

namespace Ui {
  class CustomLinesdialog;
}

class CustomLinesdialog : public QDialog
{
  Q_OBJECT

public:
  explicit CustomLinesdialog(QWidget *parent = nullptr);
  ~CustomLinesdialog();
  QColor get_Color();
  QString get_LineStyle();
  QString get_ScatterStyle();
  int get_ScatterSize();
  void ajust_Graphcombo(QVector<int> graphs);
  int getgraphindex();
  int get_linewidth();

private slots:
  void on_color_Button_clicked();

  void on_radioButton_clicked(bool checked);

  void on_Line_Combo_currentTextChanged(const QString &arg1);

  void on_scatter_Combo_currentTextChanged(const QString &arg1);

  void on_scatter_Slider_sliderMoved(int position);

  void on_ok_cancel_apply_buttons_accepted();

  void on_ok_cancel_apply_buttons_rejected();

  void on_graph_Combo_currentIndexChanged(int index);

  void on_linewidthSlider_sliderMoved(int position);

private:
  Ui::CustomLinesdialog *ui;
  QColor color;
  QString linestyleret;
  QString scatterstyleret;
  int scattersizeret = 0;
  int graphindex = 0;
  int linewidthret = 0;
  void ajust_LineStylecombo();
  void ajust_ScatterStylecombo();
  void ajust_Scatterslider();
  void ajust_linewidthslider();
};

#endif // CUSTOMLINESDIALOG_H
