#ifndef PROTOCOLCONVERTOR_H
#define PROTOCOLCONVERTOR_H

#include <QByteArray>

class SProtocolConvertor
{
public:
    SProtocolConvertor();
    ~SProtocolConvertor();

    int InToValue(QByteArray& b);
    QByteArray ValueToOut(int command, int value);

    enum Command
    {
        Reserve,
        GetInputValue,
        SetOutAction,
        SetF_in,
        SetF_out
    };
};

#endif // PROTOCOLCONVERTOR_H
