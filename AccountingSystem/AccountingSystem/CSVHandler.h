#pragma once
#include <string>
#include <functional>
#include <vector>
#include <cstdio>
#include <experimental/filesystem>
#include <memory>
#include <fstream>
#include "IDataHandler.h"

template <class T>
class CSVHandler : public IDataHandler<T>
{
public:
     static std::unique_ptr<CSVHandler<T>> CreateHandler(std::string pathToFile, std::vector<std::string> columnsName,
         std::function<std::string(T)> convertToInputFormat, std::function<T(std::string)> convertToOutputFormat);
	 int Add(const T data) override;
	 int Delete(const int id) override;
	 int Update(const int id, const T& value) override;
	 T Get(const int id) override;
     T GetLastRecord() override;
     int TableSize() override;
private:
    CSVHandler(std::string pathToFile, std::vector<std::string> columnsName, std::function<std::string(T)> convertToInputFormat,
        std::function<T(std::string)> convertToOutputFormat);
    void CreateTmpFile(const std::string pathToFile, std::vector<std::string> rows);
    std::string GetRow(int id);
    void InitFile();
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
inline std::unique_ptr<CSVHandler<T>> CSVHandler<T>::CreateHandler(std::string pathToFile, std::vector<std::string> columnsName,
                       std::function<std::string(T)> convertToInputFormat, std::function<T(std::string)> convertToOutputFormat)
{
    std::unique_ptr<CSVHandler<T>> handler (new CSVHandler(pathToFile, columnsName, convertToInputFormat, convertToOutputFormat));
    if (!std::experimental::filesystem::exists(pathToFile))
    {
        handler->InitFile();
    }
    return handler;
}

template<class T>
inline int CSVHandler<T>::Add(const T data)
{
     std::ofstream file;
     file.open(_pathToFile, std::ios_base::app);
     std::string result = _toInputFormat(data);
     file << result << "\n";
     return 0;
}

template<class T>
inline int CSVHandler<T>::Delete(const int id)
{
     if (!TableSize())
     {
         throw std::exception("File is empty");
     }
     std::ifstream file(_pathToFile);
     const auto pathToTmpFile = std::string("../GasStationTmp.txt");
     std::ofstream tmpFile(pathToTmpFile, std::ios_base::app);
     std::string line;
     /*Copy header file*/
     std::getline(file, line); 
     tmpFile << line << "\n";
     while (std::getline(file, line))
     {
         if (id != T::GetID(line))
         {
             tmpFile << line << "\n";
         }
     }
     file.close();
     remove(_pathToFile.c_str());
     tmpFile.close();
     rename(pathToTmpFile.c_str(), _pathToFile.c_str());
     return id;
}

template<class T>
inline int CSVHandler<T>::Update(const int id, const T& val)
{
    if (!TableSize())
    {
        throw std::exception("File is empty");
    }
    std::ifstream file(_pathToFile);
    const auto pathToTmpFile = std::string("../Tmp.txt");
    std::ofstream tmpFile(pathToTmpFile, std::ios_base::app);
    std::string line;
    /*To skeep header line*/
    std::getline(file, line);
    tmpFile << line << "\n";
    while (std::getline(file, line))
    {
        if (id == T::GetID(line))
        {
            line = _toInputFormat(val);
        }
        tmpFile << line << "\n";
    }
    file.close();
    remove(_pathToFile.c_str());
    tmpFile.close();
    rename(pathToTmpFile.c_str(), _pathToFile.c_str());
    return id;
}

template<class T>
inline T CSVHandler<T>::Get(const int id)
{
    if (!TableSize())
    {
        throw std::exception("File is empty");
    }
    auto row = GetRow(id);
    return _toOutputFormat(row);
}


template<class T>
inline void CSVHandler<T>::CreateTmpFile(const std::string pathToFile, std::vector<std::string> rows)
{
    std::ofstream fout;
    fout.open(pathToFile, std::ios_base::app);
    for (auto line : rows)
    {
        fout << line << "\n";
    }
}

template<class T>
inline std::string CSVHandler<T>::GetRow(int id)
{
    std::ifstream file(_pathToFile);
    std::string line;
    /*skip header file*/
    std::getline(file, line); 
    while (std::getline(file, line))
    {
        if (id == T::GetID(line))
        {
            return line;
        }
    }
    throw std::exception("Record with current ID doesnt exist");
}

template<class T>
inline void CSVHandler<T>::InitFile()
{
    std::ofstream file(_pathToFile);
    const auto delim = ' ';
    for (const auto& name : _columnsName)
    {
        file << name << delim;
    }
    file << "\n";
}

template<class T>
inline T CSVHandler<T>::GetLastRecord()
{
    if(!TableSize())
    {
        throw "File is empty!";
    }
    std::ifstream file(_pathToFile);
    std::string lastLine;
    while (file >> std::ws && std::getline(file, lastLine))
    {
    }
    return _toOutputFormat(lastLine);
}

template<class T>
inline int CSVHandler<T>::TableSize()
{
    std::ifstream file(_pathToFile);
    std::string line;
    /*skip header line*/
    std::getline(file, line);
    auto counter = 0;
    while (std::getline(file, line))
    {
        ++counter;
    }
    file.close();
    return counter;
}
