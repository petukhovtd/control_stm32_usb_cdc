#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QtSerialPort>
#include <QSerialPortInfo>

#include "protocolconvertor.h"
#include "control.h"
#include "storage.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void ComPortConnect();
    void ComPortRead();
    void SetInF();
    void SetOutF();
    void StartUpdateControl();
    void StopControl();
    void SavetoFile();
    void PlotClear();

    void UpdateInterface();

    void ReScanComPort();

private:
    void GetPortList();
    void ButtonStatefromConnect(bool state);
    void PlotConfig();

private:
    Ui::MainWindow *ui;
    QSerialPort *sp = nullptr;
    QTimer *timer = nullptr;
    std::unique_ptr<SProtocolConvertor> convert = nullptr;
    std::unique_ptr<SRegulator> reg = nullptr;
    std::unique_ptr<SStorage> buffer = nullptr;
};

#endif // MAINWINDOW_H
