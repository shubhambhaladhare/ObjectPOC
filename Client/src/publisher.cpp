#include "publisher.hpp"

void publisher::createConnString()
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

publisher::publisher(std::string username, std::string password, std::string host, std::string port, std::string exchangeName):m_amqp(NULL)
{
	m_username = username;
	m_password = password;
	m_host = ((host.size() == 0) ? "localhost" : host);
	m_port = ((port.size() == 0) ? "5672" : port);
	m_exchangeName = exchangeName;

	createConnString();

	m_headers.insert({"Delivery-mode", "1"});
}

void publisher::addHeaders(std::map<std::string, std::string> headers)
{
	m_headers.insert(headers.begin(), headers.end());
}

std::map<std::string, std::string> publisher::getHeaders()
{
	return m_headers;
}

std::string publisher::init()
{
	try
	{
		if(m_exchangeName.size() == 0)
		{
			throw std::string("Exchange Name is empty");
		}

		m_amqp = new AMQP(m_connString);

		if(m_amqp == NULL)
		{
			throw std::string("Error while creating connection");
		}

		m_ex = m_amqp->createExchange(m_exchangeName);

		if(m_ex == NULL)
		{
			throw std::string("createExchange() error");
		}

		m_ex->Declare(m_exchangeName, "topic");
	}
	catch(AMQPException& e)
	{
		return e.getMessage();
	}
	catch(const std::string& str)
	{
		return str;
	}

	return std::string();
}

std::string publisher::createAndBindQueue(const std::string& queueName, const std::string& topicName)
{
	try
	{
		if(queueName.size() == 0)
		{
			throw std::string("Queue Name is empty");
		}
		if(topicName.size() == 0)
		{
			throw std::string("Topic Name is empty");
		}
		if(m_exchangeName.size() == 0)
		{
			throw std::string("Exchange Name is empty");
		}

		AMQPQueue *qu = m_amqp->createQueue(queueName);

		if (qu == NULL)
		{
			throw std::string("createQueue() error");
		}

		qu->Declare();
		qu->Bind(m_exchangeName, topicName);

		delete qu;
	}
	catch(AMQPException& e)
	{
		return e.getMessage();
	}
	catch(const std::string& str)
	{
		return str;
	}

	return std::string();
}

std::string publisher::publishMessage(const std::string& message, const std::string& topicName)
{
	try
	{
		if(message.size() == 0)
		{
			throw std::string("Message size is empty");
		}
		if(topicName.size() == 0)
		{
			throw std::string("Topic Name is empty");
		}

		for(auto it = m_headers.begin(); it != m_headers.end(); it++)
		{
			m_ex->setHeader(it->first, it->second);
		}

		m_ex->Publish(message, topicName);
	}
	catch(AMQPException& e)
	{
		return e.getMessage();
	}
	catch(const std::string& str)
	{
		return str;
	}

	return std::string();
}

std::string publisher::deleteExchange()
{
	try
	{
		m_ex->Delete();
	}
	catch(AMQPException& e)
	{
		return e.getMessage();
	}

	return std::string();
}

publisher::~publisher()
{
	delete m_ex;
	delete m_amqp;
}
