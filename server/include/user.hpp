#include <string>
#include <vector>
#include <ctime>

#include <boost/filesystem.hpp>
#include <bits/stdc++.h>
#include <EntityBase.h>
#include "s3service.pb.h"
#include "mssql.hpp"

class user : public EntityBase
{
	bool m_bExist;
	std::string m_userName, m_userNamecheck, m_userId, m_timestamp, m_path, m_accountId, m_error, m_arn;

	void setArn();

	void setClassVariables(database& d);
 
	public:

	user(const std::string& userName, const std::string& accountId, database& d);

	void createUser(s3service::s3user& userResponse,database& d);
	
	void updateUser(const std::string& newUserName, s3service::s3user& userResponse, database& d);

	void deleteUser(s3service::s3user& userResponse, database& d);

	std::string getUserName();

	std::string getAccountId();

	std::string getUserId();

	std::string getTimestamp();

	std::string getPath();

	std::string getArn();

	std::string getErrorMessage();
	
	bool doesUserExist();

	void setUserName(const std::string& name);

	// std::string addbucket();

	// std::string updatebucket(); 

	// std::string deletebucket();
	
	~user();
};
