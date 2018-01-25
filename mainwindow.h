#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "Communicate/Com.h"
#include "Analysis/analysis.h"
#include "Qcustomplot/qcustomplot.h"
#include "qtimer.h"
#include "QPainter"
#include "QtGlobal"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

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
    void on_WaveLegendCheckBox_toggled(bool checked);
    void on_WaveMeasureCheckBox_toggled(bool checked);
    void on_SpeedLegendCheckBox_toggled(bool checked);
    void on_SpeedMeasureCheckBox_toggled(bool checked);


    //接收数据
    void ReceiveData(); //串口数据触发
    void RecvWaveData(); //定时器触发

    void ActiveMeasureCusor(QMouseEvent *pEvent);
    void ActiveSpeedCusor(QMouseEvent *pEvent);

    void MeasureCusorY1Dispose(QMouseEvent *pEvent);
    void MeasureCusorY2Dispose(QMouseEvent *pEvent);
    void MeasureCusorY3Dispose(QMouseEvent *pEvent);
    void MeasureCusorY4Dispose(QMouseEvent *pEvent);


private:
    int index;
    double Value;
    bool m_StartButtonState;//开始按钮标志
    unsigned char OperateFlag;
    int m_XScanRange;
    double m_XStart;
    double m_XStop;
    int m_YScanRange;
    int m_DisplayPoint;
    double m_Moment;
    bool FrameStartFlag;
    double m_DataRange;

    bool m_ActiveFlagPoint;
    bool m_ActiveFlagSpeed;

    Ui::MainWindow *ui;
    TCom *pCom;//串口
    TAnalysis *pAnalysis;
    QSettings *pSetting;
    //QTimer *pTimer;

    QString PATH;
    QString m_AnalysisData;
    QString m_SubAnalysisData;
    QVector<double> Speed_X,Speed_Y,Speed_NY;
    QVector<double> MeasureCusorX1,MeasureCusorY1,MeasureCuserX2,MeasureCuserY2;
    QVector<double> SpeedCusorX1, SpeedCusorY1, SpeedCusorX2, SpeedCusorY2;

};

#endif // MAINWINDOW_H

