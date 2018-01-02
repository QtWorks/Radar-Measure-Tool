#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CECPort雷达调试助手");

    m_StartButtonState = false;
    ui->StartPushButton->setText("STOP");

    pCom = new TCom;
    //预装端口号
    ui->PortComboBox->addItem("COM1");
    ui->PortComboBox->addItem("COM2");
    ui->PortComboBox->addItem("COM3");

    //预装波特率
    ui->BaudComboBox->addItem("115200");
    ui->BaudComboBox->addItem("128000");
    ui->BaudComboBox->addItem("256000");

    //预装数据位
    ui->DataBitComboBox->addItem("8");

    //预装停止位
    ui->StopBitComboBox->addItem("0");
    ui->StopBitComboBox->addItem("1");
    ui->StopBitComboBox->addItem("2");

    //预装校验位
    ui->ParityBitComboBox->addItem("0");
    ui->ParityBitComboBox->addItem("1");
    ui->ParityBitComboBox->addItem("2");

    //复选框默认设置
    ui->HEXRecCheckBox->setChecked(false);
    ui->HEXSendCheckBox->setChecked(false);
    ui->AsciiRecCheckBox->setChecked(true);
    ui->AsciiSendCheckBox->setChecked(true);

    //波形图坐标设置
    //ui->ChannelWidget->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes|QCP::iSelectLegend|QCP::iSelectPlottables);
    //ui->ResultWidget->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes|QCP::iSelectLegend|QCP::iSelectPlottables);
    QBrush qBrushColor(QColor(255,255,255));
    ui->ChannelWidget->setBackground(qBrushColor);
    ui->ChannelWidget->legend->setVisible(true);
    ui->ChannelWidget->xAxis->setRange(-127,127);
    ui->ChannelWidget->yAxis->setRange(-10,10);
    for(int i=0;i<pCom->pAnalysis->m_Channels;i++)//根据通道数添加图层
    {
        ui->ChannelWidget->addGraph();
    }
    Pen.setWidth(3);
    Pen.setColor(Qt::red);
    ui->ChannelWidget->graph(0)->setPen(Pen);
    QVector<double> x(pCom->pAnalysis->m_ChannelsLeg),y(pCom->pAnalysis->m_ChannelsLeg);
    for(int j=0;j<pCom->pAnalysis->m_ChannelsLeg;j++ )
    {
        x[j]=j-(pCom->pAnalysis->m_ChannelsLeg/2-1);
        y[j]=0;
    }
    ui->ChannelWidget->graph(0)->setData(x,y);
    Pen.setWidth(3);
    Pen.setColor(Qt::blue);
    ui->ChannelWidget->graph(1)->setPen(Pen);
    ui->ChannelWidget->graph(1)->setData(x,y);
    Pen.setWidth(3);
    Pen.setColor(Qt::green);
    ui->ChannelWidget->graph(2)->setPen(Pen);
    ui->ChannelWidget->graph(2)->setData(x,y);
    ui->ChannelWidget->replot();

    ui->ResultWidget->setBackground(qBrushColor);
    ui->ResultWidget->legend->setVisible(true);
    ui->ResultWidget->xAxis->setRange(-127,127);
    ui->ResultWidget->yAxis->setRange(-10,10);
}

MainWindow::~MainWindow()
{
    delete pCom;
    delete ui;
}

void MainWindow::on_RecClearPushButton_clicked()
{
    ui->RecDataTextBrowser->setText("");

    return;
}

void MainWindow::on_SaveScanRangePushButton_clicked()
{


}

void MainWindow::on_SaveSerialCfgPushButton_clicked()
{

}

void MainWindow::on_SendPushButton_clicked()
{
    QString SendData = ui->SendDatalineEdit->displayText();
    pCom->SerialSendData(&SendData);

    return;
}

void MainWindow::on_StartPushButton_clicked()
{
    if(!m_StartButtonState)
    {
        m_StartButtonState = true;

        QString PortName = ui->PortComboBox->currentText();//获取当前串口号字符串
        QString BaudRate = ui->BaudComboBox->currentText();
        QString DataBit = ui->DataBitComboBox->currentText();
        QString StopBit = ui->StopBitComboBox->currentText();
        QString ParityBit = ui->ParityBitComboBox->currentText();

        pCom->SerialConfig(&PortName,&BaudRate,&DataBit,&StopBit,&ParityBit);
        pCom->SerialOpen();
        connect(pCom->pSerialCom, SIGNAL(readyRead()), this, SLOT(ReceiveData()));//连接串口到显示区
        connect(pCom->pSerialCom,SIGNAL(readyRead()),this,SLOT(ShowWave()));
        ui->StartPushButton->setText("START");
    }
    else
    {
        m_StartButtonState = false;
        pCom->SerialClose();
        ui->StartPushButton->setText("STOP");
    }

    return;
}

void MainWindow::on_HEXRecCheckBox_toggled(bool checked)
{
    if(pCom->m_AsciiRecFlag){pCom->m_AsciiRecFlag=false;}
    ui->AsciiRecCheckBox->setChecked(false);
    pCom->m_HexRecFlag = checked;

    return;
}

void MainWindow::on_AsciiRecCheckBox_toggled(bool checked)
{
    if(pCom->m_HexRecFlag){pCom->m_HexRecFlag=false;}
    ui->HEXRecCheckBox->setChecked(false);
    pCom->m_AsciiRecFlag = checked;

    return;
}

void MainWindow::on_HEXSendCheckBox_toggled(bool checked)
{
    if(pCom->m_AsciiSendFlag){pCom->m_AsciiSendFlag=false;}
    ui->AsciiSendCheckBox->setChecked(false);
    pCom->m_HexSendFlag = checked;

    return;
}

void MainWindow::on_AsciiSendCheckBox_toggled(bool checked)
{
    if(pCom->m_HexSendFlag){pCom->m_HexSendFlag=false;}
    ui->HEXSendCheckBox->setChecked(false);
    pCom->m_AsciiSendFlag = checked;

    return;

}

void MainWindow::ReceiveData()
{
    QString RecDataAscii;
    pCom->SerialRecData(&RecDataAscii);
    ui->RecDataTextBrowser->insertPlainText(RecDataAscii);

    return;
}

void MainWindow::ShowWave()
{
    qDebug("Into ShowWave\n");

    return;
}


