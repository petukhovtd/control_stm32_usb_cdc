#ifndef CONTROL_H
#define CONTROL_H


class control
{
public:
    control();

    struct ControlParameters{
      int Control = 0;
      int TargetValue = 0;
      int RelayGate = 0;
      float PID_P = 0;
      float PID_I = 0;
      float PID_D = 0;
    };

    enum Control{
        Manual,
        Relay,
        PID,
        NC
        };

    ControlParameters parameters;
};

#endif // CONTROL_H
