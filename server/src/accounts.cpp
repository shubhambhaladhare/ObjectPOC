#include "accounts.hpp"

// bool doesAccountHaveUsers()
// {
// 	// check if account have users
// }

// bool doesAccountHaveBuckets()
// {
// 	// check if account have buckets
// }

void account::setClassVariables(database& d)
{
	std::vector<std::vector<std::string>> result;

	int retcode = d.executeQuery("Select * from  Msys_Accounts where accountName = '" + m_accountName + "'" , result );

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}

	if (result.empty())
	{
		m_bExist = false;
		m_accountId = "";
	}
	else
	{
		m_bExist = true;
		m_accountId = result[0][0];
		std::string decpwd = result[0][2];
		//cout << decpwd << endl;
		m_passewordcheck = Decrypt(decpwd);
		//cout << m_passewordcheck << endl;
		m_userNameCheck = result[0][1];
		//cout << m_passewordcheck << endl;
	}

}

account::account(const std::string& accountName, const std::string& password, database& d)
{
	m_accountName = accountName;
	m_password = password;
	setClassVariables(d);
}

void account::createAccount(s3service::s3account& accountResponse, database& d)
{
	if (!m_bExist)
	{
	EntityBase obj;
		m_accountId = "acc" + obj.getUniqueId();
		std::string encpwd = Encrypt(m_password);
		//cout << encpwd << endl;
		accountResponse.set_accountid(m_accountId);
		//need to call that encrypt function and then assigned to m_password variable
		int retcode = d.executeQuery("Insert into Msys_Accounts (accountId, accountName, password) values ('" + m_accountId + "', '" + m_accountName + "', '" + encpwd + "')");

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
	}
	else
	{
		m_error = "Account with the name " + m_accountName + " already exists";

		auto err = accountResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("EntityAlreadyExists");
		err->set_errormessage(m_error);
	}
}

void account::deleteAccount(s3service::s3account& accountResponse, database& d)
{
	if((m_bExist) && (m_accountName == m_userNameCheck))
	{
		if ((m_bExist) && (m_passewordcheck == m_password))
		{
			int retcode= d.executeQuery("Delete from Msys_Access_Keys WHERE accountId = '" + m_accountId + "'");

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed" << std::endl;
				return;
			}

			retcode= d.executeQuery("Delete from Msys_Accounts WHERE accountId = '" + m_accountId + "'");

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed" << std::endl;
				return;
			}
		}
		else
		{
			m_error = "The account with password " + m_password + " cannot be found";

			auto err = accountResponse.add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("NoSuchEntity");
			err->set_errormessage(m_error);
		}
	}
	else
	{
		m_error = "The account with name " + m_accountName + " cannot be found";

		auto err = accountResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchEntity");
		err->set_errormessage(m_error);
	}
	
}

std::string account::getAccountName()
{
	return m_accountName;
}

std::string account::getPassword()
{
	return m_password;
}

std::string account::getAccountId()
{
	return m_accountId;
}

std::string account::getErrorMessage()
{
	return m_error;
}

bool account::doesAccountExist()
{
	return m_bExist;
}

account::~account()
{

}
