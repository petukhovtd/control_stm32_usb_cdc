#include "control.h"

SRegulator::SRegulator()
{

}

SRegulator::~SRegulator()
{

}

/*
 * Call regulate algoritm according to Control in Parameters
 * if Manual return Action from values
*/
int SRegulator::Regulator(int& value)
{
    values.Value = value;
    switch (parameters.Control)
    {
    case Manual:
        break;
    case Relay: RelayControl();
        break;
    case PID: PIDControl();
        break;
    case NC: return 0;
    default: return 0;
    }

    return values.Action;
}

/*
 * Relay control algoritm
*/
void SRegulator::RelayControl()
{
    values.Error = parameters.TargetValue - values.Value;

    if ((-values.Error) > parameters.Relay_Gate)
    {
        values.Action = 0;
    }
    else if (values.Error > parameters.Relay_Gate)
    {
        values.Action = 100;
    }
}

void SRegulator::PIDControl()
{
    values.Error = parameters.TargetValue - values.Value;
    float P = 0;
    float I = 0;
    float D = 0;
    int sum = 0;

    P = values.Error*parameters.PID_P;

    PIDDate.ISum += values.Error;
    PIDDate.ISum = CheckThreshold(PIDDate.ISumMin, PIDDate.ISumMax, PIDDate.ISum);
    I = PIDDate.ISum*parameters.PID_I;

    D = parameters.PID_D * (values.Error - PIDDate.DLastError);
    PIDDate.DLastError = values.Error;

    sum = P + I + D;

    values.Action = CheckThreshold(0, 100, sum);
}

int SRegulator::CheckThreshold(int min, int max, int value)
{
    if (value>max)
    {
        value = max;
    }
    else if (value<min)
    {
        value = min;
    }
    return value;
}

int SRegulator::getValue()
{
    return values.Value;
}
int SRegulator::getAction()
{
    return values.Action;
}
int SRegulator::getError()
{
    return values.Error;
}
void SRegulator::setAction(int i)
{
    values.Action = i;
}
void SRegulator::setControl(int i)
{
    parameters.Control = i;
}
void SRegulator::setTargetValue(int i)
{
    parameters.TargetValue = i;
}
void SRegulator::setRelay_Gate(int i)
{
    parameters.Relay_Gate = i;
}
void SRegulator::setPID_P(float i)
{
    parameters.PID_P = i;
}
void SRegulator::setPID_I(float i)
{
    parameters.PID_I = i;
}
void SRegulator::setPID_D(float i)
{
    parameters.PID_D = i;
}
