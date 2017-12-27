#include "mainwindow.h"
#include "ui_mainwindow.h"



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

    ui->WaveWidget->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes|QCP::iSelectLegend|QCP::iSelectPlottables);

    QBrush qBrushColor(QColor(255,255,255));
    ui->WaveWidget->setBackground(qBrushColor);

    ui->WaveWidget->legend->setVisible(true);

    ui->WaveWidget->xAxis->setLabel("Time Axis (T/s)");
    ui->WaveWidget->xAxis->setRange(0,60);

    ui->WaveWidget->yAxis->setLabel("Speed Axis (V)");
    ui->WaveWidget->yAxis->setRange(0,100);

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

void MainWindow::ReceiveData()
{
    QString RecDataAscii;
    pCom->SerialRecData(&RecDataAscii);
    ui->RecDataTextBrowser->insertPlainText(RecDataAscii);

    return;
}
