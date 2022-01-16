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

  int flag_ipv4 = 0;
  int flag_wifimac = 0;
  int flag_ethernet = 0;


  for (QString word : line.split(" ")) {
      switch(i)
      {
        case(0):
          qd = word;
          break;
        case(1):
          if (qd == "+")
            queued = word.toFloat();
          else if (qd == "-")
            dequeued = word.toFloat();
          else if (qd == "r")
            received = word.toFloat();
          else if (qd == "t")
            transmitted = word.toFloat();
          else
            dropped = word.toFloat();
          timeall = word.toFloat();
          break;
        case(2):
          node_str = word.toStdString().c_str()[0];
          if (node_str == "O")
          {
              node_str = word.toStdString().c_str()[23];
              if(word[24].isDigit())
              {
                 node_str = node_str + word.toStdString().c_str()[24];;
              }
          }
          else
          {
              node_str = word.toStdString().c_str()[10];
              if(word[11].isDigit())
              {
                 node_str = node_str + word.toStdString().c_str()[11];;
              }
          }
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
                    jitter_avg.resize(node_int+1);
                    delay.resize(node_int+1);
                    delay_sum.resize(node_int+1);
                    delay_avg.resize(node_int+1);

                    pcktloss.resize(node_int+1);

                    sentdatarec.resize(node_int+1);
                    recdatarec.resize(node_int+1);
                    count_sent.resize(node_int+1);
                    count_rec.resize(node_int+1);
                    rec_avarage.resize(node_int+1);
                    sent_avarage.resize(node_int+1);
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
              node_intchecker = node_int;
            }
            else
            {
              if ((node_int)<nodelist.size())
              {
                nodelist[node_int] = node_int;
                reclist[node_int] = 1;
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
                jitter_avg.resize(node_int+1);
                delay.resize(node_int+1);
                delay_sum.resize(node_int+1);
                delay_avg.resize(node_int+1);


                pcktloss.resize(node_int+1);
                node_intchecker = node_int;

                sentdatarec.resize(node_int+1);
                count_sent.resize(node_int+1);
                count_rec.resize(node_int+1);
                rec_avarage.resize(node_int+1);
                sent_avarage.resize(node_int+1);
                recdatarec.resize(node_int+1);
                lambda.resize(node_int+1) ;
                throughput.resize(node_int+1) ;
                goodput.resize(node_int+1) ;


              }
            }
          }
          else if (qd == "d")
          {
              if ((node_int)<nodelist.size())
              {
                  pcktloss[node_int] = pcktloss.at(node_int) + 1;
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
                jitter_avg.resize(node_int+1);
                delay.resize(node_int+1);
                delay_sum.resize(node_int+1);
                delay_avg.resize(node_int+1);


                pcktloss.resize(node_int+1);
                pcktloss[node_int] = pcktloss.at(node_int) + 1;
                node_intchecker = node_int;

                sentdatarec.resize(node_int+1);
                recdatarec.resize(node_int+1);
                count_sent.resize(node_int+1);
                count_rec.resize(node_int+1);
                rec_avarage.resize(node_int+1);
                sent_avarage.resize(node_int+1);
                lambda.resize(node_int+1) ;
                throughput.resize(node_int+1) ;
                goodput.resize(node_int+1) ;


              }
          }
          else if (qd == "t")
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
                      jitter_avg.resize(node_int+1);
                      delay.resize(node_int+1);
                      delay_sum.resize(node_int+1);
                      delay_avg.resize(node_int+1);


                      pcktloss.resize(node_int+1);
                      node_intchecker = node_int;

                      sentdatarec.resize(node_int+1);
                      count_sent.resize(node_int+1);
                      count_rec.resize(node_int+1);
                      rec_avarage.resize(node_int+1);
                      sent_avarage.resize(node_int+1);
                      recdatarec.resize(node_int+1);
                      lambda.resize(node_int+1) ;
                      throughput.resize(node_int+1) ;
                      goodput.resize(node_int+1) ;

                  }
              }
          }
        break;
        case(3):
          if (word == "ns3::Ipv4Header")
          {
            flag_ipv4 = 1;
          }
          else if (word == "ns3::EthernetHeader")
          {
            flag_ethernet = 1;
          }
          else if (word == "ns3::WifiMacHeader")
          {
            flag_wifimac = 1;
          }
        break;
        case(4):
          if (word == "ns3::WifiMacHeader")
          {
            flag_wifimac = 1;
          }
        break;
        case(22):
          if(flag_ipv4 == 1)
          {
              totallength = word.toInt() +2;
              sizelength = totallength;


              if(qd == "+")
              {
                  sentdatarec[node_int] = sentdatarec[node_int]+ sizelength;
                  count_sent[node_int] = count_sent[node_int] + 1;
                  sent_avarage[node_int] = sentdatarec[node_int]/count_sent[node_int];
              }
              else if(qd == "r")
              {
                  recdatarec[node_int] = recdatarec[node_int]+ sizelength;
                  count_rec[node_int] = count_rec[node_int] + 1;
                  rec_avarage[node_int] = recdatarec[node_int]/count_rec[node_int];
              }
          }
        break;
        case(23):
          if(flag_ipv4 == 1)
          {
              sender = word;
          }
        break;
        case(24):
          if(flag_ipv4 == 1)
          {
              receiver_jitter_helper = word;
              if(qd == '+' || qd == 't')
              {
                  QVector<QString> tmp;
                  if(qd == '+')
                    tmp.push_back(QString::number(queued));
                  else
                    tmp.push_back(QString::number(transmitted));
                  tmp.push_back(sender);
                  tmp.push_back(receiver_jitter_helper);
                  jitter_helper.push_back(tmp);
              }
              else if(qd == 'r')
              {
                  int i = 0;
                  int j = 0;
                  for(i = 0; i<jitter_helper.size(); i++)
                  {
                      if(j == 0)
                      {
                          if(sender == jitter_helper[i][1] && receiver_jitter_helper == jitter_helper[i][2])
                          {
                              delay[node_int] = received - jitter_helper[i][0].toFloat();
                              delay_sum[node_int] += received - jitter_helper[i][0].toFloat();
                              delay_avg[node_int] = delay_sum[node_int]/reclist[node_int];
                              jitter[node_int] = delay[node_int]-jitter[node_int];
                              jitterSum[node_int] += jitter.at(node_int);
                              jitter_avg[node_int] = jitterSum[node_int]/reclist[node_int];
                              jitterrec[node_int] = jitter.at(node_int);
                              jitter[node_int] = delay[node_int];
                              jitter_helper.remove(i);
                              j = 1;
                          }
                      }
                  }
              }
          }
        break;
        case(27):
          if (flag_ipv4 == 0 && flag_wifimac == 0)
          {
              totallength = word.toInt() +2;
              sizelength = totallength;


              if(qd == "+")
              {
                  sentdatarec[node_int] = sentdatarec[node_int]+ sizelength;
                  count_sent[node_int] = count_sent[node_int] + 1;
                  sent_avarage[node_int] = sentdatarec[node_int]/count_sent[node_int];
              }
              else if(qd == "r")
              {
                  recdatarec[node_int] = recdatarec[node_int]+ sizelength;
                  count_rec[node_int] = count_rec[node_int] + 1;
                  rec_avarage[node_int] = recdatarec[node_int]/count_rec[node_int];
              }

          }
          break;
        case(28):
          if ( flag_ipv4 == 0 && flag_wifimac == 0)
            sender = word;
        break;
        case(29):
          if ( flag_ipv4 == 0 && flag_wifimac == 0)
          {
              receiver_jitter_helper = word;
              if(qd == '+')
              {
                  QVector<QString> tmp;
                  tmp.push_back(QString::number(queued));
                  tmp.push_back(sender);
                  tmp.push_back(receiver_jitter_helper);
                  jitter_helper.push_back(tmp);
              }
              else if(qd == 'r')
              {
                  int i = 0;
                  int j = 0;
                  for(i = 0; i<jitter_helper.size(); i++)
                  {
                      if(j == 0)
                      {
                          if(sender == jitter_helper[i][1] && receiver_jitter_helper == jitter_helper[i][2])
                          {
                              delay[node_int] = received - jitter_helper[i][0].toFloat();
                              delay_sum[node_int] += received - jitter_helper[i][0].toFloat();
                              delay_avg[node_int] = delay_sum[node_int]/reclist[node_int];
                              jitter[node_int] = delay[node_int]-jitter[node_int];
                              jitterSum[node_int] += jitter.at(node_int);
                              jitter_avg[node_int] = jitterSum[node_int]/reclist[node_int];
                              jitterrec[node_int] = jitter.at(node_int);
                              jitter[node_int] = delay[node_int];
                              jitter_helper.remove(i);
                              j = 1;
                          }
                      }
                  }
              }
          }
        break;
        case(31):
           if (word == "ns3::TcpHeader")
           {
              tcp_checker = 1;
           }
           else
           {
               tcp_checker = 0;
           }
          break;
        case(38):
          if (flag_ethernet == 0 && flag_ipv4 == 0 && flag_wifimac == 0)
          {
          if(tcp_checker == 1)
          {
              if(qd == "+")
                {

                  //lambda[node_int] = float((((sentdatarec[node_int]))/((sent_avarage[node_int])))/(timeSimu));
                  lambda[node_int] = float(count_sent[node_int]/timeSimu);
                  //throughput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]));
                  throughput[node_int] = float(sentdatarec[node_int]/timeSimu);

                  goodput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]-rec_avarage[node_int]));

                  if(goodput[node_int]  < 0)
                      goodput[node_int] = 0;


                }

          }

          else if(word != "Default" )
            {

              const QRegExp rx(QLatin1Literal("[^0-9]+"));
              const auto&& parts = word.split(rx, QString::SkipEmptyParts);

              if(qd == "+")
              {
                //lambda[node_int] = float((((sentdatarec[node_int]))/((sizelength)))/(timeSimu));
                  lambda[node_int] = float(count_sent[node_int]/timeSimu);
                //throughput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]));
                throughput[node_int] = float(sentdatarec[node_int]/timeSimu);
                goodput[node_int] = float(lambda[node_int]*(parts[0].toInt()));

              }

            }
          else
          {
              if(qd == "+")
                {
                  //lambda[node_int] = float((((sentdatarec[node_int]))/((sent_avarage[node_int])))/(timeSimu));
                  lambda[node_int] = float(count_sent[node_int]/timeSimu);
                  //throughput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]));
                  throughput[node_int] = float(sentdatarec[node_int]/timeSimu);

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
             tmp.push_back(jitter_avg[(node_int)]);
             tmp.push_back(pcktloss[(node_int)]);
             tmp.push_back(goodput[node_int]);
             tmp.push_back(recdatarec[node_int]);
             tmp.push_back(delay[(node_int)]);
             tmp.push_back(delay_avg[(node_int)]);
             data_filtered.push_back(tmp);

             tmp.clear();

             tmp.push_back(node_int);
             tmp.push_back(timeall);
             tmp.push_back(sentdatarec[node_int]);
             tmp.push_back(recdatarec[node_int]);
             tmp.push_back(jitter_avg[(node_int)]);
             tmp.push_back(pcktloss[(node_int)]);
             tmp.push_back(lambda[node_int]);
             tmp.push_back(throughput[node_int]);
             tmp.push_back(goodput[node_int]);
             tmp.push_back(delay_avg[(node_int)]);
             alldata.push_back(tmp);

           }

          break;
        case(41):
          if(flag_ethernet == 1)
          {
              if(qd == "+")
              {
                  sentdatarec[node_int] = sentdatarec[node_int]+ sizelength;
                  sent_avarage[node_int] = sentdatarec[node_int]/count_sent[node_int];
              }
              else if(qd == "r")
              {
                  recdatarec[node_int] = recdatarec[node_int]+ sizelength;
                  rec_avarage[node_int] = recdatarec[node_int]/count_rec[node_int];
              }
              if(qd == "+")
                {

                  //lambda[node_int] = float((((sentdatarec[node_int]))/((sent_avarage[node_int])))/(timeSimu));
                  lambda[node_int] = float(count_sent[node_int]/timeSimu);
                  //throughput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]));
                  throughput[node_int] = float(sentdatarec[node_int]/timeSimu);

                  goodput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]-rec_avarage[node_int]));

                  if(goodput[node_int]  < 0)
                      goodput[node_int] = 0;

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
                  tmp.push_back(jitter_avg[(node_int)]);
                  tmp.push_back(pcktloss[(node_int)]);
                  tmp.push_back(goodput[node_int]);
                  tmp.push_back(recdatarec[node_int]);
                  tmp.push_back(delay[(node_int)]);
                  tmp.push_back(delay_avg[(node_int)]);
                  data_filtered.push_back(tmp);

                  tmp.clear();

                  tmp.push_back(node_int);
                  tmp.push_back(timeall);
                  tmp.push_back(sentdatarec[node_int]);
                  tmp.push_back(recdatarec[node_int]);
                  tmp.push_back(jitter_avg[(node_int)]);
                  tmp.push_back(pcktloss[(node_int)]);
                  tmp.push_back(lambda[node_int]);
                  tmp.push_back(throughput[node_int]);
                  tmp.push_back(goodput[node_int]);
                  tmp.push_back(delay_avg[(node_int)]);
                  alldata.push_back(tmp);


                }
          }

          break;
        case(47):
              if(flag_ipv4 == 1)
              {
                  if(!(word == "length:"))
                  {
                      totallength = word.toInt() +2;
                      sizelength = totallength;


                      if(qd == "+")
                      {
                          sentdatarec[node_int] = sentdatarec[node_int]+ sizelength;
                          sent_avarage[node_int] = sentdatarec[node_int]/count_sent[node_int];
                      }
                      else if(qd == "r")
                      {
                          recdatarec[node_int] = recdatarec[node_int]+ sizelength;
                          rec_avarage[node_int] = recdatarec[node_int]/count_rec[node_int];
                      }
                  }
                  if(qd == "+")
                    {

                      //lambda[node_int] = float((((sentdatarec[node_int]))/((sent_avarage[node_int])))/(timeSimu));
                      lambda[node_int] = float(count_sent[node_int]/timeSimu);
                      //throughput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]));
                      throughput[node_int] = float(sentdatarec[node_int]/timeSimu);

                      goodput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]-rec_avarage[node_int]));

                      if(goodput[node_int]  < 0)
                          goodput[node_int] = 0;

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
                      tmp.push_back(jitter_avg[(node_int)]);
                      tmp.push_back(pcktloss[(node_int)]);
                      tmp.push_back(goodput[node_int]);
                      tmp.push_back(recdatarec[node_int]);
                      tmp.push_back(delay[(node_int)]);
                      tmp.push_back(delay_avg[(node_int)]);
                      data_filtered.push_back(tmp);

                      tmp.clear();

                      tmp.push_back(node_int);
                      tmp.push_back(timeall);
                      tmp.push_back(sentdatarec[node_int]);
                      tmp.push_back(recdatarec[node_int]);
                      tmp.push_back(jitter_avg[(node_int)]);
                      tmp.push_back(pcktloss[(node_int)]);
                      tmp.push_back(lambda[node_int]);
                      tmp.push_back(throughput[node_int]);
                      tmp.push_back(goodput[node_int]);
                      tmp.push_back(delay_avg[(node_int)]);
                      alldata.push_back(tmp);


                    }
              }
        break;
        case(48):
            if(flag_ipv4 == 1)
            {
                if(!(word == "Hop"))
                {
                    totallength = word.toInt() +2;
                    sizelength = totallength;


                    if(qd == "+")
                    {
                        sentdatarec[node_int] = sentdatarec[node_int]+ sizelength;
                        sent_avarage[node_int] = sentdatarec[node_int]/count_sent[node_int];
                    }
                    else if(qd == "r")
                    {
                        recdatarec[node_int] = recdatarec[node_int]+ sizelength;
                        rec_avarage[node_int] = recdatarec[node_int]/count_rec[node_int];
                    }
                }
                if(qd == "+")
                  {

                    //lambda[node_int] = float((((sentdatarec[node_int]))/((sent_avarage[node_int])))/(timeSimu));
                    lambda[node_int] = float(count_sent[node_int]/timeSimu);
                    //throughput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]));
                    throughput[node_int] = float(sentdatarec[node_int]/timeSimu);

                    goodput[node_int] = float(lambda[node_int]*(sent_avarage[node_int]-rec_avarage[node_int]));

                    if(goodput[node_int]  < 0)
                        goodput[node_int] = 0;

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
                    tmp.push_back(jitter_avg[(node_int)]);
                    tmp.push_back(pcktloss[(node_int)]);
                    tmp.push_back(goodput[node_int]);
                    tmp.push_back(recdatarec[node_int]);
                    tmp.push_back(delay[(node_int)]);
                    tmp.push_back(delay_avg[(node_int)]);
                    data_filtered.push_back(tmp);

                    tmp.clear();

                    tmp.push_back(node_int);
                    tmp.push_back(timeall);
                    tmp.push_back(sentdatarec[node_int]);
                    tmp.push_back(recdatarec[node_int]);
                    tmp.push_back(jitter_avg[(node_int)]);
                    tmp.push_back(pcktloss[(node_int)]);
                    tmp.push_back(lambda[node_int]);
                    tmp.push_back(throughput[node_int]);
                    tmp.push_back(goodput[node_int]);
                    tmp.push_back(delay_avg[(node_int)]);
                    alldata.push_back(tmp);


                  }
            }
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
        timeSimu = line.split(" ").at(1).toFloat();
     }

     in.seek(0);
      while (!in.atEnd())
      {
         QString line = in.readLine();
         getParameters(line);
      }

}

QVector<float> analyzer::getVector(int node, QString parameter)
{
  QVector<float> parameters;
  if(node == nodelist.size())
  {
      float time_aux = alldata[0][1];
      float parameter_aux=0.0;
      int counter_aux = 0;
      if (QString::compare(parameter, "Lambda", Qt::CaseInsensitive) == 0)
        {
          for(int i = 0; i<alldata.size();i++)
            {

                if(time_aux == alldata[i][1])
                {
                    counter_aux++;
                    parameter_aux = parameter_aux + alldata[i][6];
                }

                else
                {

                    parameters.push_back(parameter_aux/counter_aux);
                    time_aux = alldata[i][1];
                    counter_aux = 0;
                }
            }
        }
      if (QString::compare(parameter, "Throughput", Qt::CaseInsensitive) == 0)
        {
              for(int i = 0; i<alldata.size();i++)
                {

                    if(time_aux == alldata[i][1])
                    {
                        counter_aux++;
                        parameter_aux = parameter_aux + alldata[i][7];
                    }

                    else
                    {

                        parameters.push_back(parameter_aux/counter_aux);
                        time_aux = alldata[i][1];
                        counter_aux = 0;
                    }
                }
        }
      if (QString::compare(parameter, "Goodput", Qt::CaseInsensitive) == 0)
        {

              for(int i = 0; i<alldata.size();i++)
                {

                    if(time_aux == alldata[i][1])
                    {
                        counter_aux++;
                        parameter_aux = parameter_aux + alldata[i][8];
                    }

                    else
                    {

                        parameters.push_back(parameter_aux/counter_aux);
                        time_aux = alldata[i][1];
                        counter_aux = 0;
                    }
                }
        }
      if (QString::compare(parameter, "Sent Data", Qt::CaseInsensitive) == 0)
        {
              for(int i = 0; i<alldata.size();i++)
                {

                    if(time_aux == alldata[i][1])
                    {
                        counter_aux++;
                        parameter_aux = parameter_aux + alldata[i][2];
                    }

                    else
                    {

                        parameters.push_back(parameter_aux/counter_aux);
                        time_aux = alldata[i][1];
                        counter_aux = 0;
                    }
                }
        }
      if (QString::compare(parameter, "Jitter", Qt::CaseInsensitive) == 0)
        {
              for(int i = 0; i<alldata.size();i++)
                {

                    if(time_aux == alldata[i][1])
                    {
                        counter_aux++;
                        parameter_aux = parameter_aux + data_filtered[i][2];
                    }

                    else
                    {

                        parameters.push_back(parameter_aux/counter_aux);
                        time_aux = alldata[i][1];
                        counter_aux = 0;
                    }
                }
        }
      if (QString::compare(parameter, "Jitter Avg", Qt::CaseInsensitive) == 0)
        {
          for(int i = 0; i<alldata.size();i++)
            {

                if(time_aux == alldata[i][1])
                {
                    counter_aux++;
                    parameter_aux = parameter_aux + alldata[i][4];
                }

                else
                {

                    parameters.push_back(parameter_aux/counter_aux);
                    time_aux = alldata[i][1];
                    counter_aux = 0;
                }
            }
        }
      if (QString::compare(parameter, "Dropped Packets", Qt::CaseInsensitive) == 0)
        {
          for(int i = 0; i<alldata.size();i++)
            {

                if(time_aux == alldata[i][1])
                {
                    counter_aux++;
                    parameter_aux = parameter_aux + alldata[i][5];
                }

                else
                {

                    parameters.push_back(parameter_aux/counter_aux);
                    time_aux = alldata[i][1];
                    counter_aux = 0;
                }
            }
        }
      if (QString::compare(parameter, "Received Data", Qt::CaseInsensitive) == 0)
        {
          for(int i = 0; i<alldata.size();i++)
            {

                if(time_aux == alldata[i][1])
                {
                    counter_aux++;
                    parameter_aux = parameter_aux + alldata[i][3];
                }

                else
                {

                    parameters.push_back(parameter_aux/counter_aux);
                    time_aux = alldata[i][1];
                    counter_aux = 0;
                }
            }
        }
      if (QString::compare(parameter, "Received Data", Qt::CaseInsensitive) == 0)
        {
          for(int i = 0; i<alldata.size();i++)
            {

                if(time_aux == alldata[i][1])
                {
                    counter_aux++;
                    parameter_aux = parameter_aux + alldata[i][3];
                }

                else
                {

                    parameters.push_back(parameter_aux/counter_aux);
                    time_aux = alldata[i][1];
                    counter_aux = 0;
                }
            }
        }
      if (QString::compare(parameter, "Delay", Qt::CaseInsensitive) == 0)
        {
          for(int i = 0; i<alldata.size();i++)
            {

                if(time_aux == alldata[i][1])
                {
                    counter_aux++;
                    parameter_aux = parameter_aux + data_filtered[i][7];
                }

                else
                {

                    parameters.push_back(parameter_aux/counter_aux);
                    time_aux = alldata[i][1];
                    counter_aux = 0;
                }
            }
        }
      if (QString::compare(parameter, "Delay Avg", Qt::CaseInsensitive) == 0)
        {
          for(int i = 0; i<alldata.size();i++)
            {

                if(time_aux == alldata[i][1])
                {
                    counter_aux++;
                    parameter_aux = parameter_aux + alldata[i][9];
                }

                else
                {

                    parameters.push_back(parameter_aux/counter_aux);
                    time_aux = alldata[i][1];
                    counter_aux = 0;
                }
            }
        }
  }

  else
  {
  QVector<int> nodes =  search_node(node,parameter);


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
  if (QString::compare(parameter, "Jitter Avg", Qt::CaseInsensitive) == 0)
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
  if (QString::compare(parameter, "Delay", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        parameters.push_back(data_filtered[i][7]);
    }
  if (QString::compare(parameter, "Delay Avg", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodes.size();i++)
        parameters.push_back(data_filtered[i][8]);
    }
  }
  return parameters;
}

QVector<float> analyzer::getTime (int node, QString parameter)
{

  QVector<float> time;
  if(node == nodelist.size())
  {
    float time_aux = alldata[0][1];
    for(int i = 0; i<alldata.size(); i++)
    {
        if(time_aux == alldata[i][1])
        {}
        else
        {

            time.push_back(alldata[i-1][1]);
            time_aux = alldata[i][1];
        }

    }
  }
  else{
   QVector<int> nodes =  search_node(node, parameter);

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

float analyzer::getJitter_Avg(int node)
{
  return jitter_avg[node];
}

int analyzer::getPckt(int node)
{
  return pcktloss[node];
}

float analyzer::getDelay_Avg(int node)
{
  return delay_avg[node];
}

void analyzer::transform2file(QString type, QString filename)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadWrite))
    {

        if(type == "csv")
        {
            QTextStream header(&file);
            QTextStream datafile(&file);
            header.flush();
            datafile.flush();
            header<<"Node;Time;Sent;Received;JitterAvg;DelayAvg;PcktLoss;Lambda;Throughput;Goodput"<<endl;
            for(int i = 0; i<alldata.size();i++)
            {
               datafile<<alldata[i][0]<<";"<<alldata[i][1]<<";"<<alldata[i][2]<<";"<<alldata[i][3]<<";"<<alldata[i][4]<<";"<<alldata[i][9]<<";"<<alldata[i][5]<<";"<<alldata[i][6]<<";"<<alldata[i][7]<<";"<<alldata[i][8]<<";"<<endl;
            }
            file.close();
        }
        else
        {
            Document xlsx;
            xlsx.write(1, 1, "Node");       //A
            xlsx.write(1, 2, "Time");       //B
            xlsx.write(1, 3, "Sent");       //C
            xlsx.write(1, 4, "Received");   //D
            xlsx.write(1, 5, "JitterAvg");  //E
            xlsx.write(1, 6, "DelayAvg");    //F
            xlsx.write(1, 7, "PcktLoss");   //G
            xlsx.write(1, 8, "Lambda");     //H
            xlsx.write(1, 9, "Throughput"); //I
            xlsx.write(1, 10, "Goodput");   //J
            for (int i=2; i<alldata.size()+2; i++)
            {
                xlsx.write(i, 1, alldata[i-2][0]);       //A
                xlsx.write(i, 2, alldata[i-2][1]);       //B
                xlsx.write(i, 3, alldata[i-2][2]);       //C
                xlsx.write(i, 4, alldata[i-2][3]);       //D
                xlsx.write(i, 5, alldata[i-2][4]);       //E
                xlsx.write(i, 6, alldata[i-2][9]);       //F
                xlsx.write(i, 7, alldata[i-2][5]);       //G
                xlsx.write(i, 8, alldata[i-2][6]);       //H
                xlsx.write(i, 9, alldata[i-2][7]);       //I
                xlsx.write(i, 10, alldata[i-2][8]);       //J
            }
            xlsx.saveAs(filename);
        }
    }

}

QVector<float> analyzer::getValues(QString parameter)
{


  QVector<float> parameters;
  if (QString::compare(parameter, "Lambda", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodelist.size();i++)
        {
            parameters.push_back(lambda[i]);
        }
    }
  if (QString::compare(parameter, "Throughput", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodelist.size();i++)
        {
            parameters.push_back(throughput[i]);
        }
    }
  if (QString::compare(parameter, "Goodput", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodelist.size();i++)
        {
            parameters.push_back(goodput[i]);
        }
    }
  if (QString::compare(parameter, "Sent Data", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodelist.size();i++)
        {
            parameters.push_back(sentdatarec[i]);
        }
    }
  if (QString::compare(parameter, "Jitter Avg", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodelist.size();i++)
        parameters.push_back(jitter_avg[i]);
    }
  if (QString::compare(parameter, "Dropped Packets", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodelist.size();i++)
        parameters.push_back(pcktloss[i]);
    }
  if (QString::compare(parameter, "Received Data", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodelist.size();i++)
        parameters.push_back(recdatarec[i]);
    }
  if (QString::compare(parameter, "Jitter Avg", Qt::CaseInsensitive) == 0)
    {
      for(int i = 0; i<nodelist.size();i++)
        parameters.push_back(delay_avg[i]);
    }
  return parameters;
}

QVector<float> analyzer::getValues_nodes(int node)
{
  QVector<float> parameters;
  parameters.append(getLambda(node));
  parameters.append(getThrpt(node));
  parameters.append(getGood(node));
  parameters.append(getSentDatatotal(node));
  parameters.append(recdatarec.at(node));
  parameters.append(getJitter_Avg(node));
  parameters.append(getDelay_Avg(node));
  parameters.append(getPckt(node));
  return parameters;
}
