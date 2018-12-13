#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sp(new QSerialPort),
    convert(new protocolconvertor),
    reg(new control),
    timer(new QTimer)
{
    ui->setupUi(this);

    GetPortList();
    ButtonState(false);

    connect(ui->ConnectButton, SIGNAL(released()), this, SLOT(ComPortConnect()));
    connect(ui->SetInFButton, SIGNAL(released()), this, SLOT(SetInF()));
    connect(ui->SetOutFButton, SIGNAL(released()), this, SLOT(SetOutF()));
    connect(ui->StartUpdateControlButton, SIGNAL(released()), this, SLOT(StartUpdateControl()));
    connect(ui->StopControlButton, SIGNAL(released()), this, SLOT(StopControl()));

    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateInterface()));

    connect(sp, SIGNAL(readyRead()), this, SLOT(ComPortRead()));

    timer->start(1000);
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

        }
        else
        {
            sp->close();
        }
    }

    if(sp->isOpen())
    {
        ButtonState(true);
        ui->ConnectButton->setText("Disconnect");
    }
    else
    {
        ButtonState(false);
        ui->ConnectButton->setText("Connect");
    }
}

void MainWindow::ComPortRead()
{
    QByteArray data = sp->readAll();
    int i = convert->InToValue(data);

//    ui->InValueLabel->setText("Value: " + QString::number(i));

    int action = reg->Regulator(i);
    QByteArray message = convert->ValueToOut(convert->SetOutAction, action);
    sp->write(message);
}

void MainWindow::SetInF()
{
    int value = ui->InFspinBox->value();
    QByteArray message = convert->ValueToOut(convert->SetF_in, value);
    sp->write(message);
}

void MainWindow::SetOutF()
{
    int value = ui->OutFspinBox->value();
    QByteArray message = convert->ValueToOut(convert->SetF_out, value);
    sp->write(message);
}

void MainWindow::StartUpdateControl()
{
    reg->setAction(ui->ActionspinBox->value());
    reg->setTargetValue(ui->TargetValuespinBox->value());
    reg->setRelay_Gate(ui->GatespinBox->value());
    reg->setPID_P(float(ui->PIDPdoubleSpinBox->value()));
    reg->setPID_I(float(ui->PIDIdoubleSpinBox->value()));
    reg->setPID_D(float(ui->PIDDdoubleSpinBox->value()));
    reg->setControl(ui->ControlSelectWidget->currentIndex());
}

void MainWindow::StopControl()
{
    reg->setControl(reg->Manual);
}

void MainWindow::UpdateInterface()
{
    ui->InValueLabel->setText("Value: " + QString::number(reg->getValue()));
    ui->OutValueLabel->setText("Action: " + QString::number(reg->getAction()) + "%");
    ui->ErrorLabel->setText("Error: " + QString::number(reg->getError()));

}
