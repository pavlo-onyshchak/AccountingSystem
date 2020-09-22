#include "Fuel.h"
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <time.h>
#include <experimental/filesystem>

Fuel::Fuel(std::string pathToFile) : _pathToFile(pathToFile),_ballance(0)
{
	if (!std::experimental::filesystem::exists(_pathToFile))
	{
		CreateFile();
	}
}

void Fuel::FuelTheTruck(const InputData& data)
{
	UpdateFuelBallance(data.liters, false);
	std::ofstream fout;
	fout.open(_pathToFile, std::ios_base::app);
	const auto delim = ',';
	auto id = GetLastID() + 1;
	fout << std::to_string(id) << delim
		<<  data.name << delim
		<< data.surname << delim
		<< GetCurrentDate() << delim
		<< data.liters << delim
		<< GetCurrentBallance() - data.liters << "\n"; 
	fout.close();
}


int Fuel::GetCurrentBallance()
{
	if (FileIsEmpty())
	{
		return 0;
	}
	std::ifstream file(_pathToFile);
	std::string lastLine;
	while (file >> std::ws && std::getline(file, lastLine));
	std::string ballance = lastLine.substr(lastLine.find_last_of(',') + 1,lastLine.size());
	return std::stoi(ballance);
}

size_t Fuel::GetLastID()
{
	if (FileIsEmpty())
	{
		return 0;
	}
	std::ifstream file(_pathToFile);
	std::string lastLine;
	while (file >> std::ws && std::getline(file, lastLine));
	std::string id = lastLine.substr(0, lastLine.find(','));
	return std::stoi(id);
}

bool Fuel::FileIsEmpty()
{
	std::ifstream file(_pathToFile);
	std::string line;
	auto count = 0;
	while (std::getline(file, line))
	{
		count++;
	}

    return count <= 1;
}

void Fuel::CreateFile()
{
	std::ofstream myFile(_pathToFile);
	const auto delim = ' ';
	myFile << "ID" << delim
		<< "Name" << delim
		<< "Surname" << delim
		<< " Date" << delim
		<< " Liters" << delim
		<< "Ballance" << delim
		<< "\n";
	myFile.close();
}

std::string Fuel::GetCurrentDate()
{
	auto time = std::time(nullptr);
	auto currentTime = *localtime(&time);

	std::ostringstream formatedDate;
	formatedDate << std::put_time(&currentTime, "%d-%m-%Y");
	return formatedDate.str();
}
