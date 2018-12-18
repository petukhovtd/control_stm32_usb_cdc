#include "storage.h"

SStorage::SStorage(int n):
    size(n),
    value_mass(new QVector<int>),
    action_mass(new QVector<int>),
    time_mass(new QVector<qint64>)
{
    value_mass->reserve(size);
    action_mass->reserve(size);
    time_mass->reserve(size);
    StartTime = QDateTime::currentMSecsSinceEpoch(); //Start app time
}

SStorage::~SStorage()
{

}

/*
 * Add new date to array
*/
void SStorage::addPiont(int value, int action)
{
    value_mass->push_back(value);
    action_mass->push_back(action);
    time_mass->push_back(QDateTime::currentMSecsSinceEpoch()-StartTime);
}

/*
 * Copy new data to x_mass and y_mass
 * return true if there is new date
 * false if no new date
*/
bool SStorage::GenerateNewXY()
{
    int NowReadIndex = value_mass->size();
    if (NowReadIndex > LastReadIndex)
    {
        int NewSize = NowReadIndex - LastReadIndex;
        x_mass.resize(NewSize);
        y_mass.resize(NewSize);
        for (int i = 0; i < NewSize; ++i)
        {
            y_mass[i] = double(value_mass->at(i+LastReadIndex));
            x_mass[i] = double(time_mass->at(i+LastReadIndex));
        }
        LastReadIndex = NowReadIndex;
        return true;
    }
    return false;
}

/*
 * Return number of data read
*/
int SStorage::GetSize()
{
    return LastReadIndex;
}

/*
 * Save date to .csv file
 * Filename is Date+Time format yyyy-MM-dd hh-mm-ss.csv
 * Clear date mass
 * return true if write ok
 * false if open file or write error
*/
bool SStorage::SaveToFile()
{
    if (value_mass->size()>0)
    {
        std::string filename = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss").toStdString() + ".csv";
        std::fstream outfile;
        outfile.open(filename, std::fstream::out | std::fstream::trunc);
        if (outfile.is_open())
        {
            outfile << "Time,Value,Action\n";
            for (int i = 0; i<value_mass->size(); ++i)
            {
                outfile << time_mass->at(i) << "," << value_mass->at(i) << "," << action_mass->at(i) << "\n";
            }
            outfile.close();

            time_mass->resize(0);
            value_mass->resize(0);
            action_mass->resize(0);
            LastReadIndex = 0;

            return true;
        }
    }
    return false;
}
