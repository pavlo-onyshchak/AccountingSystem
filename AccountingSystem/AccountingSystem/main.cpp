#include <iostream>
#include "cpp-httplib/httplib.h"

int main(void) {
	httplib::Server svr;

	svr.Get("/hi", [](const auto&, auto& res) {
		res.set_content("Hello World!", "text/plain");
	});

	std::cout << "start server..." << std::endl;
	svr.listen("localhost", 8080);
}