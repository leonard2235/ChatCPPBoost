
#ifndef _SERVER_H_
#define _SERVER_H_
#define _WIN32_WINNT 0x0501

#include <list>
#include <deque>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include "FixSizeQueue.h"

#define BOOST_ASIO_SEPARATE_COMPILATION

class Server {
	template<class Type> class FixSizeLockQueue {
		FixSizeQueue<Type> data;
		boost::mutex queueMutex;
		int size;
	public:
		/* Work with const_iterator */
		typedef typename FixSizeQueue<Type>::const_iterator const_iterator;
		const_iterator cbegin() const { return data.cbegin(); }
		const_iterator cend() const { return data.cend(); }
		/* Work with mutex */
		void lock() { queueMutex.lock(); }
		void unlock() { queueMutex.unlock(); }
		/* Consturctors */
		FixSizeLockQueue(const int size = 0) : size(size) {}
		void setSize(int const size) { 
			this->size = size; 
			data = FixSizeQueue<Type>(size);
		}
		/* Add new object in queue */
		void put(Type const & s) {
			lock();
				data.put(s);
			unlock();
		} /* End of 'Server::FixSizeLockQueue::put' function */

	};

	boost::asio::io_service service; 
	boost::asio::ip::tcp::endpoint ep;
	boost::asio::ip::tcp::acceptor acc; //object for client's connection
	/* Store last messages to show it to new clients */
	FixSizeLockQueue<std::string> lastMessages;
	size_t numberOfLastMessages;

	/* List of all current server's clients */
	std::list<boost::shared_ptr<boost::asio::ip::tcp::socket> > clients;
	size_t maxNumberOfClients;

	/* Stores user's name as key with its password */
	std::map<std::string, std::string> clientsInfo;

	/* Mutex to work with list of clients */
	boost::mutex clientsMutex;

	/* Read data from client and send it to others client */
	void readWrite(boost::shared_ptr<boost::asio::ip::tcp::socket> & sockPtr);
public:
	Server(const std::string &address, unsigned short port);
	
	/* Connect new client and start a thread for it */
	void run();
};

#endif /* _SERVER_H_ */
