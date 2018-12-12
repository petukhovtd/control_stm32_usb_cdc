#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <QtSerialPort>
#include <QSerialPortInfo>

#include "protocolconvertor.h"
#include "control.h"

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

    void UpdateInterface();

private:
    void GetPortList();
    void ButtonState(bool state);

private:
    Ui::MainWindow *ui;
    QSerialPort *sp = nullptr;
    protocolconvertor *convert = nullptr;
    control *reg = nullptr;
    QTimer *timer = nullptr;
};

#endif // MAINWINDOW_H
