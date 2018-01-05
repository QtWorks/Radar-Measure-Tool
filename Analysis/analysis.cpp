#include "Analysis/analysis.h"
#include "qbytearray.h"
#include <QFile>
#include <QDebug>

TAnalysis::TAnalysis()
{
    m_Channels = 4;
    m_ChannelsLeg = 256;

}

TAnalysis::~TAnalysis()
{

}

void TAnalysis::AnalysisRecvData(QVector<double> &graph1_x, QVector<double> &graph1_y)
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
    unsigned char temp[65535];
    int ValueCount=0;
    for(int i=0;i<str.length();i+=3)
    {
        QString st = str.mid(i,2);//从i开始截取2个字符
        temp[ValueCount++] = HexToValue(st);//将hex字符转成数字值
    }
    qDebug("Value Counts:%d\n",ValueCount);
    switch (char step=0)
    {
        case 0://截取起始字节
        {
             if((temp[0]==0xE4)&&(temp[1]==0x2C)&&(temp[2]==0xE4)&&temp[3]==0x2C)
             {
                 step = 1;
             }
             else
             {
                 qDebug("StartBytes Error!\n");
                 return;
             }
        }

        case 1://截取通道数
        {
            m_Channels = temp[4];
            qDebug("m_Channels = %x\n",m_Channels);
            step = 2;
        }

        case 2: //截取通道长度
        {
            t_DataValue channellen;
            channellen.Data.HIGH_BYTE = temp[5];
            channellen.Data.LOW_BYTE = temp[6];
            m_ChannelsLeg = channellen.value;
            m_DotNum = temp[5]*0xffff+temp[6];
            qDebug("m_ChannelsLeg = %d\n",m_ChannelsLeg);
            qDebug("m_DotNum = %d\n",m_DotNum);
            step = 3;
        }

        case 3: //截取坐标数据
        {
            int pos = 7;
            t_DataValue dotvalue;
            double tmp[10];

            for(int n=0;n<1000;n++)
            {
                for(int m=0;m<m_Channels;m++)//通道的第一个点
                {
                    dotvalue.Data.HIGH_BYTE = temp[pos++];
                    dotvalue.Data.LOW_BYTE = temp[pos++];
                    tmp[m] = (double)dotvalue.value;
                }
                graph1_x[n] = (double)(n*m_ChannelsLeg/1000-(m_ChannelsLeg/2-1));
                graph1_y[n] = tmp[0];
                qDebug("tmp[0]=%f\n",tmp[0]);
            }
        }

        default:
            break;
    }

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

unsigned char TAnalysis::HexToValue(QString s)
{
       if(s.length()>2)
       {
           qDebug("The String is more than two charactors\n");
           return 0;
       }

       unsigned char n = 0;
       for (int i=0;i<2;i++)
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
