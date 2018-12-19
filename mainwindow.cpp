#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    sp(new QSerialPort),
    timer(new QTimer),
    convert(new SProtocolConvertor),
    reg(new SRegulator),
    buffer(new SStorage(100000))
{
    ui->setupUi(this);

    GetPortList();
    ButtonStatefromConnect(false);
    PlotConfig();

    connect(ui->ConnectButton, SIGNAL(released()), this, SLOT(ComPortConnect()));
    connect(ui->SetInFButton, SIGNAL(released()), this, SLOT(SetInF()));
    connect(ui->SetOutFButton, SIGNAL(released()), this, SLOT(SetOutF()));
    connect(ui->StartUpdateControlButton, SIGNAL(released()), this, SLOT(StartUpdateControl()));
    connect(ui->StopControlButton, SIGNAL(released()), this, SLOT(StopControl()));
    connect(ui->SaveDateButton, SIGNAL(released()), this, SLOT(SavetoFile()));
    connect(ui->PlotClearButton, SIGNAL(released()), this, SLOT(PlotClear()));
    connect(ui->ReScanPortButton, SIGNAL(released()), this, SLOT(ReScanComPort()));

    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateInterface()));

    connect(sp, SIGNAL(readyRead()), this, SLOT(ComPortRead()));

    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * Active or disactive button from connect status
 *
 * IN: status for button send IN and OUT frequency
 * /IN status button Save to file date
 * Set text "Save to file" in SavetoFile button
*/
void MainWindow::ButtonStatefromConnect(bool state)
{
    ui->SetInFButton->setEnabled(state);
    ui->SetOutFButton->setEnabled(state);

    ui->SaveDateButton->setEnabled(!state);
    ui->SaveDateButton->setText("Save to file");
}

/*
 * Get active com-port list from system
 * and set list in сombo box
*/
void MainWindow::GetPortList()
{
    ui->PortSelectcomboBox->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        QStringList list;
        list << info.portName();
        ui->PortSelectcomboBox->addItem(list.first(), list);
    }
}

/*
 * Configurate plot wigdet
*/
void MainWindow::PlotConfig()
{
    ui->plotwidget->addGraph();
    ui->plotwidget->graph(0)->setLineStyle(QCPGraph::lsNone);
    ui->plotwidget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
    ui->plotwidget->xAxis->setLabel("Time from start (ms)");
    ui->plotwidget->yAxis->setLabel("V (mV)");
    ui->plotwidget->xAxis->rescale();
    ui->plotwidget->yAxis->setRange(0, 2000);
    ui->plotwidget->replot();
}

/*
 * Connect or disconnect com port
 * Targer com port select to ComboBox
*/
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
}

/*
 * Read date from com port to signal
 * Convert input string for int
 * Call regulator, get out value
 * Convet out to byte array
 * Send date
 * Add date to buffer
*/
void MainWindow::ComPortRead()
{
    QByteArray data = sp->readAll();
    int i = convert->InToValue(data);
    int action = reg->Regulator(i);
    QByteArray message = convert->ValueToOut(convert->SetOutAction, action);
    sp->write(message);
    buffer->addPiont(i, action);
}

/*
 * Get IN F of SpinBox
 * Convet to byte array
 * Send date
*/
void MainWindow::SetInF()
{
    int value = ui->InFspinBox->value();
    QByteArray message = convert->ValueToOut(convert->SetF_in, value);
    sp->write(message);
}

/*
 * Get OUT F of SpinBox
 * Convet to byte array
 * Send date
*/
void MainWindow::SetOutF()
{
    int value = ui->OutFspinBox->value();
    QByteArray message = convert->ValueToOut(convert->SetF_out, value);
    sp->write(message);
}

/*
 * Get control from UI
 * Set praram regulator
*/
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

/*
 * Stop control
 * Set Manual regulator
*/
void MainWindow::StopControl()
{
    reg->setControl(reg->Manual);
}

/*
 * set button state and text on Connect/Disconnect button
 * Chek acsses com port when disconnect
 * Update date by timer
 * Add point to graph
*/
void MainWindow::UpdateInterface()
{
    if(sp->isOpen())
    {
        ButtonStatefromConnect(true);
        ui->ConnectButton->setText("Disconnect");
    }
    else
    {
        ButtonStatefromConnect(false);
        ui->ConnectButton->setText("Connect");
//        GetPortList();
    }

    ui->InValueLabel->setText("Value: " + QString::number(reg->getValue()));
    ui->OutValueLabel->setText("Action: " + QString::number(reg->getAction()) + "%");
    ui->ErrorLabel->setText("Error: " + QString::number(reg->getError()));

    if (buffer->GenerateNewXY())
    {
        ui->plotwidget->graph(0)->addData(buffer->x_mass, buffer->y_mass);
        ui->plotwidget->xAxis->rescale();
        ui->plotwidget->yAxis->rescale();
        ui->plotwidget->yAxis->setRangeLower(0);
        ui->plotwidget->replot();

        ui->PointsLabel->setText("Points: " + QString::number(buffer->GetSize()));
    }
}

/*
 * Clear plot
 * Date not clear
*/
void MainWindow::PlotClear()
{
    if( ui->plotwidget->graphCount())
    {
        ui->plotwidget->clearGraphs();
        ui->plotwidget->addGraph();
        ui->plotwidget->graph(0)->setLineStyle(QCPGraph::lsNone);
        ui->plotwidget->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
        ui->plotwidget->replot();
    }
}

/*
 * Save date to .csv file
*/
void MainWindow::SavetoFile()
{
    if (buffer->SaveToFile())
    {
        ui->SaveDateButton->setText("Save ok");
    }
    else
    {
        ui->SaveDateButton->setText("Save error");
    }
}

void MainWindow::ReScanComPort()
{
    GetPortList();
}
