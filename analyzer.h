#ifndef ANALYZER_H
#define ANALYZER_H
#include <QString>
#include<QVector>
#include<QTextStream>
#include<QFile>
#include<QDebug>
#include<QPdfWriter>
#include<QTextDocument>
#include<QPainter>

#include <bits/stdc++.h>
#include <algorithm>
#include <sstream>
#include <algorithm>

#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"
using namespace QXlsx;

using namespace std;
class analyzer
{
public:
  analyzer(QFile *file);
  ~analyzer();
  QVector<float> getVector(int node, QString parameter);
  QVector<float> getTime (int node,  QString parameter);
  QVector<int> getNodes ();
  QVector<float> getValues(QString parameter);
  QVector<float> getValues_nodes(int node);
  float getSimutime();
  int getSentDatatotal(int node);
  float getLambda(int node);
  float getThrpt(int node);
  float getGood(int node);
  float getJitterSum(int node);
  int getPckt(int node);
  void transform2file(QString type, QString filename);

private:

  QVector <QVector<float>> data_filtered; //after getting all data from .tr file

  //received time filtered from .tr file
  float received=0.0;
  float queued=0.0;
  float dropped = 0.0;
  float dequeued=0.0;
  float timeSimu=0.0;
  float timeall=0.0;
  int tst = 0;


  QVector <float> lambda ;
  QVector <float> throughput ;
  QVector <float> goodput ;

  QVector <int> nodelist;
  QVector <int> reclist;
  QVector <int> sent_avarage;
  QVector <int> rec_avarage;
  QVector <float> jitter;
  QVector <float> jitterSum;
  QVector <float> jitterrec;
  QVector <int> sentdatarec;
  QVector <int> count_sent;
  QVector <int> count_rec;
  QVector <int> recdatarec;
  QVector <QVector <float>> sentdata;
  QVector <int> pcktloss;
  QVector <QVector <float>> alldata;
  QString qd;
  int tcp_checker = 0;
  int node_intchecker;
  QVector <int> search_node(int item, QString parameter);

  void getParameters(QString line);
  void getData(QFile *file);


};

#endif // ANALYZER_H
