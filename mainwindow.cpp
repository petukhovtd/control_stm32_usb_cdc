#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sp(new QSerialPort),
    convert(new protocolconvertor),
    reg(new control)
{
    ui->setupUi(this);

    GetPortList();
    ButtonState(false);

    connect(ui->ConnectButton, SIGNAL(released()), this, SLOT(ComPortConnect()));
    connect(ui->SetInFButton, SIGNAL(released()), this, SLOT(SetInF()));
    connect(ui->SetOutFButton, SIGNAL(released()), this, SLOT(SetOutF()));
    connect(ui->SetActionButton, SIGNAL(released()), this, SLOT(SetAction()));
    connect(ui->StartUpdateControlButton, SIGNAL(released()), this, SLOT(StartUpdateControl()));
    connect(ui->StopControlButton, SIGNAL(relesed()), this, SLOT(StopControl()));

    connect(sp, SIGNAL(readyRead()), this, SLOT(ComPortRead()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete sp;
    delete convert;
    delete reg;
}

void MainWindow::ButtonState(bool state)
{
    ui->SetInFButton->setEnabled(state);
    ui->SetOutFButton->setEnabled(state);
    ui->SetActionButton->setEnabled(state);
}

void MainWindow::GetPortList()
{
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        QStringList list;
        list << info.portName();
        ui->PortSelectcomboBox->addItem(list.first(), list);
    }
}

void MainWindow::ComPortConnect()
{
    if(sp->isOpen())
    {
        sp->close();
        ui->ConnectButton->setText("Connect");
    }
    else
    {
        QString PortName = ui->PortSelectcomboBox->currentText();
        sp->setPortName(PortName);
        sp->setBaudRate(QSerialPort::Baud115200);
        sp->setDataBits(QSerialPort::Data8);
        sp->setParity(QSerialPort::NoParity);
        sp->setStopBits(QSerialPort::OneStop);
        sp->setFlowControl(QSerialPort::NoFlowControl);

        if(sp->open(QIODevice::ReadWrite))
        {
            ui->ConnectButton->setText("Disconnect");
        }
        else
        {
            sp->close();
        }
    }

    if(sp->isOpen())
    {
        ButtonState(true);
    }
    else
    {
        ButtonState(false);
    }
}

void MainWindow::ComPortRead()
{
    const QByteArray data = sp->readAll();
    int i = convert->InToValue(data);
    ui->InValueLabel->setText("Value: " + QString::number(i));
}

void MainWindow::SetInF()
{
    int value = ui->InFEdit->text().toInt();
    auto r = convert->ValueToOut(convert->SetF_in, value);
    QByteArray message = r.first;
    int nvalue = r.second;
    if (nvalue != value)
    {
        ui->InFEdit->setText(QString::number(nvalue));
    }
    sp->write(message);
}

void MainWindow::SetOutF()
{
    int value = ui->OutFEdit->text().toInt();
    auto r = convert->ValueToOut(convert->SetF_out, value);
    QByteArray message = r.first;
    int nvalue = r.second;
    if (nvalue != value)
    {
        ui->OutFEdit->setText(QString::number(nvalue));
    }
    sp->write(message);
}

void MainWindow::SetAction()
{
    int value = ui->ActionEdit->text().toInt();
    auto r = convert->ValueToOut(convert->SetOutAction, value);
    QByteArray message = r.first;
    int nvalue = r.second;
    if (nvalue != value)
    {
        ui->ActionEdit->setText(QString::number(nvalue));
    }
    sp->write(message);
}

void MainWindow::StartUpdateControl()
{
    reg->parameters.TargetValue = ui->TargetValueEdit->text().toInt();
    reg->parameters.RelayGate = ui->RelayGateEdit->text().toInt();
    reg->parameters.PID_P = ui->PIDPEdit->text().toInt();
    reg->parameters.PID_I = ui->PIDIEdit->text().toInt();
    reg->parameters.PID_D = ui->PIDDEdit->text().toInt();

    reg->parameters.Control = ui->ControlSelectWidget->currentIndex() + 1;
}

void MainWindow::StopControl()
{
    reg->parameters.Control = reg->Manual;
}
