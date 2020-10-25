#pragma once
#include <string>
#include <memory>
#include "IDataHandler.h"

class GasStation
{
public:
    struct GasStationData
    {
        int ID;
        std::string Name;
        std::string Surname;
        int Liters;
        std::string Date;
        int Ballance;
        static int GetID(std::string line);
        static std::string GetCSVFormat(GasStationData userData);
        static GasStationData GetGasStationData(std::string line);
    };

    struct InputData
    {
        std::string Name;
        std::string Surname;
        int Liters;
    };

	GasStation(std::unique_ptr<IDataHandler<GasStationData>>  handler);
	void AddRecord(InputData userData, const bool IncreaseTheBallance = false);
    int DeleteRecord(const int id);
    int UpdateRecord(const int& id, const InputData& userData);
    GasStationData GetRecord(int id);

private:
    int GenerateNextId();
    int GetCurrentBallance();
    std::string GetCurrentDate();
	//IDataHandler<GasStationData>* _handler;
    std::unique_ptr<IDataHandler<GasStationData>> _handler;
};