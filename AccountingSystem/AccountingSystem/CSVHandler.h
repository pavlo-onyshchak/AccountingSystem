#pragma once
#include <string>
#include <functional>
#include <vector>
#include <cstdio>
#include <experimental/filesystem>
#include "IDataHandler.h"

template <class T>
class CSVHandler : public IDataHandler<T>
{
public:
	 CSVHandler(std::string pathToFile, std::vector<std::string> columnsName, std::function<std::string(T)> convertToInputFormat,
	            std::function<T(std::string)> convertToOutputFormat);
	 int Add(const T data) override;
	 int Delete(const int id) override;
	 int Update(const int id, const T value) override;
	 T Get(const int id) override;
     T GetLastRecord() override;
     int TableSize() override;
private:
	void CreateFile(const std::string pathToFile);
    void CreateTmpFile(const std::string pathToFile, std::vector<std::string> rows);
    std::string searchRow(int id);
	std::function<std::string(T)> _toInputFormat;
	std::function<T(std::string)> _toOutputFormat;
	std::vector<std::string> _columnsName;
	std::string _pathToFile;
};

template<class T>
CSVHandler<T>::CSVHandler(std::string pathToFile, std::vector<std::string> columnsName, std::function<std::string(T)> convertToInputFormat,
    std::function<T(std::string)> convertToOutputFormat)
    : _pathToFile(pathToFile), _columnsName(columnsName), _toInputFormat(convertToInputFormat),
    _toOutputFormat(convertToOutputFormat) {}

template<class T>
inline int CSVHandler<T>::Add(const T data)
{
     if (!std::experimental::filesystem::exists(_pathToFile))
     {
          CreateFile(_pathToFile);
     }
     std::ofstream fout;
     fout.open(_pathToFile, std::ios_base::app);
     std::string result = _toInputFormat(data);
     fout << result << "\n";
     fout.close();
     return 0;
}

template<class T>
inline int CSVHandler<T>::Delete(const int id)
{
     if (!std::experimental::filesystem::exists(_pathToFile))
     {
          return 0;
     }
     std::ifstream file(_pathToFile);
     std::string line;
     std::vector<std::string> rows;
     std::getline(file, line); // to skip header line
     while (std::getline(file, line))
     {
         if (id != T::GetID(line))
         {
             rows.push_back(line);
         }
     }
     file.close();
     const std::string path = "../GasStationTmp.txt";
     CreateTmpFile(path, rows);
     remove(_pathToFile.c_str());
     rename(path.c_str(), _pathToFile.c_str());
     return id;
}

template<class T>
inline int CSVHandler<T>::Update(const int id, const T val)
{
    /*if (isPresnt(id))
    {
         auto row = _convertFrom(val);
         insert(id, row);
    }*/
    return 0;
}

template<class T>
inline T CSVHandler<T>::Get(const int id)
{
    if (!std::experimental::filesystem::exists(_pathToFile))
    {
        throw "File doesnt exist";
    }
    else if (TableSize() <= 1)
    {
        throw "File is empty!";
    }
    auto row = searchRow(id);
    return _toOutputFormat(row);
}

template<class T>
inline void CSVHandler<T>::CreateFile(const std::string pathToFile)
{
    std::ofstream myFile(pathToFile);
    const auto delim = ' ';
    for (auto i = 0; i < _columnsName.size(); ++i)
    {
    	myFile << _columnsName[i] << delim;
    }
    myFile << "\n";
}

template<class T>
inline void CSVHandler<T>::CreateTmpFile(const std::string pathToFile, std::vector<std::string> rows)
{
    if (!std::experimental::filesystem::exists(pathToFile))
    {
        CreateFile(pathToFile);
    }
    std::ofstream fout;
    fout.open(pathToFile, std::ios_base::app);
    for (auto line : rows)
    {
        fout << line << "\n";
    }
    fout.close();
}

template<class T>
inline std::string CSVHandler<T>::searchRow(int id)
{
    std::ifstream file(_pathToFile);
    std::string line;
    /*skip header file*/
    std::getline(file, line); 
    while (std::getline(file, line))
    {
        if (id == T::GetID(line))
        {
            break;
        }
    }
    file.close();
    return line;
}

template<class T>
inline T CSVHandler<T>::GetLastRecord()
{
    if(TableSize() <= 1)
    {
        throw "File is empty!";
    }
    std::ifstream file(_pathToFile);
    std::string lastLine;
    while (file >> std::ws && std::getline(file, lastLine));
    return _toOutputFormat(lastLine);
}

template<class T>
inline int CSVHandler<T>::TableSize()
{
    std::ifstream file(_pathToFile);
    std::string line;
    auto count = 0;
    while (std::getline(file, line))
    {
        count++;
    }
    file.close();
    return count;
}

