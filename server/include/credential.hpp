#include <iostream>
#include <string>
#include "s3service.pb.h"
using namespace std;
class credential
{
	string m_password;
	string m_username;
	string m_error;
public:
	credential()=default;
	credential(string username);
	credential(string password, string username);
	bool isvalidPassword(s3service::s3account& accountResponse);
	bool isvalidusername(s3service::s3user& userResponse);
	bool isvalidaccname(s3service::s3account& accountResponse);
	bool isvalidbucketname(s3service::s3bucket& bucketResponse);
	bool isvalidbucketnameObj(s3service::s3object& objectResponse);
	bool isvalidMarkerId(s3service::s3object& objectResponse);
	bool isvalidObjectname(s3service::s3object& objectResponse);
	~credential();
	
};
