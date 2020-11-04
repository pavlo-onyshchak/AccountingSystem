#include <sstream>
#include <iomanip>
#include <ctime>
#include "FuelReport.h"

FuelReport::FuelReport(std::unique_ptr<IDataHandler<Data>> handler) : _handler(std::move(handler))
{
}

void FuelReport::Add(UserInputData userData)
{
    auto data = InitData(GenerateNextID(),userData);
    _handler->Add(data);
}

int FuelReport::Delete(const int& id)
{
    return _handler->Delete(id);
}

int FuelReport::Update(const int& id, const UserInputData& userData)
{
    auto data = InitData(id,userData);
    return _handler->Update(id,data);
}

FuelReport:: Data FuelReport::Get(const int& id)
{
    return _handler->Get(id);
}

int FuelReport::GenerateNextID()
{
    return _handler->TableSize() <= 1 ? 1 : _handler->GetLastRecord().ID + 1;
}

FuelReport::Data FuelReport::InitData(const int& id, const UserInputData& userData)
{
    Data data;
    data.ID = id;
    data.DriverID = userData.DriverID;
    data.Liters = userData.Liters;
    data.DriverID = userData.Liters;
    data.Date = userData.Date;
    return data;
    return Data();
}

std::string FuelReport::GetCurrentDate()
{
    auto time = std::time(nullptr);
    auto currentTime = *localtime(&time);

    std::ostringstream formatedDate;
    formatedDate << std::put_time(&currentTime, "%d-%m-%Y");
    return formatedDate.str();
}


std::string FuelReport::Data::GetCSVFormat(const Data& data)
{
    std::stringstream ss;
    const auto delim = ',';
    ss << data.ID << delim
        << data.DriverID << delim
        << data.Liters << delim
        << data.Date << delim
        << data.GasStationID;
    return ss.str();
}

FuelReport::Data FuelReport::Data::GetData(const std::string& line)
{
    Data data;
    std::stringstream ss(line);
    std::string tmp;
    std::getline(ss, tmp, ',');
    data.ID = stoi(tmp);
    std::getline(ss, tmp, ',');
    data.DriverID = stoi(tmp);
    std::getline(ss, tmp, ',');
    data.Liters = stoi(tmp);
    std::getline(ss, data.Date, ',');
    std::getline(ss, tmp);
    data.GasStationID = stoi(tmp);
    return data;
}
