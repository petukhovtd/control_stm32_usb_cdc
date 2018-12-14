#ifndef STORAGE_H
#define STORAGE_H

#include <QDateTime>
#include <vector>
#include <memory>


class storage
{
public:
    storage(size_t n);
    ~storage();

    void addPiont(int value, int action);
private:
    size_t size = 0;
    std::unique_ptr<std::vector<int>> value_mass;
    std::unique_ptr<std::vector<int>> action_mass;
    std::unique_ptr<std::vector<QDateTime>> time_mass;
};

#endif // STORAGE_H
