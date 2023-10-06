#include <cstdlib> 
#include <string>
#include <vector>
#include <ctime>

#include <EntityBase.h>
#include "s3service.pb.h"
#include "mssql.hpp"

class account : public EntityBase
{
	bool m_bExist;
	std::string m_accountId, m_error, m_accountName, m_password; 
	std::string m_passewordcheck, m_userNameCheck;
		
	static const char* AES_KEY_STRING;
        
	// bool accountHasUser();

	// bool accountHasBucket();

	void setClassVariables(database& d);

	public:

	account(const std::string& accountName, const std::string& password,database& d);

	void createAccount(s3service::s3account& accountResponse, database& d);

	void deleteAccount(s3service::s3account& accountResponse, database& d);

	std::string getAccountName();

	std::string getPassword();

	std::string getAccountId();

	bool doesAccountExist();

	std::string getErrorMessage();
	
	~account();
};
