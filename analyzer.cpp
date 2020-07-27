#include "analyzer.h"


analyzer::analyzer(QFile *file)
{

  getData(file);
}

analyzer::~analyzer()
{
//  delete data_filtered;
}



void analyzer::getParameters(QString line)
{
  int i = 0;
  QString node_str;
  int node_int = 0;
  int totallength = 0;
  int sizelength = 0;
  int pcklosschecker = 0;

  for (QString word : line.split(" ")) {
      switch(i)
      {
        case(0):
          pcklosschecker = 0;
          if (pcktloss.size()>1 && word == qd && word == "r")
            pcklosschecker = 1;
          else
            pcklosschecker = 0;
          qd = word;
          break;
        case(1):
          if (qd == "+")
            queued = word.toFloat();
          else if (qd == "-")
            dequeued = word.toFloat();
          else
            received = word.toFloat();
          timeall = word.toFloat();
          break;
        case(2):
          node_str = word.toStdString().c_str()[10];
          node_int = node_str.toInt();
          if (qd == "+")
          {
              if(!count(nodelist.begin(),nodelist.end(),node_int))
              {
                  if ((node_int)<nodelist.size())
                    nodelist[node_int] = node_int;

                  else
                  {

                    nodelist.resize((node_int+1));
                    nodelist[node_int] = node_int;

                    reclist.resize(node_int+1);

                    jitter.resize(node_int+1);
                    jitterSum.resize(node_int+1);
                    jitterrec.resize(node_int+1);

                    pcktloss.resize(node_int+1);

                    sentdatarec.resize(node_int+1);
                    recdatarec.resize(node_int+1);
                    lambda.resize(node_int+1) ;
                    throughput.resize(node_int+1) ;
                    goodput.resize(node_int+1) ;

                  }
              }
          }

          if (qd == "r")
          {
            if(count(nodelist.begin(),nodelist.end(),node_int))
            {
              reclist[node_int] += 1;
              jitter[node_int] = received-jitter.at(node_int);
              jitterSum[node_int] += jitter.at(node_int);
              jitterrec[node_int] = jitter.at(node_int);
              jitter[node_int] = received;
              if (pcklosschecker == 1 && node_intchecker == node_int)
                pcktloss[node_int] = pcktloss.at(node_int) + 1;
              node_intchecker = node_int;
            }
            else
            {
              if ((node_int)<nodelist.size())
              {
                nodelist[node_int] = node_int;
                reclist[node_int] = 1;
                jitter[node_int] = received;
                if (pcklosschecker == 1 && node_intchecker == node_int)
                  pcktloss[node_int] = pcktloss.at(node_int) + 1;
                node_intchecker = node_int;
              }
              else
              {

                nodelist.resize((node_int+1));
                nodelist[node_int] = node_int;

                reclist.resize(node_int+1);
                reclist[node_int] = 1;

                jitter.resize(node_int+1);
                jitterSum.resize(node_int+1);
                jitterrec.resize(node_int+1);
                jitter[node_int] = received;

                pcktloss.resize(node_int+1);
                node_intchecker = node_int;

                sentdatarec.resize(node_int+1);
                recdatarec.resize(node_int+1);
                lambda.resize(node_int+1) ;
                throughput.resize(node_int+1) ;
                goodput.resize(node_int+1) ;


              }
            }
          }
          break;
        case(27):
          totallength = word.toInt() +2;
          sizelength = totallength;
          if(qd == "+")
              sentdatarec[node_int] = sentdatarec[node_int]+ sizelength;
          else if(qd == "r")
              recdatarec[node_int] = recdatarec[node_int]+ sizelength;
          break;

        case(38):
          if(word != "Default")
            {
              const QRegExp rx(QLatin1Literal("[^0-9]+"));
              const auto&& parts = word.split(rx, QString::SkipEmptyParts);
              if(qd == "+")
                {
                  lambda[node_int] = float((((sentdatarec[node_int]))/((sizelength)))/(timeSimu));
                  throughput[node_int] = float(lambda[node_int]*(totallength+2));
                  goodput[node_int] = float(lambda[node_int]*(parts[0].toInt()));
                }
              if(qd=="r")
                {
                  lambda[node_int] = float((((sentdatarec[node_int]))/((sizelength)))/(timeSimu));
                  throughput[node_int] = float(lambda[node_int]*(sizelength));
                  goodput[node_int] = float(lambda[node_int]*(parts[0].toInt()));
                }
            }
          else
          {
              if(qd == "+")
                {
                  lambda[node_int] = float((((sentdatarec[node_int]))/((sizelength)))/(timeSimu));
                  throughput[node_int] = float(lambda[node_int]*(totallength+2));
                }
              if(qd=="r")
                {
                  lambda[node_int] = float((((sentdatarec[node_int]))/((sizelength)))/(timeSimu));
                  throughput[node_int] = float(lambda[node_int]*(sizelength));
                }
          }


             QVector<float> tmp;

             tmp.push_back(node_int);
             tmp.push_back(timeall);
             tmp.push_back(sentdatarec.at(node_int));
             tmp.push_back(lambda[node_int]);
             tmp.push_back(throughput[node_int]);
             sentdata.push_back(tmp);


             tmp.clear();

             tmp.push_back(node_int);
             tmp.push_back(timeall);
             tmp.push_back(jitterrec[(node_int)]);
             tmp.push_back(jitterSum[(node_int)]);
             tmp.push_back(pcktloss[(node_int)]);
             tmp.push_back(goodput[node_int]);
             tmp.push_back(recdatarec[node_int]);
             data_filtered.push_back(tmp);


          break;
    }
    i++;
  }
}


QVector<int> analyzer::search_node(int item, QString parameter)
{
   QVector<int> rec;
   if (QString::compare(parameter, "Lambda", Qt::CaseInsensitive) == 0 || QString::compare(parameter, "Throughput", Qt::CaseInsensitive) == 0
                                                                       || QString::compare(parameter, "Sent Data", Qt::CaseInsensitive) == 0)
     {
       for (int i = 0; i < sentdata.size(); i++)
       {
        if (sentdata[i][0] == item)
          rec.push_back(i);
       }
     }
   else
    {
       for (int i = 0; i < data_filtered.size(); i++)
       {
        if (data_filtered[i][0] == item)
          rec.push_back(i);
       }
    }

   return rec;
 }



void analyzer::getData(QFile *file)

{
    QTextStream in(file);
     while (!in.atEnd())
     {
        QString line = in.readLine();
        getParameters(line);
        timeSimu = line.split(" ").at(1).toFloat();
     }

}

QVector<float> analyzer::getVector(int node, QString parameter)
{

  QVector<int> nodes =  search_node(node,parameter);

  QVector<float> parameters;
  if (QString::compare(parameter, "Lambda", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        {
            parameters.push_back(sentdata[i][3]);
        }
    }
  if (QString::compare(parameter, "Throughput", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        {
            parameters.push_back(sentdata[i][4]);
        }
    }
  if (QString::compare(parameter, "Goodput", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        {
            parameters.push_back(data_filtered[i][5]);
        }
    }
  if (QString::compare(parameter, "Sent Data", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        {
            parameters.push_back(sentdata[i][2]);
        }
    }
  if (QString::compare(parameter, "Jitter", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        parameters.push_back(data_filtered[i][2]);
    }
  if (QString::compare(parameter, "Jitter Sum", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        parameters.push_back(data_filtered[i][3]);
    }
  if (QString::compare(parameter, "Dropped Packets", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        parameters.push_back(data_filtered[i][4]);
    }
  if (QString::compare(parameter, "Received Data", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        parameters.push_back(data_filtered[i][6]);
    }
  return parameters;
}

QVector<float> analyzer::getTime (int node, QString parameter)
{
  QVector<int> nodes =  search_node(node, parameter);
  QVector<float> time;
  if (QString::compare(parameter, "Lambda", Qt::CaseInsensitive) == 0 || QString::compare(parameter, "Throughput", Qt::CaseInsensitive) == 0
                                                                      || QString::compare(parameter, "Sent Data", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
          time.push_back(sentdata[i][1]);
    }
  else
  {
    for(int i = 0; i<nodes.size();i++)
        time.push_back(data_filtered[i][1]);
  }

  return time;
}

float analyzer::getSimutime()
{
  return timeSimu;
}

QVector<int> analyzer::getNodes()
{
  return nodelist;
}

int analyzer::getSentDatatotal(int node)
{
  return sentdatarec[node];
}

float analyzer::getLambda(int node)
{
  return lambda[node];
}

float analyzer::getThrpt(int node)
{
  return throughput[node];
}

float analyzer::getGood(int node)
{
  return goodput[node];
}

float analyzer::getJitterSum(int node)
{
  return jitterSum[node];
}

int analyzer::getPckt(int node)
{
  return pcktloss[node];
}
