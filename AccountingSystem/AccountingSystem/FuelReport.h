#pragma once
#include <string>
#include <memory>
#include "IDataHandler.h"

class FuelReport
{
public:
    struct Data
    {
        int ID;
        int DriverID;
        int Liters;
        int GasStationID;
        std::string Date;
        static int GetID(std::string line);
        static std::string GetCSVFormat(const Data& data);
        static Data GetData(const std::string& line);
    };

    struct UserInputData
    {
        int DriverID;
        int Liters;
        int GasStationID;
        std::string Date;
        UserInputData(int driverID, int liters, int gasStationID, std::string date = GetCurrentDate()) : DriverID(driverID), 
                   Liters(liters), GasStationID(gasStationID), Date(date) {}
    };

    FuelReport(std::unique_ptr<IDataHandler<Data>>  handler);
    void Add(UserInputData userData);
    int Delete(const int& id);
    int Update(const int& id, const UserInputData& userData);
    Data Get(const int& id);

private:
    int GenerateNextID();
    Data InitData(const int& id,const UserInputData& userData);
    static std::string GetCurrentDate();
    std::unique_ptr<IDataHandler<Data>> _handler;

};