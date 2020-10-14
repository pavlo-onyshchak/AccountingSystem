#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include "GasStation.h"

GasStation::GasStation(IDataHandler<GasStationData>* handler):_handler(handler)
{
}

void GasStation::AddRecord(InputData userData,const bool IncreaseTheBallance)
{
    GasStationData data;
    data.Name = userData.Name;
    data.Surname = userData.Surname;
    data.Liters = userData.Liters;
    data.ID = GenerateNextId();
    data.Date = GetCurrentDate();
    auto ballance = GetCurrentBallance();
    data.Ballance = IncreaseTheBallance ? ballance + data.Liters : ballance - data.Liters;
	_handler->Add(data);
}

int GasStation::DeleteRecord(const int id)
{
    return _handler->Delete(id);
}


GasStation::GasStationData GasStation::GetRecord(int id)
{
    return _handler->Get(id);
}

std::string GasStation::GasStationData::GetCSVFormat(GasStationData data)
{
	std::stringstream ss;
	const auto delim = ',';
	ss << data.ID << delim
		<< data.Name << delim
		<< data.Surname << delim
		<< data.Date << delim
		<< data.Liters << delim
		<< data.Ballance << delim;
	return ss.str();
}

GasStation::GasStationData GasStation::GasStationData::GetGasStationData(std::string line)
{
	GasStationData data;
    std::stringstream ss(line);
    std::string tmp;
	std::getline(ss, tmp, ',');
	data.ID = stoi(tmp);
	std::getline(ss, data.Name, ',');
	std::getline(ss, data.Surname, ',');
	std::getline(ss, tmp, ',');
	data.Liters = stoi(tmp);
	std::getline(ss, data.Date, ',');
	std::getline(ss, tmp, ',');
	data.Ballance = stoi(tmp);
	return data;
}

int GasStation::GenerateNextId()
{
    return _handler->TableSize() <= 1 ? 1 : _handler->GetLastRecord().ID + 1;
}

int GasStation::GetCurrentBallance()
{
    return  _handler->TableSize() <=1 ? 0 : _handler->GetLastRecord().Ballance;
}

int GasStation::GasStationData::GetID(std::string line)
{
     std::stringstream ss(line);
     std::string id;
     std::getline(ss, id, ',');
     return stoi(id);
}

std::string GasStation::GetCurrentDate()
{
     auto time = std::time(nullptr);
     auto currentTime = *localtime(&time);
     
     std::ostringstream formatedDate;
     formatedDate << std::put_time(&currentTime, "%d-%m-%Y");
     return formatedDate.str();
}
