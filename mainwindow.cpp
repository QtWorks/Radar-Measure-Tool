#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("CECPort雷达调试助手");
    setWindowIcon(QIcon(":/new/images/Resouce/image.png")); //设置图标

    PATH = "config.xml";
    pSetting = new QSettings(PATH, QSettings::IniFormat);
    index=0;
    pCom = new TCom;
    pAnalysis = new TAnalysis;
    OperateFlag = false;

    m_Moment = 0;

    //开始按钮样式
    m_StartButtonState = false;
    ui->StartPushButton->setText("START");
    ui->StartPushButton->setStyleSheet("font-size:30px");

    //信号按钮样式
    ui->SignalPushButton->setStyleSheet("background-color:rgb(255,50,50)");

    //预装端口号
    pCom->SerialEnum();
    ui->PortComboBox->addItems(pCom->m_ComListName);

    //预装波特率
    ui->BaudComboBox->addItem("9600");
    ui->BaudComboBox->addItem("19200");
    ui->BaudComboBox->addItem("56000");
    ui->BaudComboBox->addItem("115200");
    ui->BaudComboBox->addItem("128000");
    ui->BaudComboBox->addItem("256000");

    //预装数据位
    ui->DataBitComboBox->addItem("8");
    ui->DataBitComboBox->addItem("7");
    ui->DataBitComboBox->addItem("6");
    ui->DataBitComboBox->addItem("5");

    //预装停止位
    ui->StopBitComboBox->addItem("1");
    ui->StopBitComboBox->addItem("2");

    //预装校验位
    ui->ParityBitComboBox->addItem("0");
    ui->ParityBitComboBox->addItem("1");
    ui->ParityBitComboBox->addItem("2");

    //选择串口配置
    index = pSetting->value("PortIndex").toInt();
    ui->PortComboBox->setCurrentIndex(index);
    index = pSetting->value("BaudIndex").toInt();
    ui->BaudComboBox->setCurrentIndex(index);
    index = pSetting->value("DatabitIndex").toInt();
    ui->DataBitComboBox->setCurrentIndex(index);
    index = pSetting->value("StopbitIndex").toInt();
    ui->StopBitComboBox->setCurrentIndex(index);
    index = pSetting->value("ParitybitIndex").toInt();
    ui->ParityBitComboBox->setCurrentIndex(0);

    //选择扫描范围配置
    index = pSetting->value("DisplayDotsNum").toInt();
    ui->PointNumLineEdit->setText(QString::number(index,10));
    m_DisplayPoint = index;//显示点数
    index = pSetting->value("XRange").toInt();
    ui->XRangeLineEdit->setText(QString::number(index,10));
    m_XScanRange = index;//x扫描范围
    index = pSetting->value("YRange").toInt();
    ui->YRangeLineEdit->setText(QString::number(index,10));
    m_YScanRange = index;//y扫描范围

    m_XStart = 0;
    m_XStop = m_XStart+(double)m_XScanRange;

    //复选框默认设置
    ui->HEXRecCheckBox->setChecked(true);
    ui->HEXSendCheckBox->setChecked(true);
    ui->AsciiRecCheckBox->setChecked(false);
    ui->AsciiSendCheckBox->setChecked(false);

    //波形图坐标设置
    //ui->ChannelWidget->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes|QCP::iSelectLegend|QCP::iSelectPlottables);
    //ui->ResultWidget->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom|QCP::iSelectAxes|QCP::iSelectLegend|QCP::iSelectPlottables);
    QBrush qBrushColor(QColor(50,50,50));
    ui->ChannelWidget->setBackground(qBrushColor);
    ui->ChannelWidget->legend->setVisible(false);
    //ui->ChannelWidget->legend->setBrush(QColor(50,50,50,0));//图例透明

    ui->ChannelWidget->xAxis->setRange(-127,127);
    ui->ChannelWidget->xAxis->setLabelColor(QColor(0,160,230));
    ui->ChannelWidget->xAxis->setTickLabelColor(Qt::white);
    ui->ChannelWidget->xAxis->setBasePen(QColor(32,178,170));
    ui->ChannelWidget->xAxis->setTickPen(QColor(255,165,0));
    ui->ChannelWidget->xAxis->setSubTicks(true);

    ui->ChannelWidget->yAxis->setRange(-65535,65535);
    ui->ChannelWidget->yAxis->setLabelColor(QColor(0,160,230));
    ui->ChannelWidget->yAxis->setTickLabelColor(Qt::white);
    ui->ChannelWidget->yAxis->setBasePen(QColor(32,178,170));
    ui->ChannelWidget->yAxis->setTickPen(QColor(255,165,0));
    ui->ChannelWidget->yAxis->setSubTicks(true);

    //ui->ChannelWidget->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignBottom | Qt::AlignRight);
    ui->ChannelWidget->xAxis->setRange(-(pAnalysis->m_ChannelsLeg/2-1),(pAnalysis->m_ChannelsLeg/2-1));
    ui->ChannelWidget->yAxis->setRange(-65535,65535);
    for(int i=0;i<pAnalysis->m_Channels;i++)//根据通道数添加图层
    {
        ui->ChannelWidget->addGraph();
    }

    ui->ResultWidget->setBackground(qBrushColor);
    ui->ResultWidget->legend->setVisible(false);

    ui->ResultWidget->xAxis->setRange(m_XStart,m_XStop);
    ui->ResultWidget->xAxis->setLabelColor(QColor(0,160,230));
    ui->ResultWidget->xAxis->setTickLabelColor(Qt::white);
    ui->ResultWidget->xAxis->setBasePen(QColor(32,178,170));
    ui->ResultWidget->xAxis->setTickPen(QColor(255,165,0));

    ui->ResultWidget->yAxis->setRange(-(m_YScanRange),m_YScanRange);
    ui->ResultWidget->yAxis->setLabelColor(QColor(0,160,230));
    ui->ResultWidget->yAxis->setTickLabelColor(Qt::white);
    ui->ResultWidget->yAxis->setBasePen(QColor(32,178,170));
    ui->ResultWidget->yAxis->setTickPen(QColor(255,165,0));
    ui->ResultWidget->addGraph();

    pTimer = new QTimer();
    pTimer->setInterval(100);
    //pTimer->start();
    connect(pTimer,SIGNAL(timeout()),this,SLOT(RecvWaveData()));
    connect(ui->ChannelWidget,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(MeasurePoint(QMouseEvent*)));
    connect(ui->ResultWidget,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(MeasureSpeed(QMouseEvent*)));

    return;
}

MainWindow::~MainWindow()
{
    delete pCom;
    delete pAnalysis;
    delete ui;
    delete pSetting;
    delete pTimer;
}

void MainWindow::on_RecClearPushButton_clicked()
{
    ui->RecDataTextBrowser->setText("");

    return;
}

void MainWindow::on_SaveScanRangePushButton_clicked()
{
    index = ui->XRangeLineEdit->displayText().toInt();
    pSetting->setValue("XRange",index);
    m_XScanRange = index;
    index = ui->YRangeLineEdit->displayText().toInt();
    pSetting->setValue("YRange",index);
    m_YScanRange = index;
    index = ui->PointNumLineEdit->displayText().toInt();
    pSetting->setValue("DisplayDotsNum",index);
    m_DisplayPoint = index;

    ui->ResultWidget->xAxis->setRange((m_XStop-(double)m_XScanRange),m_XStop);
    ui->ResultWidget->yAxis->setRange(-(m_YScanRange),m_YScanRange);
    ui->ResultWidget->replot();
    return;
}

void MainWindow::on_SaveSerialCfgPushButton_clicked()
{
    index = ui->PortComboBox->currentIndex();
    pSetting->setValue("PortIndex",index);
    index = ui->BaudComboBox->currentIndex();
    pSetting->setValue("BaudIndex",index);
    index = ui->DataBitComboBox->currentIndex();
    pSetting->setValue("DatabitIndex",index);
    index = ui->StopBitComboBox->currentIndex();
    pSetting->setValue("StopbitIndex",index);
    index = ui->ParityBitComboBox->currentIndex();
    pSetting->setValue("ParitybitIndex",index);

    return;
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
        QString PortName = ui->PortComboBox->currentText();//获取当前串口号字符串
        QString BaudRate = ui->BaudComboBox->currentText();
        QString DataBit = ui->DataBitComboBox->currentText();
        QString StopBit = ui->StopBitComboBox->currentText();
        QString ParityBit = ui->ParityBitComboBox->currentText();
        pCom->SerialConfig(&PortName,&BaudRate,&DataBit,&StopBit,&ParityBit);
        if(!pCom->SerialOpen())
        {
            QMessageBox::warning(this,tr("Warning"),tr("There are no any available serials!"),QMessageBox::Abort);
            return;
        }
        connect(pCom->pSerialCom, SIGNAL(readyRead()), this, SLOT(ReceiveData()));//连接串口到显示区
        pTimer->start();
        //connect(pCom->pSerialCom,SIGNAL(readyRead()),this,SLOT(ShowWave()));
        m_StartButtonState = true;
        ui->StartPushButton->setText("STOP");
        ui->SignalPushButton->setStyleSheet("background-color:rgb(50,255,50)");
    }
    else
    {
        m_StartButtonState = false;
        pCom->SerialClose();
        ui->StartPushButton->setText("START");
        ui->SignalPushButton->setStyleSheet("background-color:rgb(255,50,50)");
        pTimer->stop();
    }

    return;
}

void MainWindow::on_HEXRecCheckBox_toggled(bool checked)
{
    if(pCom->m_AsciiRecFlag)
    {
        pCom->m_AsciiRecFlag=false;
        ui->AsciiRecCheckBox->setChecked(false);
    }
    pCom->m_HexRecFlag = checked;

    return;
}

void MainWindow::on_AsciiRecCheckBox_toggled(bool checked)
{
    if(pCom->m_HexRecFlag)
    {
        pCom->m_HexRecFlag=false;
        ui->HEXRecCheckBox->setChecked(false);
    }
    pCom->m_AsciiRecFlag = checked;

    return;
}

void MainWindow::on_HEXSendCheckBox_toggled(bool checked)
{
    if(pCom->m_AsciiSendFlag)
    {
        pCom->m_AsciiSendFlag=false;
        ui->AsciiSendCheckBox->setChecked(false);
    }
    pCom->m_HexSendFlag = checked;

    return;
}

void MainWindow::on_AsciiSendCheckBox_toggled(bool checked)
{
    if(pCom->m_HexSendFlag)
    {
        pCom->m_HexSendFlag=false;
        ui->HEXSendCheckBox->setChecked(false);
    }
    pCom->m_AsciiSendFlag = checked;

    return;
}

void MainWindow::ReceiveData()
{
    //QString RecDataAscii;
    //pCom->SerialRecData(&RecDataAscii);

    return;
}

void MainWindow::ShowWave()
{
    unsigned short nSize = pAnalysis->m_DisplayDotNum ;
    QVector<double> x(nSize),y(nSize);

    for(int i=0;i<nSize;i++)
    {
        x[i] = pAnalysis->m_Channel_x[i];
        y[i] = pAnalysis->m_Channel1_y[i];
    }
    QPen Pen;
    Pen.setWidth(1);
    Pen.setColor(Qt::green);
    ui->ChannelWidget->graph(0)->setPen(Pen);
    ui->ChannelWidget->graph(0)->setData(x,y);
    ui->ChannelWidget->replot();

    for(int j=0;j<nSize;j++)
    {
        x[j] = pAnalysis->m_Channel_x[j];
        y[j] = pAnalysis->m_Channel2_y[j];
    }
    Pen.setWidth(1);
    Pen.setColor(Qt::blue);
    ui->ChannelWidget->graph(1)->setPen(Pen);
    ui->ChannelWidget->graph(1)->setData(x,y);
    ui->ChannelWidget->replot();

    for(int k=0;k<nSize;k++)
    {
        x[k] = pAnalysis->m_Channel_x[k];
        y[k] = pAnalysis->m_Channel3_y[k];
    }
    Pen.setWidth(1);
    Pen.setColor(Qt::yellow);
    ui->ChannelWidget->graph(2)->setPen(Pen);

    ui->ChannelWidget->graph(2)->setData(x,y);
    ui->ChannelWidget->replot();

    for(int l=0;l<nSize;l++)
    {
        x[l] = pAnalysis->m_Channel_x[l];
        y[l] = pAnalysis->m_Channel4_y[l];
    }
    Pen.setWidth(1);
    Pen.setColor(Qt::red);
    ui->ChannelWidget->graph(3)->setPen(Pen);
    ui->ChannelWidget->graph(3)->setData(x,y);
    ui->ChannelWidget->replot();


    return;
}

void MainWindow::RecvWaveData()
{
    if(m_StartButtonState)
    {
       QString RecDataAscii;
       pCom->SerialRecData(&RecDataAscii);
       ui->RecDataTextBrowser->insertPlainText(RecDataAscii);
       if(!OperateFlag)
       {
               if(!RecDataAscii.isEmpty())
               {
                   pTimer->stop();
                   pTimer->setInterval(800);
                   pTimer->start();
                   OperateFlag=true;
                   RecDataAscii.clear();
                   return;
               }
               m_Moment += 0.1;//时刻前进0.1s
       }

       if( RecDataAscii.contains("E4 2C E4 2C") )
       {
            m_AnalysisData = RecDataAscii.section("E4 2C E4 2C",1,1);
            m_AnalysisData.prepend("E4 2C E4 2C");
            if(RecDataAscii.contains("E4 8B E4 8B"))
            {
                 //qDebug()<<m_AnalysisData;
                 pAnalysis->AnalysisRecvData(m_AnalysisData);
                 ShowWave();
                 ShowSpeed();
            }
            m_Moment += 0.8;//时刻前进0.8s
       }

    }

    return;
}

void MainWindow::MeasurePoint(QMouseEvent *pEvent)
{
    int x_pose = pEvent->pos().x();
    int y_pose = pEvent->pos().y();
    float x_val = ui->ChannelWidget->xAxis->pixelToCoord(x_pose);
    float y_val = ui->ChannelWidget->yAxis->pixelToCoord(y_pose);
    QPen Pen;
    Pen.setColor(Qt::white);
    Pen.setWidth(1);
    Pen.setStyle(Qt::DashLine);

    QVector<double> x(0x1fffe),y(0x1fffe);
    for(long int i=0;i<0x1fffe;i++)
    {
        x[i] = (double)x_val;
        y[i] = (double)(i-0xffff);
    }
    ui->ChannelWidget->addGraph();
    ui->ChannelWidget->graph(pAnalysis->m_Channels)->setPen(Pen);
    ui->ChannelWidget->graph(pAnalysis->m_Channels)->setData(x,y);
    ui->ChannelWidget->replot();

    int n=0;
    n = ((unsigned short)x_val+(pAnalysis->m_ChannelsLeg/2-1))*pAnalysis->m_DisplayDotNum/pAnalysis->m_ChannelsLeg;

    //qDebug("x:%f,y:%f\n",x_val,y_val);
    //qDebug("n=%d\n",n);
    return;
}

void MainWindow::MeasureSpeed(QMouseEvent *pEvent)
{
    int x_pose = pEvent->pos().x();
    int y_pose = pEvent->pos().y();
    float x_val = ui->ResultWidget->xAxis->pixelToCoord(x_pose);
    float y_val = ui->ResultWidget->yAxis->pixelToCoord(y_pose);
    QPen Pen;
    Pen.setColor(Qt::white);
    Pen.setWidth(1);
    Pen.setStyle(Qt::DashLine);

    QVector<double> x(m_YScanRange*2),y(m_YScanRange*2);
    for(long int i=0;i<(m_YScanRange*2);i++)
    {
        x[i] = (double)x_val;
        y[i] = (double)(i-m_YScanRange);
    }
    ui->ResultWidget->addGraph();
    ui->ResultWidget->graph(1)->setPen(Pen);
    ui->ResultWidget->graph(1)->setData(x,y);
    ui->ResultWidget->replot();

    int n=0;
    n = ((int)x_val-m_XStart)/m_XScanRange*m_DisplayPoint;

    //qDebug("x:%f,y:%f\n",x_val,y_val);
    //qDebug("n=%d\n",n);

    return;
}

void MainWindow::ShowSpeed()
{
    //qDebug("%f,%f,%f,%f\n",pAnalysis->m_PositiveSpeed,pAnalysis->m_PositiveAmp,pAnalysis->m_NegativeSpeed,pAnalysis->m_NegativeAmp);
    static int count=0;
    if(count>m_DisplayPoint)
    {
        Speed_X.removeFirst();
        Speed_Y.removeFirst();
        count--;
    }

    QPen Pen;
    Pen.setWidth(2);
    //m_Moment += 0.5;
    Speed_X<<m_Moment;
    Speed_Y<<pAnalysis->m_NegativeAmp;
    if(m_Moment>m_XStop)
    {
        m_XStart++;
        m_XStop++;
        ui->ResultWidget->xAxis->setRange(m_XStart,m_XStop);
        ui->ResultWidget->replot();
    }
    if(pAnalysis->m_NegativeAmp>0)
    {
        Pen.setColor(Qt::green);
        ui->ResultWidget->graph(0)->setPen(Pen);
        ui->ResultWidget->graph(0)->setData(Speed_X,Speed_Y);
        ui->ResultWidget->replot();
    }
    else
    {
        Pen.setColor(Qt::red);
        ui->ResultWidget->graph(0)->setPen(Pen);
        ui->ResultWidget->graph(0)->setData(Speed_X,Speed_Y);
        ui->ResultWidget->replot();
    }

    count++;

    return;
}

