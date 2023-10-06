#include "handler.hpp"
#include "mssql.hpp"

std::string flowHandler::checkKeyAuthorizationAndReturnAccountId(const std::string& accessKeyId, const std::string& secretKey, database& d)
{
	std::vector<std::vector<std::string>> result;
	int retcode = d.executeQuery("Select accountId from Msys_Access_Keys where accessKeyId = '" + accessKeyId + "' AND secretKey = '" + secretKey + "'", result);

	if (retcode == -1)
	{
		std::cout << "ExecuteQuery failed" << std::endl;
		return "";
	}

	if (result.empty())
	{
		return "";
	}

	/*if (result[0][0] == "NULL")
	{
		result.clear();

		retcode = d.executeQuery("Select Msys_Access_Keys.userId from Msys_Access_Keys INNER JOIN Msys_Users " \
				"ON Msys_Access_Keys.userId = Msys_Users.userId " \
				"where Msys_Access_Keys.accessKeyId = '" + accessKeyId + "' AND Msys_Access_Keys.secretKey = '" + secretKey +"'", result);

		if (retcode == -1)
		{
			std::cout << "ExecuteQuery failed" << std::endl;
			return "";
		}
	}*/
	return result[0][0];
}
std::string flowHandler::checkKeyAuthorizationAndReturnAccountIdForAccessKey(const std::string& accessKeyId, const std::string& secretKey,  const std::string& username, auto keyResponse, database& d)
{
	std::vector<std::vector<std::string>> result;
	std::vector<std::vector<std::string>> result1;
		int retcode = d.executeQuery("Select userName from Msys_Users where userName = '" + username + "'", result1);

	if (retcode == -1)
	{
		std::cout << "ExecuteQuery failed" << std::endl;
		return "";
	}
	
	if (result1.empty())
	{
		return "";
	}
	
	if(result1[0][0] != username)
	{
			auto err = keyResponse->add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("SignatureDoesNotMatch");
			err->set_errormessage("The request signature we calculated does not match the signature you provided.");

			return "";
	}
	else
	{
	int retcode = d.executeQuery("Select a.accountId from Msys_Access_Keys a join Msys_Users u  ON a.accountId = u.accountId where a.accessKeyId ='" + accessKeyId + "' AND a.secretKey = '" + secretKey + "' AND u.userName = '" + username + "'", result);

	if (retcode == -1)
	{
		std::cout << "ExecuteQuery failed" << std::endl;
		return "";
	}

	if (result.empty())
	{
		if (result.empty())
		{
			retcode = d.executeQuery("Select u.accountId from Msys_Access_Keys a join Msys_Users u  ON a.userId = u.userId where a.accessKeyId ='" + accessKeyId + "' AND a.secretKey = '" + secretKey + "' AND u.userName = '" + username + "'", result);
			if (retcode == -1)
			{
				std::cout << "ExecuteQuery failed" << std::endl;
				return "";
			}
			if(result.empty())
			{
				return "";
			}
			if  (result[0][0] == "NULL" )
			{
				return "";
			}
		}
			
	}

	/*if (result[0][0] == "NULL")
	{
		result.clear();

		retcode = d.executeQuery("Select Msys_Access_Keys.userId from Msys_Access_Keys INNER JOIN Msys_Users " \
				"ON Msys_Access_Keys.userId = Msys_Users.userId " \
				"where Msys_Access_Keys.accessKeyId = '" + accessKeyId + "' AND Msys_Access_Keys.secretKey = '" + secretKey +"'", result);

		if (retcode == -1)
		{
			std::cout << "ExecuteQuery failed" << std::endl;
			return "";
		}
	}*/
	}
	return result[0][0];
}

std::string flowHandler::checkKeyAuthorizationAndReturnAccountIdForBucket(const std::string& accessKeyId, const std::string& secretKey, database& d)
{
	std::vector<std::vector<std::string>> result;
	int retcode = d.executeQuery("Select accountId from Msys_Access_Keys where accessKeyId = '" + accessKeyId + "' AND secretKey = '" + secretKey + "'", result);

	if (retcode == -1)
	{
		std::cout << "ExecuteQuery failed" << std::endl;
		return "";
	}

	if (result.empty())
	{
		return "";
	}

	if (result[0][0] == "NULL")
	{
		result.clear();

		retcode = d.executeQuery("Select Msys_Users.userId from Msys_Access_Keys INNER JOIN Msys_Users " \
				"ON Msys_Access_Keys.userId = Msys_Users.userId " \
				"where Msys_Access_Keys.accessKeyId = '" + accessKeyId + "' AND Msys_Access_Keys.secretKey = '" + secretKey +"'", result);

		if (retcode == -1)
		{
			std::cout << "ExecuteQuery failed" << std::endl;
			return "";
		}
	}
	return result[0][0];
}
std::vector<std::string> flowHandler::getUserIdForUserName(const std::string& userName, const std::string& accountId, database& d)
{
	std::vector<std::vector<std::string>> result;
	std::vector<std::string> res;

	int retcode = d.executeQuery("Select userId, userName from Msys_Users WHERE userName = '" + userName + "'" , result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return res;
	}

	if (result.empty())
	{
		return res;
	}
	res.push_back(result[0][0]);
	res.push_back(result[0][1]);

	return res;
}

bool flowHandler::doesAccountHaveBuckets(const std::string& accountId, database& d)
{
	std::vector<std::vector<std::string>> result;

	int retcode = d.executeQuery("Select COUNT(*) from Msys_Buckets where accountId = '" + accountId + "'", result);

	if (retcode == -1)
	{
		 std::cout << "Execute query failed" << std::endl;
		 return true;
	}

	if(std::atoi(result[0][0].c_str()) > 0)
	{
		return true;
	}

	return false;
}

void flowHandler::callDeleteForAllUsers(const std::string& accountId, database& d)
{
	std::vector<std::vector<std::string>> result;

	int retcode = d.executeQuery("Select userName from Msys_Users WHERE accountId = '" + accountId + "'", result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}

	for (int i = 0; i < result.size(); i++)
	{
		user u(result[i][0], accountId, d);
		s3service::s3user userResponse;

		u.deleteUser(userResponse, d);
	}
}

void flowHandler::performAccountAction(s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response, database& d)
{
	for (int i = 0; i < m_proto.account_size(); i++)
	{
		auto accountService = m_proto.account(i);

		auto accountResponse = response.add_account();

		accountResponse->set_accop(accountService.accop());
		accountResponse->set_accountname(accountService.accountname());
		//valid username and password
		credential cred(accountService.accountname(),accountService.password());
		
		if(accountService.accop() == s3service::s3account::CREATE_ACCOUNT)
		{
			bool accname = cred.isvalidaccname(*accountResponse);
			
			if(accname == false)
			{
				continue;
			}
			bool password = cred.isvalidPassword(*accountResponse);
			
			if(password == false)
			{
				continue;
			}
		}
		

		
		account acc(accountService.accountname(), accountService.password(), d);

		if (accountService.accop() == s3service::s3account::CREATE_ACCOUNT)
		{
			acc.createAccount(*accountResponse, d);

			auto err = acc.getErrorMessage();

			if (err.size() == 0)
			{
				auto accId = acc.getAccountId();

				accessKey keys(d, "");

				auto keyResponse = accountResponse->add_keys();

				keyResponse->set_accessop(s3service::s3accesskey::CREATE_KEY);

				keys.createAccessKey(accId, "", *keyResponse, d);
		    }
	    }
		else
		{
			bool isAuthorized = checkAuthorization(accountService.accountname(), accountService.password() , d);

			if (!isAuthorized)
			{
				auto err = accountResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("CredentialsDoesNotMatch");
				err->set_errormessage("The request credentials we have does not match the credentials you provided.");
				continue;
			}

			auto accId = acc.getAccountId();

			if (accId.size() == 0)
			{
				auto err = accountResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The account with name " + accountService.accountname() + " cannot be found");
				continue;
			}

			auto bucketExist = doesAccountHaveBuckets(accId, d);

			if (bucketExist)
			{
				auto err = accountResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NotEmpty");
				err->set_errormessage("The account with name " + accountService.accountname() + " is not empty");
				continue;
			}

			callDeleteForAllUsers(accId, d);

			acc.deleteAccount(*accountResponse, d);
		}
	}
}

void flowHandler::performUserAction(s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response, database& d)
{
	for (int i = 0; i < m_proto.user_size(); i++)
	{
		auto userService = m_proto.user(i);

		auto userResponse = response.add_user();

		userResponse->set_userop(userService.userop());
		userResponse->set_username(userService.username());

		auto auther = userService.auth(0);

		std::string accountId = checkKeyAuthorizationAndReturnAccountId(auther.accesskey(), auther.secretkey(),d);

		if (accountId.size() == 0)
		{
			auto err = userResponse->add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("SignatureDoesNotMatch");
			err->set_errormessage("The request signature we calculated does not match the signature you provided.");
			continue;
		}
	        //valid username and password
	        credential cred(userService.username());
		if(userService.userop() == s3service::s3user::CREATE_USER)
		{
			bool usrname = cred.isvalidusername(*userResponse);
			
			if(usrname == false)
			{
				continue;
			}
		}
		user users(userService.username(), accountId, d);

		if (userService.userop() == s3service::s3user::CREATE_USER)
		{
			users.createUser(*userResponse, d);
		}
		else if (userService.userop() == s3service::s3user::DELETE_USER)
		{
			users.deleteUser(*userResponse, d);
		}
		else
		{
			credential cred(userService.newusername());
		
			bool usrname = cred.isvalidusername(*userResponse);
		
			if(usrname == false)
			{
				continue;
			}
			users.updateUser(userService.newusername(), *userResponse, d);
		}
	}
}
bool flowHandler::checkKeyAuthorizationAndReturnAccessSecretkeyId(const std::string& accessKeyId, const std::string& secretKey, database& d)
{
		std::vector<std::vector<std::string>> result;
	int retcode = d.executeQuery("Select accessKeyId, secretKey from Msys_Access_Keys where accessKeyId = '" + accessKeyId + "' AND secretKey = '" + secretKey + "'", result);

	if (retcode == -1)
	{
		std::cout << "ExecuteQuery failed" << std::endl;
		return false;
	}

	if (result.empty())
	{
		return false;
	}

	if (result[0][0] == "NULL")
	{
		result.clear();

		retcode = d.executeQuery("Select accessKeyId, secretKey from Msys_Access_Keys INNER JOIN Msys_Users " \
				"ON Msys_Access_Keys.userId = Msys_Users.userId " \
				"where Msys_Access_Keys.accessKeyId = '" + accessKeyId + "' AND Msys_Access_Keys.secretKey = '" + secretKey +"'", result);

		if (retcode == -1)
		{
			std::cout << "ExecuteQuery failed" << std::endl;
			return false;
		}
	}
	
	if (result[0][0] != accessKeyId)
	{
		return false;
	}
	if(result[0][1] != secretKey)
	{

		return false;
	}
	return true;
}
void flowHandler::performAccessKeyAction(s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response, database& d)
{
	for (int i = 0; i < m_proto.accesskey_size(); i++)
	{
		auto accessKeyService = m_proto.accesskey(i);

		auto keyResponse = response.add_accesskey();

		keyResponse->set_accessop(accessKeyService.accessop());

		auto auth = accessKeyService.auth(0);

		
		std::string accountId = checkKeyAuthorizationAndReturnAccountIdForAccessKey(auth.accesskey(), auth.secretkey(), accessKeyService.username(),keyResponse, d);

		
		if (accountId.size() == 0)
		{
			auto err = keyResponse->add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("SignatureDoesNotMatch");
			err->set_errormessage("The request signature we calculated does not match the signature you provided.");

			continue;
		}
		
				
		/*if (accountId.size() == "NULL")
		{
			auto err = keyResponse->add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("SignatureDoesNotMatch");
			err->set_errormessage("The request signature we calculated does not match the signature you provided.");

			continue;
		}*/
		
		if(!checkKeyAuthorizationAndReturnAccessSecretkeyId(auth.accesskey(), auth.secretkey(),d))
		{
			auto err = keyResponse->add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("SignatureDoesNotMatch");
			err->set_errormessage("The request signature we calculated does not match the signature you provided.");
			continue;
		}

		accessKey key(d,accessKeyService.accesskeyid());

		if (accessKeyService.accessop() == s3service::s3accesskey::CREATE_KEY)
		{
			
			std::vector<std::string> res = getUserIdForUserName(accessKeyService.username(), accountId, d);
			if(res.empty())
			{
				auto err = keyResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The user with name " + accessKeyService.username() + " cannot be found");
				continue;
			}
			if( res[1] != accessKeyService.username())
			{
				auto err = keyResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The user with name " + accessKeyService.username() + " cannot be found");
				continue;
			}
			auto userId = res[0];
			if (userId.size() == 0)
			{
				auto err = keyResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The user with name " + accessKeyService.username() + " cannot be found");

				continue;
			}

			key.createAccessKey(accountId, userId, *keyResponse, d);
		}
		else if (accessKeyService.accessop() == s3service::s3accesskey::DELETE_KEY)
		{
						
			std::vector<std::string> res = getUserIdForUserName(accessKeyService.username(), accountId, d);
			if(res.empty())
			{
				auto err = keyResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The user with name " + accessKeyService.username() + " cannot be found");
				continue;
			}
			if( res[1] != accessKeyService.username())
			{
				auto err = keyResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The user with name " + accessKeyService.username() + " cannot be found");
				continue;
			}
			key.deleteKey(*keyResponse, d);
		}
		else if (accessKeyService.accessop() == s3service::s3accesskey::CHANGE_KEY_STATUS)
		{
									
			std::vector<std::string> res = getUserIdForUserName(accessKeyService.username(), accountId, d);
			if(res.empty())
			{
				auto err = keyResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The user with name " + accessKeyService.username() + " cannot be found");
				continue;
			}
			if( res[1] != accessKeyService.username())
			{
				auto err = keyResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The user with name " + accessKeyService.username() + " cannot be found");
				continue;
			}
			key.changeKeyStatus(accessKeyService.status(),d);
		}
		else
		{
			std::vector<std::string> res = getUserIdForUserName(accessKeyService.username(), accountId, d);
			if(res.empty())
			{
				auto err = keyResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The user with name " + accessKeyService.username() + " cannot be found");
				continue;
			}
			if( res[1] != accessKeyService.username())
			{
				auto err = keyResponse->add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchEntity");
				err->set_errormessage("The user with name " + accessKeyService.username() + " cannot be found");
				continue;
			}
			keyResponse->set_lastuseddate(key.getAccessKeyLastUsedTime());
			keyResponse->set_servicename("s3");
			keyResponse->set_region("");
		}
	}
}

void flowHandler::performBucketAction(s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response, database& d)
{
	for (int i = 0; i < m_proto.bucket_size(); i++)
	{
		auto bucketService = m_proto.bucket(i);

		auto bucketResponse = response.add_bucket();

		bucketResponse->set_buckop(bucketService.buckop());

		auto auth = bucketService.auth(0);

		std::string accountId = checkKeyAuthorizationAndReturnAccountIdForBucket(auth.accesskey(), auth.secretkey(),d);

		if (accountId.size() == 0)
		{
			auto err = bucketResponse->add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("SignatureDoesNotMatch");
			err->set_errormessage("The request signature we calculated does not match the signature you provided.");

			continue;
		}

		if (bucketService.bucketname() == "")
		{
			auto err = bucketResponse->add_errorinfo();

			err->set_resourcetype("SERVICE");
			err->set_errorcode("MethodNotAllowed");
			err->set_errormessage("The specified method is not allowed against this resource.");
			err->set_methodtype("PUT");

			continue;
		}
		//valid username and password
		
		credential cred(bucketService.bucketname());
		
		bool buckname = cred.isvalidbucketname(*bucketResponse);
		
		if(buckname == false)
		{
			continue;
		}
		
		bucket bucketObj(bucketService.bucketname(), accountId,d);

		if (bucketService.buckop() == s3service::s3bucket::CREATE_BUCKET)
		{
			bucketObj.createBucket(*bucketResponse,d);
		}
		else if (bucketService.buckop() == s3service::s3bucket::DELETE_BUCKET)
		{
			// check if there is any object in that bucket
			bucketObj.deleteBucket(*bucketResponse,d);
		}
		else if (bucketService.buckop() == s3service::s3bucket::PUT_BUCKET_TAG)
		{
			std::map<std::string, std::string> tags;

			for (int i = 0; i < bucketService.tag_size(); i++)
			{
				auto tag = bucketService.tag(i);

				tags.insert({tag.key(), tag.value()});
			}

			bucketObj.putBucketTags(tags, *bucketResponse, d);
		}
		else if(bucketService.buckop() == s3service::s3bucket::PUT_BUCKET_VERSION)
		{
			bucketObj.put_Bucket_Version(*bucketResponse,d);
		}
		
		else if(bucketService.buckop() == s3service::s3bucket::GET_BUCKET_VERSION)
		{
			bucketObj.get_Bucket_Version(*bucketResponse,d);
		}
		
		else if(bucketService.buckop() == s3service::s3bucket::s3bucket::LIST_OBJECT)
		{
			bucketObj.list_object(*bucketResponse,d);
		}
		else if(bucketService.buckop() == s3service::s3bucket::LIST_OBJECT_VERSIONS)
		{
			bucketObj.list_object_version(*bucketResponse,d);
		}
		else if(bucketService.buckop() == s3service::s3bucket::SET_LIFECYCLE_RULE)
		{
			std::string exp_data = bucketService.expiry_date();
			bucketObj.set_lifecycle_rule(*bucketResponse, d, exp_data);
		}
		
		else if( bucketService.buckop() == s3service::s3bucket::SET_STORAGE_QUOTA)
		{
			std::string memoryspace = bucketService.storagequota();
			//cout << memoryspace << endl;
			bucketObj.set_storageQuota(*bucketResponse, d, memoryspace);
		}
		
		else if( bucketService.buckop() == s3service::s3bucket::GET_STORAGE_LENS)
		{
			bucketObj.get_storagelens(*bucketResponse, d);
		}
		else
		{
			bucketObj.listBucketTags(*bucketResponse);
		}
	}
}

void flowHandler::performObjectAction(serverSocket& ssRead,serverSocket& ssWrite, s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response, database& d)
{
	for (int i = 0; i < m_proto.object_size(); i++)
	{
		auto objectService = m_proto.object(i);
		
		auto objectResponse = response.add_object();

		objectResponse->set_objectop(objectService.objectop());
		objectResponse->set_objectname(objectService.objectname());

		auto auth = objectService.auth(0);

		std::string accountId = checkKeyAuthorizationAndReturnAccountIdForBucket(auth.accesskey(), auth.secretkey(), d);
		


		if (accountId.size() == 0)
		{
			auto err = objectResponse->add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("SignatureDoesNotMatch");
			err->set_errormessage("The request signature we calculated does not match the signature you provided.");

			continue;
		}
		
		//version
		else if(objectService.objectop() == s3service::s3object::GET_OBJECT_BY_VERSION)
		{
			if ((objectService.bucketname() == ""))
			{
				auto err = objectResponse->add_errorinfo();

				err->set_resourcetype("SERVICE");
				err->set_errorcode("MethodNotAllowed");
				err->set_errormessage("The specified method is not allowed against this resource.");
				err->set_methodtype("PUT");

				continue;
			}
					//for bucket name credintial check
		credential cred(objectService.bucketname());
		bool buckname = cred.isvalidbucketnameObj(*objectResponse);
		
		if(buckname == false)
		{
			continue;
		}
			bucket bucketObj(objectService.bucketname(), accountId, d);
			std::string bucketName = bucketObj.getBucketName();

			auto bucketId = bucketObj.getBucketId();
                
			if (!bucketId.size())
			{
				auto err = objectResponse->add_errorinfo();

				err->set_resourcetype("SERVICE");
				err->set_errorcode("EntityNotFound");
				err->set_errormessage("The specified bucket is not present");

				continue;
			}
			std::string version=objectService.getobjectbyversionid();
			object objectObj(bucketId, d, version);
			objectObj.downloadObjectByVersion(ssWrite, *objectResponse, bucketName);
			continue;
			
			
		}
		
		else if (objectService.objectop() == s3service::s3object::GET_DELETE_MARKER_OBJECT)
		{
					//for bucket name credintial check
		credential cred(objectService.bucketname());
		bool buckname = cred.isvalidbucketnameObj(*objectResponse);
		
		if(buckname == false)
		{
			continue;
		}
			bucket bucketObj(objectService.bucketname(), accountId, d);
			std::string bucketName = bucketObj.getBucketName();
			if (!bucketName.size())
			{
				auto err = objectResponse->add_errorinfo();

				err->set_resourcetype("SERVICE");
				err->set_errorcode("EntityNotFound");
				err->set_errormessage("The specified bucket is not present");

				continue;
			}
			auto bucketId = bucketObj.getBucketId();
			if (!bucketId.size())
			{
				auto err = objectResponse->add_errorinfo();

				err->set_resourcetype("SERVICE");
				err->set_errorcode("EntityNotFound");
				err->set_errormessage("The specified bucket is not present");

				continue;
			}
			credential cred3(objectService.markerid());
		
			bool buckname1 = cred3.isvalidMarkerId(*objectResponse);
			
			if(buckname1 == false)
			{
				continue;
			}
			object objectObj(d, bucketId, objectService.markerid());
			objectObj.getDeleteMarker(*objectResponse, d);
			continue;
			
		}
		else if(objectService.objectop() == s3service::s3object::LIST_DELETE_MARKER_OBJECT)
		{
					//for bucket name credintial check
		credential cred(objectService.bucketname());
		bool buckname = cred.isvalidbucketnameObj(*objectResponse);
		
		if(buckname == false)
		{
			continue;
		}
			bucket bucketObj(objectService.bucketname(), accountId, d);
			std::string bucketName = bucketObj.getBucketName();
			std::string BucketVersion=bucketObj.getBucketVersion();
			if (!bucketName.size())
			{
				auto err = objectResponse->add_errorinfo();

				err->set_resourcetype("SERVICE");
				err->set_errorcode("EntityNotFound");
				err->set_errormessage("The specified bucket is not present");

				continue;
			}
			auto bucketId = bucketObj.getBucketId();
			if (!bucketId.size())
			{
				auto err = objectResponse->add_errorinfo();

				err->set_resourcetype("SERVICE");
				err->set_errorcode("EntityNotFound");
				err->set_errormessage("The specified bucket is not present");

				continue;
			}
			object objectObj(d, objectService.objectname());
			objectObj.ListdeleteMarkerObject(*objectResponse, d, BucketVersion);
			continue;
		}
		
		if ((objectService.bucketname() == "") || (objectService.objectname() == ""))
		{
			auto err = objectResponse->add_errorinfo();

			err->set_resourcetype("SERVICE");
			err->set_errorcode("MethodNotAllowed");
			err->set_errormessage("The specified method is not allowed against this resource.");
			err->set_methodtype("PUT");

			continue;
		}
		//for bucket name credintial check
		credential cred(objectService.bucketname());
		bool buckname = cred.isvalidbucketnameObj(*objectResponse);
		
		if(buckname == false)
		{
			continue;
		}
		
		//for object name credintial check
		credential cred1(objectService.objectname());
		
		bool Objectname = cred1.isvalidObjectname(*objectResponse);
		
		if(buckname == false)
		{
			continue;
		}
		
		bucket bucketObj(objectService.bucketname(), accountId, d);
		std::string bucketName = bucketObj.getBucketName();
		
		auto bucketId = bucketObj.getBucketId();
                
		if (!bucketId.size())
		{
			auto err = objectResponse->add_errorinfo();
			err->set_errortype(" Bucket");
			err->set_resourcetype("SERVICE");
			err->set_errorcode("EntityNotFound");
			err->set_errormessage("The specified bucket is not present");

			continue;
		}
		
		

		

		object objectObj(bucketId, objectService.objectname(), d);
		
		std::string BucketVersion=objectObj.getBucketVersion();
      
        	//objectObj.nameObject(objectService.objectname(),*objectResponse,BucketVersion);

		if (objectService.objectop() == s3service::s3object::PUT_OBJECT)
		{
			std::string BucketVersion = bucketObj.getBucketVersion();//std::string archive
			objectObj.putObject(ssRead, objectService, *objectResponse, d,BucketVersion,bucketName, auth.accesskey(), auth.secretkey());
		}

                    else if (objectService.objectop() == s3service::s3object::PUT_OBJECT_LOCKING)
                {
                      
                        objectObj.putObjectLock(*objectResponse,  d); /* proto + part file read and keep in uniqueid folder */
                }
         
                else if (objectService.objectop() == s3service::s3object::SET_OBJECT_RETENTION) {
			std::string retentionDate = objectService.retention_duration();

			std::cout<<"serverretentiondate"<<retentionDate<<std::endl;
   			   objectObj.setObjectRetention(*objectResponse, retentionDate, d);
                }


		else if (objectService.objectop() == s3service::s3object::DELETE_OBJECT)
		{
			std::string BucketVersion = bucketObj.getBucketVersion();
			//cout << BucketVersion << endl;
			objectObj.deleteObject(d,*objectResponse,bucketName, BucketVersion, auth.accesskey(), auth.secretkey());
		}
		else if (objectService.objectop() == s3service::s3object::GET_OBJECT)
		{
			objectObj.downloadObject(ssWrite,  *objectResponse, bucketName, d);
		}
		
		else if (objectService.objectop() == s3service::s3object::PUT_MULTIPART_OBJECT)
		{
			std::string BucketVersion = bucketObj.getBucketVersion();
			objectObj.putMultiPartUpload(ssRead, objectService, *objectResponse, bucketName, d, BucketVersion);  /* proto + part file read and keep in uniqueid folder */
		}

		  

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (objectService.objectop() == s3service::s3object::INIT_MULTIPART_OBJECT)
		{
			objectObj.initMultiPartUpload(ssRead, objectService, *objectResponse, d); /* proto read */
		}
		else if(objectService.objectop() == s3service::s3object::PUT_MULTIPART)
		{
			objectObj.putmultipart(ssRead, objectService, *objectResponse,d);
		}

		else if (objectService.objectop() == s3service::s3object::COMPLETE_MULTIPART_OBJECT)
		{
			std::string BucketVersion = bucketObj.getBucketVersion();
			objectObj.completeMultiPartUpload(ssRead, objectService, *objectResponse, bucketName, BucketVersion, d);
		}
		else if (objectService.objectop() == s3service::s3object::ABORT_MULTIPART_OBJECT)
		{
			objectObj.abortMultiPartUpload(ssRead, objectService, *objectResponse, d);
		}
		else if (objectService.objectop() == s3service::s3object::LIST_MULTIPART_OBJECT)
		{
			objectObj.listMultiPartUpload(ssRead, objectService, *objectResponse, d);
		}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// else if (bucketService.buckop() == s3service::s3bucket::PUT_BUCKET_TAG)
		// {
		// 	std::map<std::string, std::string> tags;

		// 	for (int i = 0; i < bucketService.tag_size(); i++)
		// 	{
		// 		auto tag = bucketService.tag(i);

		// 		tags.insert({tag.key(), tag.value()});
		// 	}

		// 	bucketObj.putBucketTags(tags, *bucketResponse);
		// }
		// else
		// {
		// 	bucketObj.listBucketTags(*bucketResponse);
		// }
		
	}
}

bool flowHandler::checkAuthorization(const std::string& accountName, const std::string& password,database& d)
{
	account acc1(accountName, password,d);

	return acc1.doesAccountExist();
}

flowHandler::flowHandler()
{

}

void flowHandler::performAction(serverSocket& ssRead, serverSocket& ssWrite, database& d,s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response)
{
	response.set_entitytype(m_proto.entitytype());
	response.set_requestid(m_proto.requestid());
	auto entityType = m_proto.entitytype();
	//entityType=s3service::serviceRequestResponse::ACCOUNT;
	switch (entityType)
	{
		case s3service::serviceRequestResponse::USER:

			performUserAction(m_proto, response,d);
			break;

		case s3service::serviceRequestResponse::ACCOUNT:

			performAccountAction(m_proto, response, d);
			break;

		case s3service::serviceRequestResponse::ACCESSKEY:

			performAccessKeyAction(m_proto, response, d);
			break;

		case s3service::serviceRequestResponse::BUCKET:

			performBucketAction(m_proto, response, d);
			break;

		case s3service::serviceRequestResponse::OBJECT:

			performObjectAction(ssRead, ssWrite, m_proto, response, d);
			break;
		//case //need to write s3service::serviceRequestResponse::version
			//break;
	}
}

flowHandler::~flowHandler()
{

}
