#ifndef STORAGE_H
#define STORAGE_H

#include <QVector>
#include <QDateTime>
#include <memory>
#include <fstream>


class SStorage
{
public:
    SStorage(int n);
    ~SStorage();

    void addPiont(int value, int action);
    bool GenerateNewXY();
    bool SaveToFile();
    int GetSize();

private:
    qint64 StartTime = 0;
    int size = 0;
    int LastReadIndex = 0;
    std::unique_ptr<QVector<int>> value_mass = nullptr;
    std::unique_ptr<QVector<int>> action_mass = nullptr;
    std::unique_ptr<QVector<qint64>> time_mass = nullptr;
public:
    QVector<double> x_mass;
    QVector<double> y_mass;
};

#endif // STORAGE_H
