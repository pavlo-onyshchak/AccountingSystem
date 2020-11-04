#pragma once
#include <string>
#include <memory>
#include "IDataHandler.h"

class GasStation
{
public:
    struct Data
    {
        int ID;
        std::string Location;
        int Ballance;
        static int GetID(std::string line);
        static std::string GetCSVFormat(const Data& userData);
        static Data GetData(const std::string& line);
    };

    struct UserInputData
    {
        std::string Location;
        int Ballance;
    };

	GasStation(std::unique_ptr<IDataHandler<Data>>  handler);
	void Add(UserInputData userData);
    int Delete(const int& id);
    int Update(const int& id, const UserInputData& userData);
    Data Get(const int& id);

private:
    int GenerateNextId();
    int GetBallance();
    Data InitData(const int& id, const UserInputData& userData);
    std::unique_ptr<IDataHandler<Data>> _handler;
};