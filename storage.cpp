#include "storage.h"

storage::storage(size_t n):
    size(n),
    time(new QDateTime),
    value(new std::vector<int>),
    action(new std::vector<int>)
{
    value->reserve(size);
    action->reserve(size);
}


storage::~storage()
{
    delete time;
    delete value;
    delete action;
}
