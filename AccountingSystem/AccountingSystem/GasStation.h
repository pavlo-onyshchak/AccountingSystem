#pragma once
#include <string>
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
    };

    struct InputData
    {
        std::string Name;
        std::string Surname;
        int Liters;
    };

	GasStation(IDataHandler<GasStationData>* _handler);
	void AddRecord(InputData data, const bool IncreaseTheBallance = false);
    int DeleteRecord(const int id);
    GasStationData GetRecord(int id);
	static std::string GetCSVFormat(GasStationData userData);
	static GasStationData GetGasStationData(std::string line);

private:
    int GenerateNextId();
    int GetCurrentBallance();
    std::string GetCurrentDate();
	IDataHandler<GasStationData>* _handler;
};