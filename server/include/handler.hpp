#include "user.hpp"
#include "accounts.hpp"
#include "accesskeys.hpp"
#include "ldapserver.hpp"
#include "bucket.hpp"
#include "object.hpp"

#include "EntityBase.h"
#include "s3service.pb.h"
#include "credential.hpp"

/*extern int accountCount;
extern int userCount;
extern int accessKeycount;
extern int bucketCount;
extern std::vector<account> accountList;
extern std::vector<user> userList;
extern std::vector<accessKey> accessKeyList;
extern std::vector<bucket> bucketList;
extern std::vector<object> objectList;
*/

class flowHandler
{
	void performUserAction(s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response, database& d);

	void performAccountAction(s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response, database& d);

	void performAccessKeyAction(s3service::serviceRequestResponse m_proto,  s3service::serviceRequestResponse& response, database& d);

	void performBucketAction(s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response, database& d);
	
	void performObjectAction(serverSocket& ssRead, serverSocket& ssWrite, s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response, database& d);

	bool checkAuthorization(const std::string& accountName, const std::string& password, database& d);

	std::string checkKeyAuthorizationAndReturnAccountId(const std::string& accessKey, const std::string& secretKey, database& d);
	std::string checkKeyAuthorizationAndReturnAccountIdForBucket(const std::string& accessKey, const std::string& secretKey, database& d);
	std::string checkKeyAuthorizationAndReturnAccountIdForAccessKey(const std::string& accessKeyId, const std::string& secretKey,  const std::string& username, auto keyResponse, database& d);
	std::vector<std::string> getUserIdForUserName(const std::string& userName, const std::string& accountId, database& d);

	void callDeleteForAllUsers(const std::string& accountId, database& d);

	bool doesAccountHaveBuckets(const std::string& accountId, database& d);
	bool checkKeyAuthorizationAndReturnAccessSecretkeyId(const std::string& accessKeyId, const std::string& secretKey, database& d);
	public:

	flowHandler();

	void setProtoObject(const std::string& serializedProto);

	 void performAction(serverSocket& ssRead, serverSocket& ssWrite, database& d, s3service::serviceRequestResponse m_proto, s3service::serviceRequestResponse& response);


	~flowHandler();
};
