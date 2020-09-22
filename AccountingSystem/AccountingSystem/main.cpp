#include <iostream>
#include "cpp-httplib/httplib.h"
#include "Fuel.h"



int main(void) {
    
	httplib::Server svr;
	Fuel fuel("../Fuel.txt");
	Fuel::InputData data{ "Pavlo","CC",123 };
	fuel.FuelTheTruck(data);
	std::cout << fuel.GetCurrentBallance() << std::endl;
	/*svr.Get("/hi", [](const auto& req, auto& res) {
		res.set_content("Hello World!", "text/plain");
	});

	std::cout << "start server..." << std::endl;
	svr.listen("localhost", 8080);*/
}