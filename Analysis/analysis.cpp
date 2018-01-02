#include "Analysis/analysis.h"
#include "qbytearray.h"

TAnalysis::TAnalysis()
{
    m_Channels = 3;
    m_ChannelsLeg = 256;

}

TAnalysis::~TAnalysis()
{


}

void TAnalysis::AnalysisRecvData(QByteArray *RecvDataArray)
{
    QByteArray AnalysisBuf;
    strcpy(AnalysisBuf.data(),RecvDataArray->data());
    unsigned char StartBytes[4]={0xe4,0x2c,0xe4,0x2c};
    char *str = NULL;
    str=AnalysisBuf.mid(0,4).data();
    qDebug("str:%x,%x,%x,%x\n",(str[0]&0xff),(str[1]&0xff),(str[2]&0xff),(str[3]&0xff));
    if(uStrcmp(StartBytes,str))
    {
        qDebug("start ok...\n");
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
