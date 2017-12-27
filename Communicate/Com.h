#ifndef COM_H
#define COM_H

#include <stdio.h>
#include "Qextserial/qextserialport.h"
#include "ui_mainwindow.h"

class TCom
{

public:
     explicit TCom();
     virtual ~TCom();

    QextSerialPort *pSerialCom; //串口通信调用指针

    bool SerialOpen();
    void SerialConfig(QString *PortName,QString *BaudRate, QString *Databit, QString *StopBit, QString *ParityBit);
    void SerialClose();
    void SerialSendData(QString *SendData);
    void SerialRecData(QString *RecDataAscii);

public:
    bool m_HexRecFlag;
    bool m_AsciiRecFlag;
    bool m_HexSendFlag;
    bool m_AsciiSendFlag;
    bool m_Handle;

};

#endif // COMBASE_H
