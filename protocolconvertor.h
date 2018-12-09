#ifndef PROTOCOLCONVERTOR_H
#define PROTOCOLCONVERTOR_H

#include <QByteArray>
#include <array>

class protocolconvertor
{
public:
    protocolconvertor();
    int InToValue(QByteArray b);
    std::pair<QByteArray, int> ValueToOut(int command, int value);

    enum Command
    {
        Reserve,
        GetInputValue,
        SetOutAction,
        SetF_in,
        SetF_out
    };

private:
    std::array<int, 10> Treshold {{
        0, 0,
        0, 0,
        0, 100,
        1, 10000,
        15, 15000 }};
};

#endif // PROTOCOLCONVERTOR_H
