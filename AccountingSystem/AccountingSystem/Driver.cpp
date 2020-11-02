#include <sstream>
#include "Driver.h"

Driver::Driver(std::unique_ptr<IDataHandler<Data>> handler) : _handler(std::move(handler)) 
{
}

void Driver::Add(UserInputData userData)
{
    auto data = InitData(GenerateNextId(), userData);
    _handler->Add(data);
}

int Driver::Delete(const int& id)
{
    return _handler->Delete(id);
}

int Driver::Update(const int& id, const UserInputData& userData)
{
    auto data = InitData(id, userData);
    return _handler->Update(id,data);
}

Driver::Data Driver::Get(int id)
{
    return _handler->Get(id);
}

int Driver::GenerateNextId()
{
    return _handler->TableSize() <= 1 ? 1 : _handler->GetLastRecord().ID + 1;
}

Driver::Data Driver::InitData(const int& id, const UserInputData& userData)
{
    Data data;
    data.ID = id;
    data.Name = userData.Name;
    data.Surname = userData.Surname;
    return data;
}

int Driver::Data::GetID(std::string line)
{
    std::stringstream ss(line);
     std::string id;
     std::getline(ss, id, ',');
     return stoi(id);
}

std::string Driver::Data::GetCSVFormat(Data data)
{
    std::stringstream ss;
    const auto delim = ',';
    ss << data.ID << delim
        << data.Name << delim
        << data.Surname;
    return ss.str();
}

Driver::Data Driver::Data::GetData(std::string line)
{
    Data data;
    std::stringstream ss(line);
    std::string tmp;
    std::getline(ss, tmp, ',');
    data.ID = stoi(tmp);
    std::getline(ss, data.Name, ',');
    std::getline(ss, data.Surname);
    return data;
}
