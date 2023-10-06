#include <string>
#include <map>

#include "s3service.pb.h"
#include "EntityBase.h"
#include "mssql.hpp"

#include <experimental/filesystem>



#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <cstdint>
#include <boost/filesystem.hpp>

#include <unistd.h>
#include <stdio.h>

#include <string>
#include <termios.h>
#include <typeinfo>
#include <ctype.h>








#include <fcntl.h>


#include <cstdlib>
#include <cstring>
#include <fstream>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include<vector>
#include <sstream>
#include<signal.h>

class bucket : public EntityBase
{
	std::string  m_accountId, m_error, m_arn, m_creationTimestamp, m_bucketId, m_bucketVersion, m_memory;
	std::string m_bucketName, m_bucketNameCheck;
	std::string archive_Enable;
	std::map<std::string, std::string> m_tags;
	bool m_bExists;

    void setClassVariables(database& d);

    void setTags(std::map<std::string, std::string>& m);

    public:
    	long long int sizebyte();
	
	bucket(const std::string& bucketName, const std::string& accountId, database& d);
	
	void createBucket(s3service::s3bucket& bucketResponse, database& d);
	
	void deleteBucket(s3service::s3bucket& bucketResponse, database& d);
	
	// void putBucketPolicy(string bucketName,char *accessId, string bucketAccess = "true", string policyContent);
	
	// string getBucketPolicy(string bucketName, char* accountId);
	
	// string putBucketVersioning(string bucketName, char* accountId);
	
	// string getBucketVersioning(string bucketName,char* accoundId);
	
	void putBucketTags(std::map<std::string, std::string>& tags, s3service::s3bucket& bucketResponse,database& d);
	
	void listBucketTags(s3service::s3bucket& bucketResponse);

	void deleteBucketTags(s3service::s3bucket& bucketResponse,database& d);
	void put_Bucket_Version(s3service::s3bucket& bucketResponse,database& d);
	void get_Bucket_Version(s3service::s3bucket& bucketResponse,database& d);
	void list_object_version(s3service::s3bucket& bucketResponse,database& d);
	void list_object(s3service::s3bucket& bucketResponse,database& d);
	void set_lifecycle_rule(s3service::s3bucket& bucketResponse,database& d, std::string  expire_date);
	void set_storageQuota(s3service::s3bucket& bucketResponse,database& d, std::string memoryspace);
	void get_storagelens(s3service::s3bucket& bucketResponse,database& d);
	std::string getBucketName();

    	std::string getBucketId();

    	std::string getAccountId();

    	std::string getArn();
    
    	std::string getCreationTimestamp();

	std::string getErrorMessage();

	bool doesBucketExists();
	std::string getarchive();
	std::map<std::string, std::string> getTags();
	std::string getBucketVersion();
	
	virtual ~bucket();
	
	
};
