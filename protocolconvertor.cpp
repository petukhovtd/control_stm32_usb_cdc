#include "protocolconvertor.h"
#include <regex>

#include <QDebug>

protocolconvertor::protocolconvertor()
{

}

int protocolconvertor::InToValue(QByteArray b)
{
    std::string s = b.toStdString();
    static std::regex re(R"(D(\d{1,2})=(\d{1,6});)");
    std::smatch sm;
    std::regex_match(s, sm, re);
    if (sm.size() == 3)
    {
        if (stoi(sm[1]) == 1)
        {
            return stoi(sm[2]);
        }
    }
    return 0;
}

std::pair<QByteArray, int> protocolconvertor::ValueToOut(int command, int value)
{
    int lo = protocolconvertor::Treshold[protocolconvertor::Command(command)*2];
    int hi = protocolconvertor::Treshold[protocolconvertor::Command(command)*2+1];
    if (value > hi)
    {
        value = hi;
    }
    else if (value < lo)
    {
        value = lo;
    }

    char buf[11];
    sprintf(buf, "D%d=%d;", command, value);
    std::string s = buf;
    return std::make_pair(QByteArray::fromStdString(s), value);
}
