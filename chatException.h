#ifndef _CHAT_EXCEPTION_
#define _CHAT_EXCEPTION_

#include <exception>

class IncorrectNamePassword : public std::exception {
public:
	const char* what() const {return "Wrong user name or password\n";}
};

class TooMuchUsers : public std::exception {
public:
	const char* what() const {return "Too much user are connected to the server\n";}
};

#endif /* _CHAT_EXCEPTION_ */

