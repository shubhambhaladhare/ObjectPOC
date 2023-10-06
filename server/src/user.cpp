#include "user.hpp"

void user::setArn()
{
	m_arn = "arn:aws:iam::" + m_accountId + ":user\\" + m_userName;
}

void user::setClassVariables(database& d)
{
	std::vector<std::vector<std::string>> result;

	int retcode = d.executeQuery("Select * from  Msys_Users where userName = '" + m_userName + "' AND accountId = '" + m_accountId + "'", result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}

	if (result.empty())
	{
		m_bExist = false;
		m_path = "";
		m_timestamp = "";
		m_userId = "";
		m_arn = "";
	}
	else
	{
		m_bExist = true;
		m_userId = result[0][0];
		m_arn = result[0][2];
		m_path = result[0][4];
		m_timestamp = result [0][3];
		m_userNamecheck = result[0][1];
	}
}

user::user(const std::string& userName, const std::string& accountId, database& d)
{
	m_userName = userName;
	m_accountId = accountId;
	setClassVariables(d);
}

void user::createUser(s3service::s3user& userResponse, database& d)
{

	if (!m_bExist)
	{
		std::string current_path = get_current_dir_name();
		std::string full_path = current_path + "/upload/";
		m_path = full_path;
		m_timestamp = getCurrentTimeStamp();
		m_userId = "user" + getUniqueId();
		m_arn = "arn:aws:iam::" + m_accountId + ":user\\" + m_userName;

		std::string query = "Insert into Msys_Users " \
	    		"(userId, userName, arn, timestamp, path, accountId) " \
	    		"values " \
				"('" + m_userId + "', '" + m_userName + "', '" + m_arn + "', '" + m_timestamp + "', '" + m_path + "', '" +  m_accountId + "')";

		//std::cout << "Query: " << query << std::endl;

	    int retcode = d.executeQuery(query);

	    if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			m_error = "User with the same name already exists";

			auto err = userResponse.add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("EntityAlreadyExists");
			err->set_errormessage("User with the name " + m_userName + " already exists");
			return;
		}

		userResponse.set_createdate(m_timestamp);
		userResponse.set_userid(m_userId);
		userResponse.set_arn(m_arn);
		userResponse.set_path(m_path);
	}
	else
	{
		m_error = "User with the same name already exists";

		auto err = userResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("EntityAlreadyExists");
		err->set_errormessage("User with the name " + m_userName + " already exists");
	}
}

void user::updateUser(const std::string& newUserName, s3service::s3user& userResponse, database& d)
{
	if (!m_bExist)
	{
		m_error = "No such user exist";

		auto err = userResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchEntity");
		err->set_errormessage("The user with name" + m_userName + " cannot be found");
	}
	else
	{
		int retcode = d.executeQuery("Update Msys_Users SET userName =  '" + newUserName  + "' where userName = '" + m_userName + "' AND accountId = '" + m_accountId + "'");

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}

		m_userName = newUserName;
		m_arn = "arn:aws:iam::" + m_accountId + ":user\\" + m_userName;

		retcode = d.executeQuery("Update Msys_Users SET arn =  '" + m_arn  + "' where userName = '" + m_userName + "' AND accountId = '" + m_accountId + "'");

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
	}
}

void user::deleteUser(s3service::s3user& userResponse, database& d)
{
	if (!m_bExist)
	{
		m_error = "No such user exist";

		auto err = userResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchEntity");
		err->set_errormessage("The user with name " + m_userName + " cannot be found");
	}
	else if( (m_bExist) && (m_userName == m_userNamecheck))
	{

		if (m_bExist)
		{
			int retcode = d.executeQuery("DELETE from Msys_Access_Keys where userId = '" + m_userId + "'");

			if ((retcode != 0) && (retcode != 100))
			{
				std::cout << "ExecuteQuery Failed" << std::endl;
				return;
			}

			retcode = d.executeQuery("DELETE from Msys_Users WHERE userName = '" + m_userName + "'");

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed" << std::endl;
				return;
			}
		}
	}
	else 
	{
		m_error = "No such user exist";

		auto err = userResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchEntity");
		err->set_errormessage("The user with name " + m_userName + " cannot be found");
	}
}

std::string user::getUserName()
{
	return m_userName;
}

std::string user::getAccountId()
{
	return m_accountId;
}

std::string user::getUserId()
{
	return m_userId;
}

std::string user::getTimestamp()
{
	return m_timestamp;
}

std::string user::getPath()
{
	return m_path;
}

std::string user::getArn()
{
	return m_arn;
}

std::string user::getErrorMessage()
{
	return m_error;
}

void user::setUserName(const std::string& name)
{
	m_userName = name;
}

bool user::doesUserExist()
{
	return m_bExist;
}

user::~user()
{

}
