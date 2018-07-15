
#include <iostream>

#include "client.h"
#include "chatException.h"

int main() {
	std::cout << "Please, enter your name" << std::endl;
	std::string s;
	std::getline(std::cin, s);
	std::cout << "Please, enter your password" << std::endl;
	std::string p;
	std::getline(std::cin, p);
	try {
		Client client("127.0.0.1", 1234, s, p);
		client.join();
	} catch (std::exception &e) {
		std::cout << e.what();
	}
	return 0;
} /* End of 'main' function */
