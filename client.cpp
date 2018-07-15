
#define _WIN32_WINNT 0x0501

#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "client.h"
#include "message.h"
#include "chatException.h"

using namespace boost::asio;

void Client::sendToServer() {
	try {
		boost::asio::streambuf buf;
		std::ostream os(&buf);
		std::vector<boost::asio::const_buffer> buffers(2);
		while (true) {
			std::string s;
			std::getline(std::cin, s); 
			boost::archive::binary_oarchive archive(os);
			Message msg(userName, s + "\n");				
			archive << msg; //serialize message
			const size_t header = buf.size(); //size of serialized message
			/* Size of serialized object */				
			buffers[0] = boost::asio::buffer(&header, sizeof(header));
			/* Serialized object */
			buffers[1] = boost::asio::buffer(buf.data(), header);
			boost::asio::write(socket, buffers);
			buf.consume(header);  //clear input sequence
		}
	} catch (boost::system::system_error) { //connection was closed
	}
}

void Client::getFromServer() {
	try {
		boost::asio::streambuf buf;
		std::istream in(&buf);
		/* Read header */
		size_t header;
		while (read(socket, boost::asio::buffer(&header, sizeof(header)))) { 	// read data from server
			/* Read body */
			read(socket, buf.prepare(header));
			buf.commit(header);  //prepare streambuf for input
			/* Deserialization */
			boost::archive::binary_iarchive archive(in);
			Message msg;
			archive >> msg;
			std::cout << msg.getMessage(); 
		}
	} catch (boost::system::system_error) { //connection was closed
	}
}

Client::Client(const std::string & address, unsigned short port, 
			const std::string & userName, const std::string & password) 
		: socket(service), userName(userName) 
{
	/* Connects to the server */
	ip::tcp::endpoint ep(ip::address::from_string(address), port);
	socket.connect(ep);
	/* Try to connect to the server */
	boost::asio::write(socket, buffer(userName + "\n" + password + "\n"));
	boost::asio::streambuf buf;
	std::istream in(&buf);
	boost::asio::read_until(socket, buf, "\n");
	std::string answer;
	in >> answer;
	if (answer == "IncorrectNamePassword")
		throw IncorrectNamePassword();
	if (answer == "TooMuchUsers")
		throw TooMuchUsers();
	/* Everything is ok, can start work */
	toServer = boost::thread(&Client::sendToServer, this);
	fromServer = boost::thread(&Client::getFromServer, this);
}

void Client::join() {
	toServer.join();
	fromServer.join();
}

