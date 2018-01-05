#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <QArrayData>
#include <QByteArray>
#include <QVector>

typedef struct
{
    unsigned short x_value;
    unsigned short y_value;

}t_Dots;

typedef union
{
    struct
    {
        unsigned char LOW_BYTE;
        unsigned char HIGH_BYTE;

    }Data;

    short value;
}t_DataValue;

class TAnalysis
{
public:
    TAnalysis();
    ~TAnalysis();

    void AnalysisRecvData(QVector<double> &graph1_x, QVector<double> &graph1_y);//协议数据解析
    bool uStrcmp(unsigned char *str1,char *str2);
    unsigned char HexToValue(QString s);

public:
    unsigned char m_StartBytes[4];  //起始字节
    unsigned char m_Channels;       //通道数
    unsigned short m_ChannelsLeg;   //通道总长
    unsigned short m_PositiveSpeed;
    unsigned short m_PositiveAmp;
    unsigned short m_NegativeSpeed;
    unsigned short m_NegativeAmp;
    unsigned short m_StopByte[4];
    unsigned short m_DotNum;

};

#endif // ANALYSIS_H
