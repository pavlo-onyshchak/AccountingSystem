#pragma once
#include <string>
#include <memory>
#include "IDataHandler.h"

class Driver
{
public:
    struct Data
    {
        int ID;
        std::string Name;
        std::string Surname;
        static int GetID(std::string line);
        static std::string GetCSVFormat(const Data& data);
        static Data GetData(const std::string& line);
    };

    struct UserInputData
    {
        std::string Name;
        std::string Surname;
    };

    Driver(std::unique_ptr<IDataHandler<Data>>  handler);
    void Add(UserInputData userData);
    int Delete(const int& id);
    int Update(const int& id, const UserInputData& userData);
    Data Get(int id);

private:
    int GenerateNextID();
    Data InitData(const int& id, const UserInputData& userData);
    std::unique_ptr<IDataHandler<Data>> _handler;
};
