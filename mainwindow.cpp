#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CECPort雷达调试助手");

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

    //连接信号与功能
    connect(pCom->pSerialCom, SIGNAL(readyRead()), this, SLOT(ReceiveData()));

    connect(ui->StartPushButton, SIGNAL(clicked(bool)), this, SLOT(ClickStartButtonDispose()));
    connect(ui->RecClearPushButton,SIGNAL(clicked(bool)),this,SLOT(ClickRecClearButtonDispose()));
    connect(ui->SendPushButton,SIGNAL(clicked(bool)),this,SLOT(ClickSendButtonDispose()));
    connect(ui->SaveScanRangePushButton,SIGNAL(clicked(bool)),this,SLOT(ClickSaveScanRangeButtonDispose()));

    connect(ui->HEXRecCheckBox,SIGNAL(clicked(bool)),this,SLOT(CheckRecHexDispose()));
    connect(ui->HEXSendCheckBox,SIGNAL(clicked(bool)),this,SLOT(CheckSendHexDispose()));
    connect(ui->AsciiRecCheckBox,SIGNAL(clicked(bool)),this,SLOT(CheckRecAsciiDispose()));
    connect(ui->AsciiSendCheckBox,SIGNAL(clicked(bool)),this,SLOT(CheckSendAsciiDispose()));

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

void MainWindow::ClickRecClearButtonDispose()
{


}

void MainWindow::ClickSaveScanRangeButtonDispose()
{


}

void MainWindow::ClickSendButtonDispose()
{

}

void MainWindow::ClickStartButtonDispose()
{


}

void MainWindow::ReceiveData()
{


}
