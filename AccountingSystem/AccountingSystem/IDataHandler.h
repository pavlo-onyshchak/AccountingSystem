#pragma once
template <class T>
class IDataHandler
{
public:
     virtual int Add(const T data) = 0;
     virtual int Delete(const int id) = 0;
     virtual int Update(const int id, const T& value) = 0;
     virtual T Get(const int id) = 0;
     virtual T GetLastRecord() = 0;
     virtual int TableSize() = 0;
};