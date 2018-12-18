#ifndef CONTROL_H
#define CONTROL_H

class SRegulator
{
public:
    SRegulator();
    ~SRegulator();

    int Regulator(int& value);

    void setAction(int i);
    void setControl(int i);
    void setTargetValue(int i);
    void setRelay_Gate(int i);
    void setPID_P(float i);
    void setPID_I(float i);
    void setPID_D(float i);

    int getValue();
    int getAction();
    int getError();

private:
    void RelayControl();
    void PIDControl();
    int CheckThreshold(int min, int max, int value);

public:
    enum Control
    {
        Manual,
        Relay,
        PID,
        NC
    };

private:
    struct ControlVal
    {
        int Value = 0;
        int Action = 0;
        int Error = 0;
    } values;

    struct ControlParameters
    {
        int Control = 0;
        int TargetValue = 0;
        int Relay_Gate = 0;
        float PID_P = 0;
        float PID_I = 0;
        float PID_D = 0;

    } parameters;

    struct PIDDate
    {
        int ISum = 0;
        static constexpr int ISumMax = 1000;
        static constexpr int ISumMin = -1000;
        int DLastError = 0;

    } PIDDate;
};

#endif // CONTROL_H
