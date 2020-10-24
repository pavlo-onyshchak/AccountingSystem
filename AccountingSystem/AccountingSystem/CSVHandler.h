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
     static CSVHandler* GetHandler(std::string pathToFile, std::vector<std::string> columnsName,
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
inline CSVHandler<T>* CSVHandler<T>::GetHandler(std::string pathToFile, std::vector<std::string> columnsName, std::function<std::string(T)> convertToInputFormat, std::function<T(std::string)> convertToOutputFormat)
{
    std::ofstream myFile(pathToFile);
    const auto delim = ' ';
    for (auto i = 0; i < columnsName.size(); ++i)
    {
        myFile << columnsName[i] << delim;
    }
    myFile << "\n";
    return new CSVHandler(pathToFile,columnsName,convertToInputFormat,convertToOutputFormat);
}

template<class T>
inline int CSVHandler<T>::Add(const T data)
{
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
inline int CSVHandler<T>::Update(const int id, const T& val)
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
    else if (TableSize() <= 1)
    {
        throw std::exception("File is empty");
    }
    auto row = searchRow(id);
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
    if (line.empty())
    {
        throw std::exception("Record with current ID doesnt exist");
    }
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
    auto count = 0;
    while (std::getline(file, line))
    {
        ++count;
    }
    file.close();
    return count;
}

