#include "Analysis/analysis.h"
#include "qbytearray.h"
#include <QFile>
#include <QDebug>

TAnalysis::TAnalysis()
{
    m_Channels = 3;
    m_ChannelsLeg = 256;

}

TAnalysis::~TAnalysis()
{


}

void TAnalysis::AnalysisRecvData()
{
    QFile file("test.txt");
    if(!file.open(QFile::ReadOnly | QIODevice::Text))
    {
        qDebug("can not open file!\n");
        return;
    }
    QByteArray DataArray;
    while (!file.atEnd())
    {
        DataArray = file.readAll();//读出所有字符
    }
    file.close();
    QString str(DataArray);
    qDebug()<<str;
    int temp[65535];
    int j=0;
    for(int i=0;i<str.length();i+=3)
    {
        QString st = str.mid(i,2);//从i开始截取2个字符
        qDebug() << st;
        temp[j++] = HexToInt(st,2);
        qDebug("%d\n",temp[j-1]);
    }

    m_Channels = temp[4];

    return;
}


bool TAnalysis::uStrcmp(unsigned char *str1, char *str2)
{
    while((*str1 != '\0')&&(*str2 != '\0'))
    {
        qDebug("str1=%x,str2=%x\n",*str1,(*str2&0xff));
        if(*str1 != (*str2&0xff))return false;
        str1++;
        str2++;
    }

    return true;
}

int TAnalysis::HexToInt(QString s, char len)
{
       int i;
       int n = 0;

       if(s.length()<len) return 0;

       for (i=0;i<len;++i)
       {
           if (s.at(i) > '9')
           {
               n = 16 * n + (10 + s.at(i).toLatin1() - 'A');
           }
           else
           {
               n = 16 * n +( s.at(i).toLatin1() - '0');
           }
       }

       return n;

}
