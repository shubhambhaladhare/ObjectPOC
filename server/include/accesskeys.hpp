#include <EntityBase.h>

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <ctime>

#include "s3service.pb.h"
#include "ldapserver.hpp"
#include "mssql.hpp"

class accessKey : public EntityBase
{
  bool m_bExists;
  std::string m_creationTimestamp, m_lastUsedTimestamp, m_status, m_userId, m_accountId, m_error, m_accessKeyId, m_secretKey;

  void setClassVariables(database& d);

  bool isAccessKeyLimitReached(database& d);

  bool doesAccessKeyIdExist(database& d);

  public:

  accessKey(database& d, const std::string& accesskeyId);

  void createAccessKey(const std::string& accountId, const std::string& userId, s3service::s3accesskey& keyResponse, database& d);

  void deleteKey(s3service::s3accesskey& keyResponse, database& d);

  void changeKeyStatus(const std::string& status, database& d);

  void listAccessKeys();

  std::string getUserId();

  int getKeyCount();

  std::string getAccessKeyCreationTime();

  std::string getKeyStatus();

  std::string getAccessKeyId();

  std::string getAccessKeyLastUsedTime();

  std::string getErrorMessage();

  std::string getSecretKey();

  std::string getAccountId();

  bool doesKeyExist();

  ~accessKey();
};

