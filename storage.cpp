#include "storage.h"

storage::storage(size_t n):
    size(n)
{
    value_mass->reserve(size);
    action_mass->reserve(size);
    time_mass->reserve(size);
}

void storage::addPiont(int value, int action)
{
    value_mass->push_back(value);
    action_mass->push_back(action);
//    time_mass->push_back(QDateTime::)

}

storage::~storage()
{

}
