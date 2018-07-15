
#ifndef _CLIENT_H_
#define _CLIENT_H_

#define _WIN32_WINNT 0x0501

#include <boost/asio.hpp>
#include <boost/thread.hpp>

class Client {
	boost::asio::io_service service; 
	boost::asio::ip::tcp::socket socket;
	boost::thread toServer, fromServer;
	std::string userName;

	void sendToServer();
	void getFromServer();
public:
	Client(const std::string & address, unsigned short port, 
				const std::string & userName, const std::string & password);

	void join();
};

#endif /* _CLIENT_H_ */
