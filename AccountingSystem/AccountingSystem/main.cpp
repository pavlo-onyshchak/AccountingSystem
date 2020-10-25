#include <iostream>
#include "cpp-httplib/httplib.h"
#include "GasStation.h"
#include "IDataHandler.h"
#include "CSVHandler.h"

#include <vector>
#include<sstream>



int main(void) 
{
	std::vector<std::string> vec{ "ID","Name","Surname","Date","Liters","Ballance" };
	std::function<std::string(GasStation::GasStationData)> func = GasStation::GasStationData::GetCSVFormat;
	std::function<GasStation::GasStationData(std::string)> func1 = GasStation::GasStationData::GetGasStationData;
    auto handler = CSVHandler<GasStation::GasStationData>::CreateHandler("../GasStation.txt", vec, func, func1);
	auto obj = GasStation(std::move(handler));
	//obj.AddRecord({"pavlo","ivansdasd",100});
   //obj.DeleteRecord(1);
   // obj.UpdateRecord(4, { "petro","ivanov",100 });
    //auto k = obj->GetRecord(3);
    //std::cout << GasStation::GasStationData::GetCSVFormat(k) << std::endl;

    //obj->DeleteRecord(5);

	/*httplib::Server svr;
	std::vector<std::string> vec{ "ID","¿≈Ã
	Fuel::InputData data{ "Pavlo","CC",123 };
	fuel.FuelTheTruck(data);
	*/
	//std::cout << fuel.GetCurrentBallance() << std::endl;
	/*svr.Get("/hi", [](const auto& req, auto& res) {
		res.set_content("Hello World!", "text/plain");
	});

	std::cout << "start server..." << std::endl;
	svr.listen("localhost", 8080);*/
}