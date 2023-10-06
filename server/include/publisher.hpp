#include "AMQPcpp.h"

#include <iostream>
#include <string>
#include <map>

class publisher
{
	AMQPExchange *m_ex;
	AMQP *m_amqp;
	std::string m_username, m_password, m_host, m_port, m_exchangeName, m_connString;
	std::map<std::string, std::string> m_headers;

	void createConnString();

	public:

	publisher(std::string username, std::string password, std::string host, std::string port, std::string exchangeName);

	void addHeaders(std::map<std::string, std::string> headers);

	std::map<std::string, std::string> getHeaders();

	std::string init();

	std::string createAndBindQueue(const std::string& queueName, const std::string& topicName);

	std::string publishMessage(const std::string& message, const std::string& topicName);
	//std::string publisher::publishMessageserlized(const std::string& message, const std::string& topicName);
	std::string deleteExchange();

	~publisher();
};
;
