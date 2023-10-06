#include "consumer.hpp"

std::vector<std::string> consumer::m_consumeRequestMsg;
std::mutex consumer::mtx;

void consumer::createConnString()
{
	if((m_username.size() != 0) || (m_password.size() != 0))
	{
		if((m_username.size() != 0) && (m_password.size() != 0))
		{
			m_connString = m_username + ":" + m_password + "@";
		}
		else if(m_username.size() == 0)
		{
			m_connString = ":" + m_password + "@";
		}
		else
		{
			m_connString = m_username + "@";
		}
	}

	m_connString += m_host + ":" + m_port;
}

int consumer::onMessage(AMQPMessage* message)
{
	uint32_t j;
	auto data = message->getMessage(&j);

	mtx.lock();

	consumer::m_consumeRequestMsg.push_back(data);

	mtx.unlock();

	return 0;
}

consumer::consumer(std::string username, std::string password, std::string host, std::string port, std::string queueName, std::string consumerTag):m_amqp(NULL)
{
	m_username = username;
	m_password = password;
	m_host = ((host.size() == 0) ? "localhost" : host);//amqps://b-ed4a3cfc-6986-4ec2-b597-e33481343ffb.mq.eu-north-1.amazonaws.com:5671
	//m_host = ((host.size() == 0) ? "amqps://b-ed4a3cfc-6986-4ec2-b597-e33481343ffb.mq.eu-north-1.amazonaws.com" : host);
	m_port = ((port.size() == 0) ? "5672" : port);
	//m_port = ((port.size() == 0) ? "5671" : port);
	m_queueName = queueName;
	m_consumerTag = consumerTag;

	createConnString();
}

std::string consumer::init()
{
	try
	{
		if(m_queueName.size() ==0)
		{
			throw std::string("Queue is empty");
		}

		m_amqp = new AMQP(m_connString);

		if(m_amqp == NULL)
		{
			throw std::string("Error while creating connection");
		}
		

		m_qu = m_amqp->createQueue(m_queueName);

		if(m_qu == NULL)
		{
			throw std::string("createQueue() Failed");
		}

		m_qu->Declare();
		m_qu->setConsumerTag(m_consumerTag);
		m_qu->addEvent(AMQP_MESSAGE, onMessage);
	}
	catch(const std::string& str)
	{
		return str;
	}
	catch(AMQPException& e)
	{
		return e.getMessage();
	}

	return std::string();
}

std::string consumer::bindToExchange(const std::string& exchangeName, const std::string& topicName)
{
	try
	{
		if(topicName.size() == 0)
		{
			throw std::string("Topic Name is empty");
		}
		if(exchangeName.size() == 0)
		{
			throw std::string("Exchange Name is empty");
		}
		AMQPExchange *m_ex= m_amqp->createExchange(exchangeName);
		m_ex->Declare(exchangeName, "topic");
		m_qu->Bind(exchangeName, topicName);
	}
	catch(const std::string& str)
	{
		return str;
	}
	catch (AMQPException& e)
	{
		return e.getMessage();
	}

	return std::string();
}

std::string consumer::unbindToExchange(const std::string& exchangeName, const std::string& topicName)
{
	try
	{
		if(exchangeName.size() == 0)
		{
			throw std::string("Exchange Name is empty");
		}
		if(topicName.size() == 0)
		{
			throw std::string("Topic Name is empty");
		}

		m_qu->unBind(exchangeName, topicName);
	}
	catch(const std::string& str)
	{
		return str;
	}
    catch (AMQPException& e)
	{
		return e.getMessage();
	}

    return std::string();
}

std::string consumer::consumeMessage()
{
	try
	{
		m_qu->Consume(AMQP_NOACK);
	}
	catch(AMQPException& e)
	{
		return e.getMessage();
	}

	return std::string();
}

std::string consumer::deleteQueue()
{
	try
	{
		m_qu->Delete();
	}
	catch(AMQPException& e)
	{
		return e.getMessage();
	}

	return std::string();
}

std::string consumer::purgeQueue()
{
	try
	{
		m_qu->Purge();
	}
	catch(AMQPException& e)
	{
		return e.getMessage();
	}

	return std::string();
}

consumer::~consumer()
{
	delete m_qu;
	delete m_amqp;
}
