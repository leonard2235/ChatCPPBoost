
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class Message {
	std::string userName;
	std::string messageLine; 

	friend class boost::serialization::access;
	template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {
        /* & represents << or >> */
		ar & messageLine;
        ar & userName;
    }
public:
	Message() {}
	Message(const std::string & user, const std::string & message) 
		: userName(user), messageLine(user + ": " + message) {}
	std::string const & getName() const { return userName; }
	std::string const & getMessage() const { return messageLine; }
};

#endif /* _MESSAGE_H_ */
