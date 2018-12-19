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
    int date = -1;
    std::string s = b.toStdString();
    static std::regex re(R"(D(\d{1,2})=(\d{1,6});)");
    std::smatch sm;
    while(std::regex_search(s, sm, re))
    {
        if ((sm.size() == 3) & (stoi(sm[1]) == 1))
        {
            date = stoi(sm[2]);
        }
        s = sm.suffix();
    }
    return date;
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
