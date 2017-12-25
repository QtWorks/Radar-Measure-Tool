
#include "Com.h"


//构造
TCom::TCom()
{
    pSerialCom = NULL;
    return;
}

//析构
TCom::~TCom()
{
    delete pSerialCom;
    return;
}

bool TCom::SerialOpen(void)
{
    SerialConfig();//配置
    bool bHandle = pSerialCom->open(QIODevice::ReadWrite);//以可读写方式打开
    if(bHandle == false)
    {
        return false;
    }

    return true;
}

void TCom::SerialClose()
{
    pSerialCom->close();
    return;
}

void TCom::SerialConfig()
{
    QString PortName = pMainWindow->ParityBitComboBox->currentText();//选择当前串口号
    pSerialCom = new QextSerialPort(PortName);

    pSerialCom->setBaudRate((BaudRateType)pMainWindow->BaudComboBox->currentText().toInt());//设置串口波特率
    pSerialCom->setDataBits((DataBitsType)pMainWindow->DataBitComboBox->currentText().toInt());//设置串口数据位
    switch(pMainWindow->ParityBitComboBox->currentIndex())//设置串口校验位
    {
        case '0':pSerialCom->setParity(PAR_NONE);break;
        case 1:pSerialCom->setParity(PAR_ODD);break;
        case 2:pSerialCom->setParity(PAR_EVEN);break;
        default:pSerialCom->setParity(PAR_NONE);break;
    }
    switch(pMainWindow->StopBitComboBox->currentIndex())//设置串口停止位
    {
        case 0:pSerialCom->setStopBits(STOP_1);
        case 1:pSerialCom->setStopBits(STOP_1_5);
        case 2:pSerialCom->setStopBits(STOP_2);
        default:pSerialCom->setStopBits(STOP_1);
    }

    pSerialCom->setFlowControl(FLOW_OFF);//设置控制流
    pSerialCom->setTimeout(200);//设置延时

    return;
}

void TCom::SerialRecData()
{
    QByteArray RecDataBuf = pSerialCom->readAll();
    QString RecDataAscii;
    RecDataAscii = QString(RecDataBuf);
    pMainWindow->RecDataTextBrowser->insertPlainText(RecDataAscii);
    return;
}

void TCom::SerialSendData()
{
    if(!pSerialCom->isOpen())return;
    QString SendData = pMainWindow->SendDatalineEdit->displayText();
    if(SendData.isEmpty())return;
    QByteArray SendDataArray = "";
    SendDataArray = SendData.toLatin1();
    pSerialCom->write(SendDataArray);

    return;
}
