#include "AMQPcpp.h"

#include <iostream>
#include <string>
#include <mutex>

class consumer

{
	AMQPQueue *m_qu;
	AMQP *m_amqp;

	static std::mutex mtx;
	std::string m_username, m_password, m_host,
	m_port, m_queueName, m_connString, m_consumerTag;

	void createConnString();

	static int onMessage(AMQPMessage *message);

	static std::vector<std::string> getConsumeRequestMessage();

	public:

	static std::vector<std::string> m_consumeRequestMsg;

	consumer(std::string username, std::string password, std::string host, std::string port, std::string queueName, std::string consumerTag);

	std::string init();

	std::string bindToExchange(const std::string& exchangeName, const std::string& topicName);

	std::string unbindToExchange(const std::string& exchangeName, const std::string& topicName);

	std::string consumeMessage();

	std::string deleteQueue();

	std::string purgeQueue();

	std::vector<std::string> getConsumedMessage();

	~consumer();
};
