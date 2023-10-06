#include "accesskeys.hpp"
#include <string>

bool accessKey::isAccessKeyLimitReached(database& d)
{
	std::vector<std::vector<std::string>> result;

	int retcode = d.executeQuery("SELECT COUNT(*) FROM Msys_Access_Keys WHERE userID = '" + m_userId + "'", result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return false;
	}

	if (std::atoi(result[0][0].c_str()) >= 2)
	{
		return true;
	}

	return false;
}

void accessKey::setClassVariables(database& d)
{
	std::vector<std::vector<std::string>> result;

	if (m_accessKeyId != "")
	{
		int retcode = d.executeQuery("Select * from  Msys_Access_Keys where accessKeyId = '" + m_accessKeyId + "'" , result);

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
	}

	if (result.empty())
	{
		m_bExists = false;

		m_userId = "";
		m_creationTimestamp = "";
		m_lastUsedTimestamp = "";
		m_status = "ACTIVE";
		m_secretKey = "";
		m_userId = "";
		m_accountId = "";
    }
	else
	{
		m_bExists = true;
		m_secretKey = result[0][1];
		m_creationTimestamp = result[0][2];
		m_lastUsedTimestamp = result[0][3];
		m_status = result[0][4];
		m_userId = ((result[0][6] == "NULL") ? "" : result[0][6]);
		m_accountId = ((result[0][5] == "NULL") ? "" : result[0][5]);
	}
}

accessKey::accessKey(database& d, const std::string& accessKeyId = "")
{
    m_accessKeyId = accessKeyId;

    setClassVariables(d);
}

void accessKey::createAccessKey(const std::string& accountId, const std::string& userId, s3service::s3accesskey& keyResponse, database& d)
{
	//	char userDN[]="ou=People,dc=example,dc=com";
	//	char *accessKey=&m_accessKeyId[0];
	//	char *secretKey=&m_secretKey[0];
	//	int accessKeyCount =0;

	m_userId = userId;
	m_accountId = accountId;

	if (isAccessKeyLimitReached(d))
	{
		m_error = "Cannot exceed accesskey limit per user: 2";

		auto err = keyResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("LimitExceeded");
		err->set_errormessage(m_error);
	}
	else
	{
		m_accessKeyId = "access" + getUniqueId();
		m_creationTimestamp = getCurrentTimeStamp();
		m_lastUsedTimestamp = m_creationTimestamp;

		m_secretKey = "secret" + getUniqueId();
		if (m_userId == "")
		{
			int retcode = d.executeQuery("Insert into Msys_Access_Keys " \
					"(accessKeyId, secretKey, creationTimestamp, lastUsedTimestamp , status, accountId, userId) " \
					"values " \
					"('" + m_accessKeyId + "', '" + m_secretKey + "', '" + m_creationTimestamp + "', '" + m_lastUsedTimestamp + "', 'ACTIVE', '" + m_accountId +"', NULL)");

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed" << std::endl;
				return;
			}
		}
		else
		{
			int retcode = d.executeQuery("Insert into Msys_Access_Keys " \
					"(accessKeyId, secretKey, creationTimestamp, lastUsedTimestamp , status, userId) " \
					"values " \
					"('" + m_accessKeyId + "', '" + m_secretKey + "', '" + m_creationTimestamp + "', '" + m_lastUsedTimestamp + "', 'ACTIVE', '" + m_userId + "')");

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed" << std::endl;
				return;
			}
		}

		keyResponse.set_accesskeyid(m_accessKeyId);
		keyResponse.set_secretkey(m_secretKey);
    	keyResponse.set_accesskeyselector("HMAC");
    	keyResponse.set_createdate(m_creationTimestamp);
    	keyResponse.set_status(m_status);
	}
}

void accessKey::deleteKey(s3service::s3accesskey& keyResponse, database& d)
{
	//char userDN[]="ou=People,dc=example,dc=com";

	if (!m_bExists)
	{
		m_error = "The accesskey with id "+ m_accessKeyId + " cannot be found";

		auto err = keyResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchEntity");
		err->set_errormessage(m_error);
	}
	else
	{
		int retcode = d.executeQuery("DELETE FROM Msys_Access_Keys WHERE accessKeyId = '" + m_accessKeyId + "' AND secretKey = '" + m_secretKey + "'");

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
	}
}

void accessKey::changeKeyStatus(const std::string& status, database& d)
{
    if (m_status == status)
    {
        m_error = "Given the already defined status";
    }
    else
    {
		int retcode = d.executeQuery("Update Msys_Access_Keys SET status = '" + status + "' "\
		"where accessKeyId = '" + m_accessKeyId + "' AND secretKey = '" + m_secretKey + "'");

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
    }
}

std::string accessKey::getUserId()
{
	return m_userId;
}

std::string accessKey::getAccessKeyCreationTime()
{
	return m_creationTimestamp;
}

std::string accessKey::getAccessKeyLastUsedTime()
{
	return m_lastUsedTimestamp;
}

std::string accessKey::getAccessKeyId()
{
	return m_accessKeyId;
}

std::string accessKey::getSecretKey()
{
	return m_secretKey;
}

std::string accessKey::getErrorMessage()
{
	return m_error;
}

std::string accessKey::getKeyStatus()
{
	return m_status;
}

std::string accessKey::getAccountId()
{
	return m_accountId;
}

bool accessKey::doesKeyExist()
{
	return m_bExists;
}

accessKey::~accessKey()
{

}
