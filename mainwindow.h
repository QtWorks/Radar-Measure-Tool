#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Communicate/Com.h"
#include "Qcustomplot/qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //按钮处理
    void on_RecClearPushButton_clicked();
    void on_SendPushButton_clicked();
    void on_StartPushButton_clicked();
    void on_SaveScanRangePushButton_clicked();

    //复选框处理
    void on_HEXRecCheckBox_toggled(bool checked){pCom->m_HexRecFlag = checked;}
    void on_AsciiRecCheckBox_toggled(bool checked){pCom->m_AsciiRecFlag = checked;}
    void on_HEXSendCheckBox_toggled(bool checked){pCom->m_HexSendFlag = checked;}
    void on_AsciiSendCheckBox_toggled(bool checked){pCom->m_AsciiSendFlag = checked;}
    //接收数据
    void ReceiveData();

private:
    Ui::MainWindow *ui;
    TCom *pCom;

    bool m_StartButtonState;

};

#endif // MAINWINDOW_H
