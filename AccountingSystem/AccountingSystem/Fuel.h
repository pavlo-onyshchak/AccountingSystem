#pragma once
#include <string>

class Fuel
{
public:

	struct Data
	{
		std::string name;
		std::string surname;
		size_t litters;
		size_t ballance;
	};

	struct InputData
	{
		std::string name;
		std::string surname;
		size_t liters;
	};

    Fuel(std::string pathToFile);
	void FuelTheTruck(const InputData& data);
	int GetCurrentBallance();
	size_t GetLastID();

private:
	void CreateFile();
	std::string GetCurrentDate();
	bool FileIsEmpty();
	std::string _pathToFile; 
};
