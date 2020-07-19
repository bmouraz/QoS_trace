#ifndef ANALYZER_H
#define ANALYZER_H
#include <QString>
#include<QVector>
#include<QTextStream>
#include<QFile>
#include<QDebug>

#include <bits/stdc++.h>
#include <algorithm>
#include <sstream>
#include <algorithm>

using namespace std;
class analyzer
{
public:
  analyzer(QFile *file);
  ~analyzer();
  QVector<float> getVector(int node, QString parameter);
  QVector<float> getTime (int node,  QString parameter);
  QVector<int> getNodes ();
  float getSimutime();
  int getSentDatatotal(int node);
  float getLambda(int node);
  float getThrpt(int node);
  float getGood(int node);
  float getJitterSum(int node);
  int getPckt(int node);

private:

  QVector <QVector<float>> data_filtered; //after getting all data from .tr file

  //received time filtered from .tr file
  float received=0.0;
  float queued=0.0;
  float dequeued=0.0;
  float timeSimu=0.0;
  float timeall=0.0;

  QVector <float> lambda ;
  QVector <float> throughput ;
  QVector <float> goodput ;

  QVector <int> nodelist;
  QVector <int> reclist;
  QVector <float> jitter;
  QVector <float> jitterSum;
  QVector <float> jitterrec;
  QVector <int> sentdatarec;
  QVector <int> recdatarec;
  QVector <QVector <float>> sentdata;
  QVector <int> pcktloss;

  QString qd;
  int node_intchecker;
  QVector <int> search_node(int item, QString parameter);

  void getParameters(QString line);
  void getData(QFile *file);


};

#endif // ANALYZER_H
