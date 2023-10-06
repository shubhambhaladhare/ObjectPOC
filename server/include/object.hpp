#include <string>
#include <vector>
#include <fstream>
#include <chrono>
//////////////////////////
#include <fcntl.h>



#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <bits/stdc++.h>
#include <iostream>
#include <cstdint>
#include <boost/filesystem.hpp>


///////////////////////
#include "EntityBase.h"
#include "s3service.pb.h"
#include "serversocket.hpp"
#include "mssql.hpp"

//////////////////////
#include <sstream>
#include <openssl/sha.h>
#include <bits/stdc++.h>

#include <unistd.h>


#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <cstdlib>


#include <crypto++/cryptlib.h>

#include <crypto++/base64.h>
#include <crypto++/hex.h>
#include <crypto++/rsa.h>
#include <crypto++/aes.h>
#include <crypto++/files.h>
#include <crypto++/randpool.h>
#include <crypto++/validate.h>
#include <crypto++/modes.h>
#include <crypto++/osrng.h>
#include <crypto++/seckey.h>

#include <iostream>

#include <chrono>
#include <ctime>



using namespace std;



class object : public EntityBase
{	
   	 std::string m_objectId, m_bucketId, m_objectName, m_creationTimestamp, m_error, m_versionStatus, m_versionId,m_versionId1,	m_fileSize,m_unique_multipart_id, m_bucketName, m_objectNameToCheck, m_markerId, m_filesize, m_key,m_retentionDate, m_retentionDuration;
    	std::vector<char> m_data;
    	bool m_bExists,m_objectlocking;

    // create table Msys_Objects(objectId varchar(255) PRIMARY KEY, objectName varchar(255) NOT NULL, creationTimestamp varchar(255) NOT NULL, bucketId varchar(255) FOREIGN KEY REFERENCES Msys_Buckets(bucketId))

    	auto readFromFile(serverSocket& ssWrite, const std::string objectPath);

	bool writeToFile(serverSocket& ss, s3service::s3object& objectRequest);
	bool writeToFileformultipartupload(serverSocket& ssRead, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d);
    	void setClassVariables(database& d);
    	void setClassVariablesBasedonVersionId(database& d);
    	void setClassVariablesBasedonDeleteMarkerId(database& d);
    	static std::string generateVersionId(const std::string& filename);

//    GUID getUploadId(const std::string& objectKey);
//
//    std::string getMd5Checksum(const std::string& data);
//
//	std::string getETag();
//
//	std::string getFinalETag(std::vector<std::string>& etags);
//
//	std::string getObjectKey();
	
	bool isObjectPresent();
	std::string setvariable(database& d);
	inline void setData(const std::vector<char>& data);
	void retriveDeletemarker(string objectPath, string m_bucketName);
	std::string set(database& d);
    public:
    	int filecount(std::string path, s3service::s3object& objectResponse);
    	long long int sizebyte() ;
	object(const std::string& bucketId, const std::string& objectName, database& d);
	object(const std::string& bucketId,database& d,const std::string versionId);
	object(database& d, const std::string bucketId, const std::string markerId);
	object(database& d, const std::string objectName);
	void putObject(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d,std::string BucketVersion,std::string bucketname, std::string accesskey, std::string secretkey);

	void ListdeleteMarkerObject(s3service::s3object& objectResponse, database& d, std::string BucketVersion);
	void deleteObject(database& d,s3service::s3object& objectResponse,std::string bucketname, std::string BucketVersion, string accesskey, string secretkey);

	void downloadObject(serverSocket& ssWrite, s3service::s3object& objectResponse, std::string bucketname, database& d);
	//void object::bucketName(s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d)
	
	void downloadObjectByVersion(serverSocket& ssWrite, s3service::s3object& objectResponse, std::string bucketname);
	
	void getObjectProperties(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d);
	void initMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d);
	void putMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse,std::string bucketname, database& d, std::string bucketversion);
	void completeMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, string bucketName, std::string BucketVersion, database& d);
	void abortMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d);
	void listMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d);
	long getFileSize(std::string filename);
	//int uploadFile(serverSocket& ss, string file_name);
	int downloadFile(serverSocket& ss, string file_name, unsigned long long bytes, std::string bucketname, s3service::s3object& objectResponse, std::string BucketVersion, database& d);
	void nameObject(const std::string& filename,s3service::s3object& objectResponse,std::string BucketVersion);
	void readDeletemarker(string objectPath, string m_bucketName);
	void getDeleteMarker(s3service::s3object& objectResponse, database& d);
	void putmultipart(serverSocket& ssRead, s3service::s3object& objectService,s3service::s3object& objectResponse,database& d);
	string getBucketVersion();

	string getretentionduration();
	
void putObjectLock(s3service::s3object& objectResponse, database& d);
void setObjectRetention(s3service::s3object& objectResponse, string retentionDate, database& d);
	
	// bool copyObject(wstring targetBucket);

	// GUID createMultipartUpload();

	// std::string uploadPart(const std::string& body, long objectSize, GUID uploadId, int partNumber, std::string checksum);

	// std::vector<std::string> completeMultipartUpload(std::string body, const GUID uploadId);
	
	// bool listParts(int maxParts, int partNumber);
	
	// bool listMultipartUploads(int maxParts, const int partNumber);
	
	// bool abortMultipartUpload(const int partNumber);

//	bool getObjectTagging(std::string versionID = "dummy value");
//
//	bool putObjectTagging(wstring checksum, wstring versionID = "dummy value");

	inline std::string getObjectName();

	inline std::string getBucketId();

	inline std::string getObjectId();

	inline std::vector<char> getData();

	inline std::string getCreationTimestamp();

	inline bool doesObjectExists();

	std::string getErrorMessage();

    ~object();    
};
