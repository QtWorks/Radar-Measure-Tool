#ifndef COM_H
#define COM_H

#include <stdio.h>
#include "Qextserial/qextserialport.h"
#include "ui_mainwindow.h"

class TCom
{

public:
     explicit TCom();
    ~TCom();

    QextSerialPort *pSerialCom; //串口通信调用指针

    void SerialOpen();
    void SerialConfig();
    void SerialClose();
    void SerialSendData();
    void SerialRecData();

private:
    Ui::MainWindow *pMainWindow;

};

#endif // COMBASE_H
