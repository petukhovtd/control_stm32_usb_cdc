#ifndef STORAGE_H
#define STORAGE_H

#include <QDateTime>
#include <vector>


class storage
{
public:
    storage(size_t n);
    ~storage();

private:
    size_t size = 0;
    QDateTime *time = nullptr;
    std::vector<int> *value = nullptr;
    std::vector<int> *action = nullptr;

};

#endif // STORAGE_H
