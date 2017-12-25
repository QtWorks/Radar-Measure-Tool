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

    bool m_HexRecFlag;
    bool m_AsciiRecFlag;
    bool m_HexSendFlag;
    bool m_AsciiSendFlag;

private slots:
    //按钮处理
    void ClickStartButtonDispose();
    void ClickRecClearButtonDispose();
    void ClickSendButtonDispose();
    void ClickSaveScanRangeButtonDispose();
    //复选框处理
    void CheckRecHexDispose(bool checked){m_HexRecFlag = checked;}
    void CheckRecAsciiDispose(bool checked){m_AsciiRecFlag = checked;}
    void CheckSendHexDispose(bool checked){m_HexSendFlag = checked;}
    void CheckSendAsciiDispose(bool checked){m_AsciiSendFlag = checked;}
    //接收数据
    void ReceiveData();

private:
    Ui::MainWindow *ui;
    TCom *pCom;

};

#endif // MAINWINDOW_H
