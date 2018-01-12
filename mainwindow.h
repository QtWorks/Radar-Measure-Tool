#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "Communicate/Com.h"
#include "Analysis/analysis.h"
#include "Qcustomplot/qcustomplot.h"
#include "qtimer.h"
#include "QPainter"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ShowWave();
    void ShowSpeed();

private slots:
    //按钮处理
    void on_RecClearPushButton_clicked();
    void on_SendPushButton_clicked();
    void on_StartPushButton_clicked();
    void on_SaveScanRangePushButton_clicked();
    void on_SaveSerialCfgPushButton_clicked();

    //复选框处理
    void on_HEXRecCheckBox_toggled(bool checked);
    void on_AsciiRecCheckBox_toggled(bool checked);
    void on_HEXSendCheckBox_toggled(bool checked);
    void on_AsciiSendCheckBox_toggled(bool checked);

    //接收数据
    void ReceiveData(); //串口数据触发
    void RecvWaveData(); //定时器触发
    void MeasurePoint(QMouseEvent *pEvent); //鼠标事件触发
    void MeasureSpeed(QMouseEvent *pEvent);

private:
    int index;
    bool m_StartButtonState;//开始按钮标志
    unsigned char OperateFlag;
    int m_XScanRange;
    int m_YScanRange;
    int m_DisplayPoint;

    Ui::MainWindow *ui;
    TCom *pCom;//串口
    TAnalysis *pAnalysis;
    QString PATH;
    QSettings *pSetting;
    QTimer *pTimer;
    QString m_AnalysisData;
};

#endif // MAINWINDOW_H

