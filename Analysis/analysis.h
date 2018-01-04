#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <qarraydata.h>
#include <qbytearray.h>

typedef struct
{
    unsigned short x_value;
    unsigned short y_value;

}t_Dots;

class TAnalysis
{
public:
    TAnalysis();
    ~TAnalysis();

    void AnalysisRecvData();//协议数据解析
    bool uStrcmp(unsigned char *str1,char *str2);
    int HexToInt(QString s, char len);

public:
    unsigned char m_StartBytes[4];  //起始字节
    unsigned char m_Channels;       //通道数
    unsigned short m_ChannelsLeg;   //通道总长
      //每个点的值
    unsigned short m_PositiveSpeed;
    unsigned short m_PositiveAmp;
    unsigned short m_NegativeSpeed;
    unsigned short m_NegativeAmp;
    unsigned short m_StopByte[4];
};

#endif // ANALYSIS_H
