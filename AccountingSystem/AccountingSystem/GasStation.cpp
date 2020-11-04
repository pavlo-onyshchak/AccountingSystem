#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include "GasStation.h"

GasStation::GasStation(std::unique_ptr<IDataHandler<Data>> handler) : _handler(std::move(handler))
{
}

void GasStation::Add(UserInputData userData)
{
    auto data = InitData(GenerateNextId(), userData);
    _handler->Add(data);
}

int GasStation::Delete(const int& id)
{
    return _handler->Delete(id);
}

int GasStation::Update(const int& id,const UserInputData& userData)
{
    auto data = InitData(GenerateNextId(), userData);
    return _handler->Update(id,data);
}


GasStation::Data GasStation::Get(const int& id)
{
    return _handler->Get(id);
}

std::string GasStation::Data::GetCSVFormat(const Data& data)
{
	std::stringstream ss;
	const auto delim = ',';
    ss << data.ID << delim
        << data.Location << delim
        << data.Ballance;
	return ss.str();
}

GasStation::Data GasStation::Data::GetData(const std::string& line)
{
	Data data;
    std::stringstream ss(line);
    std::string tmp;
	std::getline(ss, tmp, ',');
	data.ID = stoi(tmp);
	std::getline(ss, data.Location, ',');
	std::getline(ss, tmp, ',');
    data.Ballance = stoi(tmp);
	return data;
}

int GasStation::GenerateNextId()
{
    return _handler->TableSize() <= 1 ? 1 : _handler->GetLastRecord().ID + 1;
}

int GasStation::GetBallance()
{
    return  _handler->TableSize() <=1 ? 0 : _handler->GetLastRecord().Ballance;
}

GasStation::Data GasStation::InitData(const int& id, const UserInputData& userData)
{
    Data data;
    data.ID = id;
    data.Location = userData.Location;
    data.Ballance = userData.Ballance;
    return data;
}

int GasStation::Data::GetID(std::string line)
{
     std::stringstream ss(line);
     std::string id;
     std::getline(ss, id, ',');
     return stoi(id);
}
