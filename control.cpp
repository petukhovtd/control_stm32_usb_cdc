#include "control.h"

control::control()
{

}

int control::Regulator(int& value)
{
    switch (parameters.Control)
    {
    case Manual:
        break;
    case Relay: RelayControl(value);
        break;
    case PID: return 0;
    case NC: return 0;
    default: return 0;
    }

    return values.Action;
}

void control::RelayControl(int& value)
{
    values.Error = parameters.TargetValue - value;

    if (value > (parameters.TargetValue + parameters.Relay_Gate))
    {
        values.Action = 0;
    }
    else if (value < (parameters.TargetValue - parameters.Relay_Gate))
    {
        values.Action = 100;
    }
}


int control::getValue()
{
    return values.Value;
}
int control::getAction()
{
    return values.Action;
}
int control::getError()
{
    return values.Error;
}
void control::setAction(int i)
{
    values.Action = i;
}
void control::setControl(int i)
{
    parameters.Control = i;
}
void control::setTargetValue(int i)
{
    parameters.TargetValue = i;
}
void control::setRelay_Gate(int i)
{
    parameters.Relay_Gate = i;
}
void control::setPID_P(float i)
{
    parameters.PID_P = i;
}
void control::setPID_I(float i)
{
    parameters.PID_I = i;
}
void control::setPID_D(float i)
{
    parameters.PID_D = i;
}
