#include "protocolconvertor.h"
#include <regex>

SProtocolConvertor::SProtocolConvertor()
{

}

SProtocolConvertor::~SProtocolConvertor()
{

}

/*
 * Input: QByteArray from com port
 *
 * Parse input messege Dx=y;
 * x - nuber command
 * y - date
 *
 * return y integer date
*/
int SProtocolConvertor::InToValue(QByteArray& b)
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


/*
 * Input: x,y integer
 *
 * Conver input x, y command to message
 * Dx=y;
 *
 * return QByteArray message
*/
QByteArray SProtocolConvertor::ValueToOut(int command, int value)
{
    char buf[11];
    sprintf(buf, "D%d=%d;", command, value);
    std::string s = buf;
    return QByteArray::fromStdString(s);
}
