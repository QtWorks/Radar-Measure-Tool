#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CECPort雷达调试助手");

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
    ui->StopBitComboBox->addItem("1");

    //预装校验位
    ui->PorityBitComboBox->addItem("0");
}

MainWindow::~MainWindow()
{
    delete ui;
}
