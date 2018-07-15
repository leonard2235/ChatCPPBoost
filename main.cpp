#include <ostream>
#include <exception>
#include <boost/property_tree/exceptions.hpp>

#include "server.h"

int main() {
	try {
		Server s("127.0.0.1", 1234);
		s.run();
	} catch (boost::property_tree::ptree_error &e) {
		std::cout << e.what();
	} catch (std::exception &e) {
		std::cout << e.what();
	}
	return 0;
} /* End of 'main' function */
