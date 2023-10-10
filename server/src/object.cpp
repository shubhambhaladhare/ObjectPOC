#include "object.hpp"

auto object::readFromFile(serverSocket& ssWrite, const std::string objectPath)
{

	std::string current_path = get_current_dir_name();
	std::string full_path = current_path+"/upload/"+  m_bucketName + "/";
	std::string obj = full_path+objectPath;
	//cout << objectPath << endl;
	
	//scout << obj << endl;
    	/*size_t extension_index = objectPath.find_last_of(".");
    	cout << extension_index << endl;	    
	std::string extension;    
	if (extension_index != std::string::npos)
	{
		extension = objectPath.substr(extension_index);
	}
	cout << extension << endl;
    	std::string p = full_path + "decrypt" + extension;
    	//cout << p.c_str() << endl;
    	remove(p.c_str());
    	
    	using namespace CryptoPP;

    	unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
    	unsigned char *iv =  (unsigned char *)"0123456789012345";
    	size_t key_size = strlen((const char*)key);

	CBC_Mode<AES>::Decryption  decryptor(key, key_size, iv);
	cout << obj.c_str() << endl;
	cout << p.c_str() << endl;
    	FileSource(obj.c_str(), true, new StreamTransformationFilter(decryptor, new FileSink(p.c_str())));
	obj = full_path + "decrypt" + extension;
	cout << obj << endl; */
	
	const int BUFFER_SIZE = 50000;
	char* buffer = new char[BUFFER_SIZE];
	int bytesRead;
	sleep(3);

	std::ifstream iF(obj, std::ios::in | std::ios::binary);
	//cout << obj << endl; 

	if (!iF)
	{
		std::cout << "File not found!";
		return false;
	}

	iF.seekg(0, std::ios::end);
	size_t size = iF.tellg();
	iF.seekg(0, std::ios::beg);

	/*char* data = new char[size];
	int bytesRead;
	
	int th3stop = 0;
	
	iF.read(data, size);*/
	int t =0;
	int a = 0;
	memset(buffer, 0, BUFFER_SIZE);
	while (iF && (bytesRead = iF.readsome(buffer, BUFFER_SIZE)) > 0) 
	{
        	
        	
        	auto ret = ssWrite.writeSocket(buffer, BUFFER_SIZE);
        	
        	if (ret != "")
		{
			std::cout << ret << std::endl;
			//return -1;
			
		}
		//std::cout<<t<<std::endl;
		//t++;
		a+=bytesRead;
		
		//std::cout<<(a)<<std::endl;
		memset(buffer, 0, BUFFER_SIZE);
		
        }
        delete buffer;
        //cout << "Out of while loop" << endl;
	/*while (iF && (bytesRead = iF.readsome(data, size)) > 0) 
	{
        	if(th3stop == 0)
        	{
        		auto ret = ssWrite.writeSocket(data, size);
        		if (ret != "")
			{
				std::cout << ret << std::endl;
				//return -1;
			
			}
			std::cout<<t<<std::endl;
			t++;
			a += bytesRead;
			std::cout<<(a)<<std::endl;
			memset(data, 0, size);
		}
        }*/
    	/*char END_MARKER[] = "!!!EOF!!!";
    	auto ret = ssWrite.writeSocket(END_MARKER, sizeof(END_MARKER));
    	if (ret != "")
	{
		std::cout << ret << std::endl;
	}*/
	
	/*auto ret = ssWrite.writeSocket(data, size);

	if (ret != "")
	{
		std::cout << ret << std::endl;
		return false;
	}*/

	//objResponse.set_len(size);
	iF.close();
	//remove(obj.c_str());
	//delete in;
	//delete in2;
	return true;
}

auto readObject(serverSocket& ssWrite, const std::string path, s3service::s3object& objectResponse)
{
	std::ifstream iF(path, std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
		return false;
	}

	iF.seekg(0, std::ios::end);
	size_t size = iF.tellg();
	iF.seekg(0, std::ios::beg);

	char* data = new char[size];

	iF.read(data, size);
	auto ret = ssWrite.writeSocket(data, size);

	if (ret != "")
	{
		std::cout << ret << std::endl;
		return false;
	}

	objectResponse.set_len(size);

	iF.close();
	return true;
}

long long int object::sizebyte() {
	std::string current_path= get_current_dir_name();
	std::string full_path=current_path+"/upload/"+  m_bucketName;
  	DIR *d = opendir(full_path.c_str());
  	full_path=current_path+"/upload/"+  m_bucketName + "/";
	
  if( d == NULL ) {
    fprintf( stderr, "Cannot open current working directory\n" );
    return 0;
  }

  struct dirent *de;
  struct stat buf;

  long long int total = 0;
  for( de = readdir( d ); de != NULL; de = readdir( d ) ) {
    string object = de->d_name;
    std::string path = full_path + object;//+ std::to_string(de->d_name.c_str());
    
    std::ifstream iF(path, std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
	}

	iF.seekg(0, std::ios::end);
	size_t file_size = iF.tellg();
    total += file_size;
  }

  closedir( d );

  return total;
}




void object::readDeletemarker(string objectPath, string m_bucketName)
{
	std::string current_path = get_current_dir_name();
	std::string full_path =current_path+"/upload/"+  m_bucketName + "/";
	std::string obj = full_path + objectPath;
	//cout << obj << endl;
	std::ifstream iF(obj, std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
		return;
	}

	iF.seekg(0, std::ios::end);
	size_t size = iF.tellg();
	iF.seekg(0, std::ios::beg);

	char* data = new char[size];	
	iF.read(data, size);
	
	current_path= get_current_dir_name();
	full_path=current_path+"/deletedfile/"+ m_bucketName + "/";
	
	boost::filesystem::create_directories(full_path);
	std::string m_object_N = full_path + objectPath;
	//cout << full_path << endl;
	
	std::ofstream oF(m_object_N.c_str(), std::ios::out | std::ios::binary);
	
	oF.write(data, size);
    	oF.close();
}
void object::nameObject(const std::string& filename,s3service::s3object& objectResponse,std::string BucketVersion){
    
    std::string new_filename=filename;
    if(BucketVersion=="Enable"){
    
        // get the extension of the file
            size_t extension_index = filename.find_last_of(".");
            
            std::string extension;
            
            if (extension_index != std::string::npos)
            {
            	extension = filename.substr(extension_index);
            }
        
        // create the new filename with extension
        std::ostringstream oss;
        
            oss << filename.substr(0, (extension_index-21)) << extension;
            
            new_filename = oss.str();
            
            objectResponse.set_objectname(new_filename);
        }
        else
        {
            	objectResponse.set_objectname(new_filename);
        }
}
bool object::writeToFileformultipartupload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d)
{


	std::vector<std::vector<std::string>> re;
	int retcode1 = d.executeQuery("select bucketName from Msys_Buckets where bucketId = '" + m_bucketId +"'",re);
	
	if (retcode1 != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return false ;
	}
	
	if(re.empty())
	{
		std::cout << "result is empty" << std::endl;
		m_error = "result is empty";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		return false;
	}
	
	if(re[0][0] == "NULL")
	{
		std::cout << "result is NULL" << std::endl;
		m_error = "result is NULL";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);	
		return false;
	}
	std::string current_path = get_current_dir_name();
	std::string full_path = current_path + "/" + re[0][0] + "/" + m_objectName;
	std::string m_object_N = full_path;
	
	
	std::ofstream oF(m_object_N.c_str(), std::ios::out | std::ios::binary);
	auto ret = ss.readSocket(m_data, objectRequest.len());
	if (ret != "")
	{
		std::cout << ret << std::endl;
		return false;
	}
	
    	oF.write(&m_data[0], objectRequest.len());
    	oF.close();
    	/*std::ifstream iF(m_object_N.c_str(), std::ios::in | std::ios::binary);

	if (!iF)
	{
		m_error = "File not found!";
		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Object");
		err->set_errorcode("File not found");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		std::cout << "File not found!";
		return false;
	}
	iF.close();*/
	return true;
}

bool object::writeToFile(serverSocket& ss, s3service::s3object& objectRequest)
{
	int size = 0;
	std::string current_path = get_current_dir_name();
	std::string full_path = current_path+"/upload/"+ m_bucketName + "/";
	size_t extension_index = m_objectName.find_last_of(".");
			    
	std::string extension;
		    
	if (extension_index != std::string::npos)
	{
		extension = m_objectName.substr(extension_index);
	}
		
	
	boost::filesystem::create_directories(full_path);
	std::string m_object_N = full_path + "temp" + extension ;
	cout << m_object_N << endl;
	
	std::ofstream oF(m_object_N.c_str(), std::ios::out | std::ios::binary);

	auto ret = ss.readSocket(m_data, objectRequest.len());
	

	if (ret != "")
	{
		std::cout << ret << std::endl;
		return false;
	}

    	oF.write(&m_data[0], objectRequest.len());
    	oF.close();
    	std::ifstream iF(m_object_N.c_str(), std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
		return false;
	}
	iF.seekg(0, std::ios::end);
	int siz=iF.tellg();
	ostringstream str1;
	str1 << siz;
	m_fileSize = str1.str();
	
	m_fileSize.append("_bytes");
	iF.close();
	using namespace CryptoPP;
	std::string full_path1 = current_path+ "/upload/" + m_bucketName + "/" + m_objectName ;
    	unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
    	unsigned char *iv =  (unsigned char *)"0123456789012345";
    	size_t key_size = strlen((const char*)key);

    	CBC_Mode<AES>::Encryption      encryptor(key, key_size, iv);

    	FileSource(m_object_N.c_str(), true, new StreamTransformationFilter(encryptor, new FileSink(full_path1.c_str())));
	remove(m_object_N.c_str());

    return true;
}
string object::getBucketVersion()
{
	return m_versionStatus;
}



string object::getretentionduration() {
    return m_retentionDate;
}

void object::setClassVariables(database& d)
{
	std::vector<std::vector<std::string>> result;

	int retcode = d.executeQuery("Select * from  Msys_Objects where objectName = '" + m_objectName + "' AND bucketId = '" + m_bucketId + "'", result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}

	if (result.empty())
	{
		m_bExists = false;
		m_objectNameToCheck = "";
		m_objectId = "";
		m_creationTimestamp = "";
		m_objectlocking=false;
		 m_retentionDate="";
		m_data = std::vector<char>();
	}
	else
	{
		m_bExists = true;

		m_objectId = result[0][0];
		m_versionStatus = result[0][1];
		m_objectNameToCheck = result[0][2];
		m_creationTimestamp = result[0][5];
//		m_objectlocking = (result[0][6] == "true") ? true : false;

		m_objectlocking=(result[0][6]=="true");
		m_retentionDate = result[0][7];

		m_data = std::vector<char>();
	}
}


void addErrorInfo(s3service::s3object& objectResponse, const std::string& errorType, const std::string& errorCode, const std::string& errorMessage, const std::string& key = "") {
    auto err = objectResponse.add_errorinfo();
    err->set_errortype(errorType);
    err->set_errorcode(errorCode);
    if (!key.empty()) {
        err->set_key(key);
    }
    err->set_errormessage(errorMessage);
}





inline void object::setData(const std::vector<char>& data)
{
    m_data = data;
}

object::object(const std::string& bucketId, const std::string& objectName, database& d)
{
    m_bucketId = bucketId;
    m_objectName = objectName;
    m_bExists = false;

    setClassVariables(d);
}
void object::setClassVariablesBasedonVersionId(database& d)
{
	std::vector<std::vector<std::string>> result;

	int retcode = d.executeQuery("Select * from  Msys_Objects where bucketId = '" + m_bucketId + "' AND objectVersion = '" + m_versionId + "'", result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}

	if (result.empty())
	{
		m_bExists = false;
		m_versionStatus = "";
		m_objectId = "";
		m_creationTimestamp = "";
		m_data = std::vector<char>();
		m_objectName = "";
		m_objectlocking=false;
		 m_retentionDate="";
	}
	else
	{
		m_bExists = true;

		m_objectId = result[0][0];
		m_versionStatus = result[0][1];
		m_creationTimestamp = result[0][5];
		m_objectName = result[0][2];
		m_versionId1 = result[0][4];
	//	m_objectlocking=(result[0][6]==true);
	       //  m_objectlocking=(result[0][6]=="true");
	       //
	      m_objectlocking = (result[0][6] == "true") ? true : false;

	       m_retentionDate = result[0][7];

		m_data = std::vector<char>();
	}
}

object::object(const std::string& bucketId,database& d,const std::string versionId)
{
	m_bucketId = bucketId;
   	m_versionId = versionId;
    	m_bExists = false;
//	m_objectlocking= true;
    	setClassVariablesBasedonVersionId(d);
}

std::string object::generateVersionId(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string contents = buffer.str();

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(contents.c_str()), contents.length(), hash);

    std::stringstream versionId;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        versionId << std::hex << static_cast<int>(hash[i]);
    }

    return versionId.str();
}

void object::putObject(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d,std::string BucketVersion,std::string bucketname, std::string accesskey, std::string secretkey)
{
        
	m_bucketName = bucketname;
//	m_objectlocking=false;
	std::vector<std::vector<std::string>> re;
	double kb = 0;
	int retcode1 = d.executeQuery("select Memory_space_IN_bytes from Msys_Buckets where bucketName = '" + m_bucketName +"'",re);
	
	if (retcode1 != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}
	
	if(re[0][0] != "NULL")
	{
		std::string inkb = re[0][0];
		stringstream m;
		m << inkb;
		m >> kb;
	}
	std::string key = to_string((rand()%10)+1);
	
	if(BucketVersion == "Enable")
	{
		size_t extension_index = m_objectName.find_last_of(".");
	    	string extension;
	    	if (extension_index != string::npos) {
			extension = m_objectName.substr(extension_index);
	    	}
	    	std::vector<std::vector<std::string>> result;
	    	m_creationTimestamp = getCurrentTimeStamp();
	    	
		std::ostringstream oss;
	    	oss << m_objectName.substr(0, extension_index) << "_" << m_creationTimestamp << extension;
	    	m_objectName = oss.str();
	    	
		std::string current_path = get_current_dir_name();
		std::string full_path = current_path + "/upload/" + m_bucketName + "/";
	    	std::string m_object_N = full_path + m_objectName;
	    	
		if (m_bExists)
	   		{
	   			
				writeToFile(ss, objectRequest);
				long long int filesizeinbytes = sizebyte();
				//cout << filesizeinbytes << endl;
				//cout << kb << endl;
				if(filesizeinbytes > kb  && re[0][0] != "NULL")
				{
					std::string current_path= get_current_dir_name();
					std::string full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
					//cout << m_objectName << endl;
					int ret = remove(full_path.c_str());
					m_error = "Size is full for this bucket please update the bucket size";
					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Object");
					err->set_errorcode("Size is full");
					err->set_key(m_objectName);
					err->set_errormessage(m_error);
					cout << "Size is full" << endl;
					return;
				}
				
				//cout << m_object_N << endl;
				
				m_versionId = "versionId" + getUniqueId();
				//m_versionId = object::generateVersionId(m_object_N);
	
				m_objectId = "object" + getUniqueId();
			
			       int retcode = d.executeQuery("Insert into Msys_Objects (objectId,VersionStatus, objectName, objectsize, objectVersion, creationTimestamp, bucketId,objectLocking,objectRetention) " \
				    			"values "\
"('"+ m_objectId +"', '"+ BucketVersion +"', '"+ m_objectName +"', '"+  m_fileSize +"', '"+ m_versionId +"', '"+ m_creationTimestamp +"', '"+ m_bucketId +"','"+ std::to_string(m_objectlocking) +"','"+ m_retentionDate +"')");
				if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed 1" << std::endl;
					return;
				}
			}
		else 
		{
				writeToFile(ss, objectRequest);
				long long int filesizeinbytes = sizebyte();
				//cout << filesizeinbytes << endl;
				//cout << kb << endl;
				if(filesizeinbytes > kb && re[0][0] != "NULL")
				{
					std::string current_path= get_current_dir_name();
					std::string full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
					//cout << m_objectName << endl;
					int ret = remove(full_path.c_str());
					m_error = "Size is full for this bucket please update the bucket size";
					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Object");
					err->set_errorcode("Size is full");
					err->set_key(m_objectName);
					err->set_errormessage(m_error);
					cout << "Size is full" << endl;
					return;
				}
				m_versionId = "versionId" + getUniqueId();
				//m_versionId = object::generateVersionId(m_object_N);
				m_bExists = true;
				m_objectId = "object" + getUniqueId();
				int retcode = d.executeQuery("Insert into Msys_Objects " \
				    			"(objectId, VersionStatus,objectName,objectsize, objectVersion, creationTimestamp, bucketId,objectLocking,objectRetention) " \
				    			"values " \
"('"+ m_objectId +"', '"+ BucketVersion +"', '"+ m_objectName +"','"+ m_fileSize +"' , '"+ m_versionId +"' , '"+ m_creationTimestamp +"', '"+ m_bucketId +"','"+ std::to_string(m_objectlocking) +"','"+ m_retentionDate +"')");


	    			if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed 2" << std::endl;
					return;
				}
		}

	    			
	    }
		
	
	
	else if(BucketVersion == "Disable")
	{

	

	if (m_bExists)
   		{
			writeToFile(ss, objectRequest);
			long long int filesizeinbytes = sizebyte();
			if(filesizeinbytes > kb && kb != 0)
			{
				std::string current_path= get_current_dir_name();
				std::string full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
				int ret = remove(full_path.c_str());
				m_error = "Size is full for this bucket please update the bucket size";
				auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("Size is full");
				err->set_key(m_objectName);
				err->set_errormessage(m_error);
				cout << "Size is full" << endl;
				return;
			}
			m_creationTimestamp = getCurrentTimeStamp();
			
			int retcode = d.executeQuery("Update Msys_Objects SET creationTimestamp =  '" + m_creationTimestamp  + "' where bucketId = '" + m_bucketId + "' AND objectId = '" + m_objectId + "'");

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed" << std::endl;
				return;
			}
		}
		else
		{
			writeToFile(ss, objectRequest);
			long long int filesizeinbytes = sizebyte();
			//cout << filesizeinbytes << endl;
			//cout << kb << endl;
			if(filesizeinbytes > kb && kb != 0)
			{
				std::string current_path= get_current_dir_name();
				std::string full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
				int ret = remove(full_path.c_str());
				
				m_error = "Size is full for this bucket please update the bucket size";
				auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("Size is full");
				err->set_key(m_objectName);
				err->set_errormessage(m_error);
				cout << "Size is full" << endl;
				return;
			}
				std::string current_path = get_current_dir_name();
				std::string full_path = current_path + "/upload/" + m_bucketName + "/";
				std::string m_object_N = full_path + m_objectName;
				//cout << m_object_N << endl;
				m_versionId = object::generateVersionId(m_object_N);
	
			std::vector<std::vector<std::string>> result11;
	int recode123 = d.executeQuery("select o.CommonId,o.objectName from Msys_Buckets b JOIN Msys_Access_Keys a on a.accountId = b.accountId JOIN Msys_Objects o ON b.bucketId = o.bucketId where o.CommonId = '" + m_versionId +"' AND a.accessKeyId = '" + accesskey + "' AND a.secretKey = '" + secretkey +"' ", result11);
	//cout << recode123 << endl;
	if (recode123 != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
		
	}	if(!result11.empty() ){
	std::string bucket1;
	//cout << result11[0][0] << result11.size() << endl;
			if(result11[0][0] != "NULL")
			{
				char* data = NULL;
				int i =0;
				for(; i< result11.size();i++)
				{
					//cout << result11[i][1] << endl;
					if(result11[i][0] == m_versionId && result11[i][1] == m_objectName)
					{
						vector<vector<string>> result1;
						int recode = d.executeQuery("select b.bucketName, o.objectsize from Msys_Objects o JOIN Msys_Buckets b ON b.bucketId= o.bucketId where CommonId = '"+ m_versionId +"'" , result1);
	
						if (recode != 0)
						{
							std::cout << "ExecuteQuery Failed" << std::endl;
							return;
						}
							std::string current_path= get_current_dir_name();
							bucket1 = result1[0][0];
							std::string full_path = current_path+"/upload/"+  result1[0][0] + "/";
							std::string obj = full_path+m_objectName;
							//cout << obj << endl;
							std::ifstream iF(obj, std::ios::in | std::ios::binary);

							if (!iF)
							{
								//std::cout << "File not found!" << endl;
								iF.close();
								std::string current_path= get_current_dir_name();
								std::string obj = current_path+"/upload/" + m_objectName;
								//cout << obj << endl;
								std::ifstream iF1(obj, std::ios::in | std::ios::binary);
								
								if (!iF1)
								{
									std::cout << "File not found!" << endl;
									return;
								}
								iF1.seekg(0, std::ios::end);

								size_t size = iF1.tellg();
							
								iF1.seekg(0, std::ios::beg);
								data = new char[size];

								iF1.read(data, size);
								iF1.close();

							}


							if(iF)
							{
								iF.seekg(0, std::ios::end);

								size_t size = iF.tellg();

								iF.seekg(0, std::ios::beg);
								data = new char[size];
								iF.read(data, size);
								iF.close();
						       }
							current_path= get_current_dir_name();
							full_path = current_path+"/upload/";
							
							std::string m_object_N = full_path+ m_objectName;
							//cout << m_object_N << endl;
							//cout << full_path << endl;
							
							std::ofstream oF(m_object_N.c_str(), std::ios::out | std::ios::binary);

						    	oF.write(data, objectRequest.len());
						    	oF.close();
						    	recode = d.executeQuery("update Msys_objects set path= '"+ m_object_N + "'where CommonId = '"+ m_versionId + "'" , result1);
	
						if (recode != 0)
						{
							std::cout << "ExecuteQuery Failed" << std::endl;
							return;
						}
						
						/*recode = d.executeQuery("update Msys_objects set CommonId= 'NULL' where bucketId = '"+ result1[0][2] + "'" , result1);
	
						if (recode != 0)
						{
							std::cout << "ExecuteQuery Failed" << std::endl;
							return;
						}*/
						
						
						m_objectId = "object" + getUniqueId();
						m_fileSize = result1[0][1];
						m_creationTimestamp = getCurrentTimeStamp();
						//cout << m_objectId << "/n" << BucketVersion << "/n" << m_objectName << "/n" << m_fileSize << "/n" << m_creationTimestamp << "/n" << m_object_N << "/n" << m_bucketId << endl;
						int recode1 = -1;
						
					       recode1 = d.executeQuery("Insert into Msys_Objects " \
				    			"(objectId, VersionStatus,objectName,objectsize,  creationTimestamp, path, bucketId,objectLocking,objectRetention) " \
				    			"values " \
"('"+ m_objectId +"', '"+ BucketVersion +"', '"+ m_objectName +"','"+ m_fileSize +"' ,  '"+ m_creationTimestamp +"', '" + m_object_N +"', '"+ m_bucketId +"','"+ std::to_string(m_objectlocking) +"','"+ m_retentionDate +"')");
						if (recode1 != 0)
						{
							std::cout << "ExecuteQuery Failed" << std::endl;
							return;
						}
						current_path= get_current_dir_name();
						full_path = current_path + "/upload/" + bucket1 + "/" + m_objectName;
						int ret = remove(full_path.c_str());
						
						current_path = get_current_dir_name();
						full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
						ret = remove(full_path.c_str());
						if (ret != 0)
						{
							std::cout << "Error in removing file" << std::endl;
							return; 
						}
						/*if (ret != 0)
						{
							std::cout << "Error in removing file "  << bucket1 << std::endl;
						}*/
						
					}

					
				}
			if(result11[i-1][0] != "NULL" && result11[i-1][1] != m_objectName)
			{
				m_creationTimestamp = getCurrentTimeStamp();
				m_objectId = "object" + getUniqueId();
				//m_versionId = object::generateVersionId(m_object_N);
				//cout << m_versionId << endl;

				int retcode = d.executeQuery("Insert into Msys_Objects " \
				    			"(objectId, VersionStatus,objectName, objectsize, creationTimestamp, CommonId, bucketId,objectLocking,objectRetention) " \

				    			"values " \
	"('"+ m_objectId +"', '"+ BucketVersion +"', '"+ m_objectName +"','"+ m_fileSize +"','"+ m_creationTimestamp +"', '"+ m_versionId +"', '"+ m_bucketId +"','"+ std::to_string(m_objectlocking) +"','"+ m_retentionDate +"' )");

	    			if (retcode != 0)

				{

					std::cout << "ExecuteQuery Failed" << std::endl;

					return;

				}
			}	
				return;
				
			}
		}
			m_bExists = true;
			m_creationTimestamp = getCurrentTimeStamp();
			m_objectId = "object" + getUniqueId();
			//m_versionId = object::generateVersionId(m_object_N);
			//cout << m_versionId << endl;

			int retcode = d.executeQuery("Insert into Msys_Objects " \
			    			"(objectId, VersionStatus,objectName, objectsize, creationTimestamp, CommonId, bucketId,objectLocking,objectRetention) " \

			    			"values " \
"('"+ m_objectId +"', '"+ BucketVersion +"', '"+ m_objectName +"','"+ m_fileSize +"','"+ m_creationTimestamp +"', '"+ m_versionId +"', '"+ m_bucketId +"','"+ std::to_string(m_objectlocking) +"','"+ m_retentionDate +"')");

    			if (retcode != 0)

			{

				std::cout << "ExecuteQuery Failed" << std::endl;

				return;

			}

    			
    			}
		}


	
}
    


object::object(database& d, std::string bucketId, std::string markerId)
{
	m_bucketId = bucketId;
	m_markerId = markerId;
	setClassVariablesBasedonDeleteMarkerId(d);
}

void object::setClassVariablesBasedonDeleteMarkerId(database& d)
{
	std::vector<std::vector<std::string>> result;

	int retcode = d.executeQuery("Select * from  Msys_DeleteMarker where deleteMarkerID = '" + m_markerId + "' AND bucketId = '" + m_bucketId + "'", result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}

	if (result.empty())
	{
		m_bExists = false;
	}
	else
	{
		m_bExists = true;
	}
}

void object::retriveDeletemarker(string objectPath, string m_bucketName)
{
	std::string current_path = get_current_dir_name();
	std::string full_path =current_path+"/deletedfile/"+  m_bucketName + "/";
	std::string obj = full_path + objectPath;
	//cout << obj << endl;
	std::ifstream iF(obj, std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
		return;
	}

	iF.seekg(0, std::ios::end);
	size_t size = iF.tellg();
	iF.seekg(0, std::ios::beg);

	char* data = new char[size];	
	iF.read(data, size);
	
	current_path= get_current_dir_name();
	full_path=current_path+"/upload/"+ m_bucketName + "/";
	
	boost::filesystem::create_directories(full_path);
	std::string m_object_N = full_path + objectPath;
	//cout << full_path << endl;
	
	std::ofstream oF(m_object_N.c_str(), std::ios::out | std::ios::binary);
	
	oF.write(data, size);
    	oF.close();
}

void object::getDeleteMarker(s3service::s3object& objectResponse, database& d)
{
	if((m_bExists == true))
	{
		std::vector<std::vector<std::string>> result;
		int retcode = d.executeQuery("Select bucketName, objectId, bucketId from  Msys_DeleteMarker where deleteMarkerID = '" + m_markerId + "' AND bucketId = '" + m_bucketId + "'", result);

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed in Msys_DeleteMarker" << std::endl;
				m_error = "ExecuteQuery Failed in Msys_DeleteMarker.";
				auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("Nosuch object in DeleteMarker");
				err->set_key(m_objectName);
				err->set_errormessage(m_error);
				return;
			}
		
		for(int i = 0; i < result.size(); i++)
		{
			string bucketName = result[i][0];
			string objId = result[i][1];
			string bucketId = result[i][2];
			
			
			
		std::vector<std::vector<std::string>> result1;
		int retcode = d.executeQuery("Select * from  Msys_DeleteMarker where objectId = '" + objId + "' AND bucketId = '" + bucketId + "'", result1);
			//string objId = result1[0][0];
			string versionstatus = result1[0][1];
			string objName = result1[0][2];
			string objsize = result1[0][3];
			string versionId = result1[0][4];
			string creationTime = result1[0][5];
//			bool objectlocking =(result1[0][6]=="true");
			//string bucketId = result1[0][6];

			retcode = d.executeQuery("Insert into Msys_Objects " \
			    			"(objectId, VersionStatus,objectName, objectsize, objectVersion,creationTimestamp, bucketId,objectLocking,objectRetention) " \
			    			"values " \
"('"+ objId +"', '"+ versionstatus +"', '"+ objName +"', '"+ objsize +"', '" + versionId +"', '"+ creationTime +"', '"+ bucketId +"','"+ std::to_string(m_objectlocking) +"','"+ m_retentionDate +"')");


			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed in Msys_DeleteMarker" << std::endl;
				m_error = "ExecuteQuery Failed in deleting Msys_DeleteMarker.";
				auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("Nosuch object in deleting DeleteMarker");
				err->set_key(m_markerId);
				err->set_errormessage(m_error);
				return;
			}
			retriveDeletemarker(objName, bucketName);
			std::string current_path= get_current_dir_name();
			std::string full_path = current_path + "/deletedfile/" + bucketName + "/" + objName;
			int ret = remove(full_path.c_str());
			if (ret != 0)
			{
				std::cout << "Error in removing file delete file in for loop" << std::endl;
			}
			
			retcode = d.executeQuery("DELETE from Msys_DeleteMarker Where  objectId = '" + objId + "' AND bucketId = '" + bucketId + "'");

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed in Msys_DeleteMarker" << std::endl;
				m_error = "ExecuteQuery Failed in deleting Msys_DeleteMarker.";
				auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("Nosuch object in deleting DeleteMarker");
				err->set_key(m_markerId);
				err->set_errormessage(m_error);
				return;
			}
		}
		
	}
		else
	{
		m_error = "The specific object does not exists.";
		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Object");
		err->set_errorcode("Nosuch object in DeleteMarker");
                err->set_key(m_objectName);
		err->set_errormessage(m_error);
	}
}

object::object(database& d, std::string objectName)
{
    	m_objectName = objectName;
    	m_objectId = set(d);
}

std::string object::set(database& d)
{
	std::vector<std::vector<std::string>> result;
	int retcode = d.executeQuery("select objectId from Msys_DeleteMarker where objectName = '" + m_objectName + "'", result);
	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed for Currentversion" << std::endl;
		return "";
	}
	if(result.empty())
	{
		return "";
	}
	return result[0][0];
}
std::string object::setvariable(database& d)
{
	std::vector<std::vector<std::string>> result;
	int retcode = d.executeQuery("select VersionStatus from Msys_DeleteMarker where objectName = '" + m_objectName + "' AND bucketId = '" + m_bucketId + "'", result);
	if(result.empty())
	{
		return "";
	}
	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed for Currentversion" << std::endl;
		return "";
	}
	return result[0][0];
}
void object::ListdeleteMarkerObject(s3service::s3object& objectResponse, database& d, std::string BucketVersion)
{
	//std::string BucketVersion = setvariable(d);
	if(BucketVersion == "Enable")
    	{
    		std::vector<std::vector<std::string>> result;
      
		size_t extension_index = m_objectName.find_last_of(".");
			    
		std::string extension;
		    
		if (extension_index != std::string::npos)
		{
			extension = m_objectName.substr(extension_index);
		}
		
	   
	      	std::ostringstream oss;
		
		oss << m_objectName.substr(0, (extension_index-21)) << "%" << extension;
		
		    
		 std::string new_filename =  oss.str();
		 	
	int retcode = d.executeQuery("select distinct(deleteMarkerID) from Msys_DeleteMarker where ObjectId =   '" +m_objectId+"'", result);

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed for Currentversion" << std::endl;
			return;
		}
		std::string markerId = result[0][0];
		objectResponse.set_listdeletemarker(markerId);
		return;
       }
       
      	else
	{
		m_error = "The specific object does not exists.";
		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Object");
		err->set_errorcode("Nosuch object");
                err->set_key(m_objectName);
		err->set_errormessage(m_error);
	}
       
}





void object::putObjectLock(s3service::s3object& objectResponse, database& d)
{
    if (!m_bExists)
    {
        addErrorInfo(objectResponse, "Object", "NoSuchObject", "The specified object does not exist.", m_objectName);
        return;
    }
    
    if (m_objectlocking==false) // Check if object locking is enabled (m_objectlocking is true)
    {
        m_objectlocking = true; // Disable object locking

       // int retcode = d.executeQuery("UPDATE Msys_Objects SET objectLocking = " + m_objectlocking + " WHERE objectId = '" + m_objectId + "'");


	 int retcode = d.executeQuery("Update Msys_Objects SET objectLocking = '" + std::to_string(m_objectlocking) + "' WHERE objectId = '" + m_objectId + "'");
//        int retcode = d.executeQuery("UPDATE Msys_Objects SET objectLocking = ? WHERE objectId = ?", m_objectlocking, m_objectId);


        if (retcode != 0)
        {
            // Handle the case where the database update fails.
            std::cout << "ExecuteQuery Failed" << std::endl;
            return;
        }

        // Set the object locking information in the response.
        objectResponse.set_locking(m_objectlocking);//true

    std::cout << "Object Locking State (putObjectLock): " << m_objectlocking << std::endl;



    }
    else
    {
        // Object locking is already disabled.
        addErrorInfo(objectResponse, "Object Locking", "ObjectNotLocked", "Object locking is already disabled for this object.");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void object::setObjectRetention(s3service::s3object& objectResponse, string retentionDate, database& d) {
 std::vector<std::vector<std::string>> orr;

// Parse retentionDate string into a std::tm
    std::tm tm={};
    std::istringstream retentionDateStream(retentionDate);
    retentionDateStream >> std::get_time(&tm, "%m/%d/%Y");

 // Check if the parsed date is valid
    if (retentionDateStream.fail()) {
        // Invalid date format
        addErrorInfo(objectResponse, "Object Retention", "InvalidRetentionDate", "Invalid date format. Please use mm/dd/yyyy format.", m_objectName);
        return;
    }

    int retcode=d.executeQuery (" select objectLocking from Msys_Objects where objectname='"+ m_objectName + "'",orr);

       if (retcode != 0)
                {
                        std::cout << "ExecuteQuery Failed objectlocking" << std::endl;
                        return;
                }

 
    
             if (!orr.empty())
    {
        int objectLockingValue = std::stoi(orr[0][0]);


      std::cout << "Object Locking State: " << objectLockingValue << std::endl;



        if (objectLockingValue == 0)
        {
            addErrorInfo(objectResponse, "Object Retention", "ObjectRetentiondate", "Object Retention is not set for this object", m_objectName);
            return;
        }
    }

  if (!m_bExists) {
        addErrorInfo(objectResponse, "Object", "NoSuchObject", "The specified object does not exist.", m_objectName);
        return;
    }
// Validate and set the retention period
    // Assuming you want to check if the date is in the past
    auto currentDate = std::chrono::system_clock::now();
    auto retentionDatePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    if (currentDate >= retentionDatePoint) {
        // Retention period has expired, disable object locking
        m_objectlocking = false;
        int retcode = d.executeQuery("Update Msys_Objects SET objectLocking = '" + std::to_string(m_objectlocking) + "' WHERE objectId = '" + m_objectId + "'");
        if (retcode != 0) {
            std::cout << "ExecuteQuery Failed OL2" << std::endl;
            return;
        }
    } else {
        // Update the object with the retention period
        m_retentionDate = retentionDate;

        // Update the database with the retention period
        int retcode = d.executeQuery("UPDATE Msys_Objects SET objectRetention = '" + m_retentionDate + "' WHERE objectId = '" + m_objectId + "'");
        if (retcode != 0) {
            std::cout << "ExecuteQuery Failed OL3" << std::endl;
            return;
        }
}

    // Set the retention information in the response
    objectResponse.set_retention_duration(m_retentionDate);
   
}



//////////////////////////////////////////////////////////////////////////////////////////////
/*

void object::putObjectLock(s3service::s3object& objectResponse, database& d)
{
	if (!m_bExists)
	{
		addErrorInfo(objectResponse, "Object", "NoSuchObject", "The specified object does not exist.", m_objectName);
		return;
	}
    if (!m_objectlocking) // Check object locking is disabled (m_objectlocking is false)
    {
        m_objectlocking = true; // Enable object locking

        int retcode = d.executeQuery("UPDATE Msys_Objects SET objectLocking = " + std::to_string(m_objectlocking ? 1 : 0) + " WHERE objectId = '" + m_objectId + "'");


        if (retcode != 0)
        {
            // Handle the case where the database update fails.
            std::cout << "ExecuteQuery Failed" << std::endl;
            return;
        }

        // Set the object locking information in the response.
        objectResponse.set_locking(m_objectlocking);
    }


    else

   {
    // Object locking is already enabled.
    addErrorInfo(objectResponse, "Object Locking", "ObjectAlreadyLocked", "Object locking is already enabled for this object.");
    }
}
*/







///////////////
void object::deleteObject(database& d,s3service::s3object& objectResponse,std::string bucketname, std::string BucketVersion, string accesskey, string secretkey)
{

         m_bucketName = bucketname;
         std::vector<std::vector<std::string>> result11;
	 std::vector<std::vector<std::string>> ol;
 

         std::cout << "Object Locking State: " << m_objectlocking << std::endl;//false
//write query
//
    int retcode=d.executeQuery (" select objectLocking from Msys_Objects where objectname='"+ m_objectName + "'",ol);

  // int retcode=d.executeQuery ( " SELECT objectLocking FROM Msys_Objects WHERE objectName = '"+ m_objectName +" AND objectLocking = '"+ std::to_string(m_objectlocking) +"'",ol);

    
                if (retcode != 0)
                {
                        std::cout << "ExecuteQuery Failed" << std::endl;
                        return;
                }


   /*     if (!ol.empty())
    {
        addErrorInfo(objectResponse, "Object Locking", "ObjectLocked", "Object locking is enabled for this object. Cannot delete.", m_objectName);
        return;
    }*/

		 if (!ol.empty())
    {
        int objectLockingValue = std::stoi(ol[0][0]);


      std::cout << "Object Locking State: " << objectLockingValue << std::endl;



        if (objectLockingValue == 1)
        {
            addErrorInfo(objectResponse, "Object Locking", "ObjectLocked", "Object locking is enabled for this object. Cannot delete.", m_objectName);
            return;
        }
    }

      
	
	if((m_bExists) && (m_objectName == m_objectNameToCheck) && (BucketVersion == "Disable") && (m_objectlocking == 0) )
	{
		//cout << m_objectId << endl;
		//cout << m_bucketId << endl;
		std::vector<std::vector<std::string>> re;
		int retcode = d.executeQuery("select count(path) from Msys_Objects where objectName= '" + m_objectName + "'", re);
		//cout << re[0][0] << endl;
		
		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
		
		 retcode = d.executeQuery("select o.path from Msys_Buckets b JOIN Msys_Access_Keys a on a.accountId = b.accountId JOIN Msys_Objects o  ON b.bucketId = o.bucketId where o.objectId= '" + m_objectId + "' AND o.bucketId = '" + m_bucketId + "' AND a.accessKeyId = '" + accesskey + "' AND a.secretKey = '" + secretkey +"'", result11);
		 
		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
		if(!result11.empty())
		{
			if(result11[0][0] !="NULL")
			{
			
							std::vector<std::vector<std::string>> r;
							retcode = d.executeQuery("select o.CommonId from Msys_Buckets b JOIN Msys_Access_Keys a on a.accountId = b.accountId JOIN  Msys_Objects o ON b.bucketId = o.bucketId  where o. objectName = '"+ m_objectName  +"' AND a.accessKeyId = '" + accesskey + "' AND a.secretKey = '" + secretkey +"' AND o.CommonId <> 'NULL'", r);
								//cout << r[0][0];
							if (retcode != 0)
							{
								std::cout << "ExecuteQuery Failed 1" << std::endl;
								return;
							}
							
							if(!r.empty() && r[0][0] !="NULL")
							{
								std::vector<std::vector<std::string>> res1;
								retcode = d.executeQuery("select o.bucketId from Msys_Buckets b JOIN Msys_Access_Keys a on a.accountId = b.accountId JOIN Msys_Objects o ON b.bucketId = o.bucketId where o.objectName = '"+ m_objectName + "'AND a.accessKeyId = '" + accesskey + "' AND a.secretKey = '" + secretkey +"' AND o.bucketId <> '"+m_bucketId+"'",res1);
								//cout << res1[0][0] << endl;
								//cout << r[0][0] << endl;
								if (retcode != 0)
								{
									std::cout << "ExecuteQuery Failed 2" << std::endl;
									return;
								}
								
								if(!res1.empty())
								{
									retcode = d.executeQuery("update Msys_Objects set CommonId = '"+ r[0][0] +"'	where bucketId = '" + res1[0][0] +"'AND ObjectName = '"+m_objectName+"'");
								}
								if (retcode != 0)
								{
									std::cout << "ExecuteQuery Failed 3" << std::endl;
									return;
								}
								
							}
				retcode = d.executeQuery("DELETE from Msys_Objects Where objectId = '" + m_objectId + "' AND bucketId = '" + m_bucketId + "'");

				if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed" << std::endl;
					return;
				}
				
				if(re[0][0] =="1")
				{
				
					std::string current_path= get_current_dir_name();
					std::string full_path = current_path + "/upload/"  + m_objectName;
					int ret = remove(full_path.c_str());
				
					if (ret != 0)
					{
						std::cout << "Error in removing file" << std::endl;
					}
				}
			return ;
		}
		
	}
		retcode = d.executeQuery("DELETE from Msys_Objects Where objectId = '" + m_objectId + "' AND bucketId = '" + m_bucketId + "'");

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
			
		std::string current_path= get_current_dir_name();
		std::string full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
		int ret = remove(full_path.c_str());
			
		if (ret != 0)
		{
			std::cout << "Error in removing file" << std::endl;
		}
		return;
	
	}
	
	std::string new_filename;
	std::vector<std::vector<std::string>> result;
    	if(BucketVersion == "Enable")
    	{
    
      
		size_t extension_index = m_objectName.find_last_of(".");
			    
		std::string extension;
		    
		if (extension_index != std::string::npos)
		{
			extension = m_objectName.substr(extension_index);
		}
		
	   
	      	std::ostringstream oss;
		
		oss << m_objectName.substr(0, (extension_index-21)) << "_____-%" << extension;
		
		    
		 new_filename =  oss.str();
		 	
	int retcode = d.executeQuery("select convert(DATETIME, UPPER(creationTimestamp)) DateTime ,objectName from Msys_Objects where objectName Like '" + new_filename + "' AND bucketId = '" + m_bucketId + "' order by DateTime DESC", result);

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed for Currentversion" << std::endl;
			return;
		}
       }
	
 



        
	if((m_bExists) && (m_objectName == m_objectNameToCheck) && (result[0][1] == m_objectName ) && (BucketVersion == "Enable") && (m_objectlocking == 0))
	{
		string deletemarkerId = "deletemarker" + getUniqueId();
		for(int i = 0; i < result.size(); i++)   
		{
			string objName = result[i][1];
			readDeletemarker(objName, m_bucketName);
			std::vector<std::vector<std::string>> result1;
			
			int retcode = d.executeQuery("Select * from  Msys_Objects where objectName = '" + objName + "' AND bucketId = '" + m_bucketId + "'", result1);

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed getting objectID" << std::endl;
				return;
			}
				
			string objId = result1[0][0];
			string versionStatus = result1[0][1];
			//string objName = result1[0][2];
			string objSize = result1[0][3];
			string versionId = result1[0][4];
			string creationTime = result1[0][5];
			string bucketId = result1[0][8];
			string bucketName = m_bucketName;
			string deletedtime = getCurrentTimeStamp();


		//	bool objectLocking = m_objectlocking ;


//			        std::cout << "Object locking value for '" << objName << "': " << objectlocking << std::endl;


			 
// 			if (objectlocking){
//			
//		        addErrorInfo(objectResponse,"Object Locking","ObjectLocked","Object locking is enabled for this object. Cannot delete.");
  //      return;
			
//			}
//
//                  else{

			retcode = d.executeQuery("Insert into Msys_DeleteMarker(objectId,VersionStatus, objectName, objectsize, objectVersion, creationTimestamp, bucketId, deleteMarkerID, bucketName, DeletedCreationTimestamp,objectLocking,objectRetention) " \
				    			"values "\
"('"+ objId +"', '"+ versionStatus +"', '"+ objName +"', '"+  objSize +"', '"+ versionId +"', '"+ creationTime +"', '"+ bucketId +"', '"+ deletemarkerId + "', '" + bucketName + "', '" + deletedtime + "','"+ std::to_string(m_objectlocking) +"','"+ m_retentionDate +"' )");
				if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed insert Deletemarker" << std::endl;
					return;
				}

			retcode = d.executeQuery("DELETE from Msys_Objects Where objectId = '" + objId + "' AND bucketId = '" + m_bucketId + "'");

			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed for deleting object in for loop" << std::endl;
				return;
			}
			
			std::string current_path= get_current_dir_name();
			std::string full_path = current_path + "/upload/" + m_bucketName + "/" + objName;
			int ret = remove(full_path.c_str());
			
			if (ret != 0)
			{
				std::cout << "Error in removing file delete file in for loop" << std::endl;
				
			}
		}
	}
//////}
	else if ((m_bExists) && (m_objectName == m_objectNameToCheck) && (result[0][1] != m_objectName )  && (BucketVersion == "Enable"))
	{
		
				int retcode = d.executeQuery("DELETE from Msys_Objects Where objectId = '" + m_objectId + "' AND bucketId = '" + m_bucketId + "'");

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed Delete" << std::endl;
			return;
		}
		
		std::string current_path= get_current_dir_name();
		std::string full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
		int ret = remove(full_path.c_str());
		
		if (ret != 0)
		{
			std::cout << "Error in removing file" << std::endl;
		}
	}
	

	else
	{
		m_error = "The specific object does not exists.";
		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Object");
		err->set_errorcode("Nosuch object");
                err->set_key(m_objectName);
		err->set_errormessage(m_error);
	}
	
}
//////////////////////////////////////////////////////////////////////////////////
void object::downloadObject(serverSocket& ssWrite,  s3service::s3object& objectResponse, std::string bucketname, database& d)
{
    if (!m_bExists)
    {
        m_error = "The specific key does not exists.";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
                err->set_key(m_objectName);
		err->set_errormessage(m_error);
    }
    else if((m_bExists) && (m_objectName == m_objectNameToCheck))
    {
    		std::vector<std::vector<std::string>> result11;
		
		 int retcode = d.executeQuery("select path from Msys_Objects where objectId= '" + m_objectId + "' AND bucketId = '" + m_bucketId + "'", result11);
		
		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
    	if(result11[0][0] != "NULL")
    	{
    		std::string current_path = get_current_dir_name();
    		std::string path= current_path + "/upload/" + m_objectName;
    		objectResponse.set_objectname(m_objectName);
    		auto  ret = readObject(ssWrite, path, objectResponse);
    		
    	return ;
    	}
    	m_bucketName = bucketname;
    	object::nameObject(m_objectName,objectResponse,m_versionStatus);
    	std::string current_path = get_current_dir_name();
    	std::string path = current_path + "/upload/" + bucketname + "/" + m_objectName;
    	std::ifstream iF(path, std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
		return ;
	}

	iF.seekg(0, std::ios::end);
	size_t size = iF.tellg();
	iF.seekg(0, std::ios::beg);
	
	objectResponse.set_len(size);
	//object::nameObject(m_objectName,objectResponse,m_versionStatus);
          
       std::string name = m_objectName;
       //cout << name << endl;
       std::thread th3([this,&ssWrite,name]() { readFromFile(ssWrite,name); });
       sleep(7);
      if(th3.joinable())
      {
      
      	 th3.detach();
      }
      else 
      {
      		/*static std::thread th3([this,&ssWrite,name]() { readFromFile(ssWrite,name); });
       	sleep(7);*/
      	cout << "Thread is not started yet" << endl;
      }
	
        /*auto ret = readFromFile(ssWrite, m_objectName, objectResponse);

        if (!ret)
        {
        	std::cout << "Read from file failed" << std::endl;
        	m_error = "Read from file failed.";

		auto err = objectResponse.add_errorinfo();


		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
                err->set_key(m_objectName);
		err->set_errormessage(m_error);
        }*/
    }
    else 
    {
    	     	m_error = "The object does not exists.";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
              	err->set_key(m_objectName);
		err->set_errormessage(m_error);
    
    }
}

void object::downloadObjectByVersion(serverSocket& ssWrite, s3service::s3object& objectResponse, std::string bucketname)
{
	if (!m_bExists)
    	{
        	m_error = "The specific key does not exists.";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
    	}
    else if(m_versionId1 != m_versionId)
    {
    	        m_error = "The specific version does not exists.";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoversionID");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
    }
    	
    else if((m_bExists) && (m_versionId != "NULL") )
    {
    	m_bucketName = bucketname;
    	object::nameObject(m_objectName,objectResponse,m_versionStatus);
    	std::string current_path = get_current_dir_name();
    	std::string path = current_path + "/upload/" + m_bucketName  + "/" +m_objectName;
    	std::ifstream iF(path, std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
		return ;
	}

	iF.seekg(0, std::ios::end);
	size_t size = iF.tellg();
	iF.seekg(0, std::ios::beg);
	
	objectResponse.set_len(size);
	//object::nameObject(m_objectName,objectResponse,m_versionStatus);
          
       std::string name =m_objectName;
	 std::thread th3([this,&ssWrite,name]() { readFromFile(ssWrite,name); });
	sleep(7);
	th3.detach();
        /*auto ret = readFromFile(ssWrite,m_objectName, objectResponse);

        if (!ret)
        {
        	std::cout << "Read from file failed" << std::endl;
        	m_error = "Read from file failed.";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
                err->set_key(m_objectName);
		err->set_errormessage(m_error);
        }*/
    }
}

inline std::string object::getObjectName()
{
    return m_objectName;
}

inline std::string object::getBucketId()
{
    return m_bucketId;
}

inline std::string object::getObjectId()
{
	return m_objectId;
}

inline std::vector<char> object::getData()
{
    return m_data;
}

inline std::string object::getCreationTimestamp()
{
    return m_creationTimestamp;
}

inline bool object::doesObjectExists()
{
    return m_bExists;
}

std::string object::getErrorMessage()
{
	return m_error;
}



object::~object()
{

}


void object::getObjectProperties(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d)
{
    if (!m_bExists)
    {
        m_error = "The specific key does not exists.";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
               err->set_key(m_objectName);
		err->set_errormessage(m_error);
    }
    else
    {
    	long long file_size = getFileSize(m_objectName);
    	objectResponse.set_len(file_size);
    }
}
void object::initMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d)
{
	std::vector<std::vector<std::string>> re;
	double kb = 0;
	//cout << m_bucketName << endl;
	int retcode1 = d.executeQuery("select Memory_space_IN_bytes, bucketNaME from Msys_Buckets where bucketId = '" + m_bucketId +"'",re);
	
	if (retcode1 != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return ;
	}
	
	if(re.empty())
	{
		std::cout << "result is empty" << std::endl;
		m_error = "result is empty";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
	}
	
	if(re[0][0] != "NULL")
	{
		std::string inkb = re[0][0];
		stringstream m;
		m << inkb;
		m >> kb;
	}
	m_bucketName = re[0][1];
	long long int filesizeinbytes = sizebyte();
	int filesize = stoi(objectRequest.file());
	filesizeinbytes += filesize;

	if(filesizeinbytes > kb  && re[0][0] != "NULL")
	{
		//std::string current_path= get_current_dir_name();
		//std::string full_path = current_path + "/upload/" + re[0][1] + "/" + m_objectName;
		//int ret = remove(full_path.c_str());
		m_error = "Size is full for this bucket please update the bucket size";
		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Object");
		err->set_errorcode("Size is full");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		cout << "Size is full" << endl;
		return;
	}
	std::string fullpath = m_bucketName + "/" + m_objectName;
	boost::filesystem::create_directories(fullpath);

}



	
void object::putMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, std::string bucketname,database& d, std::string bucketversion)
{
	string filepath =  m_objectName;
	m_filesize = objectRequest.filesize();
	int result = downloadFile(ss,filepath, objectRequest.len(), bucketname, objectResponse, bucketversion, d);
}

void object::putmultipart(serverSocket& ssRead, s3service::s3object& objectService,s3service::s3object& objectResponse,database& d)
{
	writeToFileformultipartupload(ssRead,objectService, objectResponse, d);
}


int object::filecount(std::string path, s3service::s3object& objectResponse)
{
	DIR *dp;
  	int i = 0;
  	struct dirent *ep;     
  	dp = opendir (path.c_str());

  	if (dp != NULL)
  	{
    	while (ep = readdir (dp))
      	  i++;

    	(void) closedir (dp);
  	}
  else
  {
  	perror ("Couldn't open the directory");
  	std::cout << "Couldn't open the directory" << std::endl;
	m_error = "Couldn't open the directory";

	auto err = objectResponse.add_errorinfo();

	err->set_errortype("Sender");
	err->set_errorcode("NoSuchKey");
	err->set_key(m_objectName);
	err->set_errormessage(m_error);
	return 0;
  }
    

  return i-2;
}

void object::completeMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, string bucketName, std::string BucketVersion, database& d)
{
	std::string current_path = get_current_dir_name();
	std::string pathforcheck = current_path + "/" + bucketName + "/" + m_objectName + "/" ;
	int number_of_files = filecount(pathforcheck, objectResponse);
	
	std::vector<std::vector<std::string>> re;
	double kb = 0;
	int retcode1 = d.executeQuery("select Memory_space_IN_bytes, bucketName from Msys_Buckets where bucketId = '" + m_bucketId +"'",re);
	
	if (retcode1 != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return ;
	}
	
	if(re.empty())
	{
		std::cout << "result is empty" << std::endl;
		m_error = "result is empty";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		return;
	}
	
	if(re[0][0] != "NULL")
	{
		std::string inkb = re[0][0];
		stringstream m;
		m << inkb;
		m >> kb;
	}
	
		
	if(number_of_files == stoi(objectRequest.filecount()))
	{
		string extension;
		if(m_bExists == false || BucketVersion == "Enable")
		{
			std::string objName; 
			if(BucketVersion == "Disable")
			{
				objName = m_objectName;
				size_t extension_index = m_objectName.find_last_of(".");
			    	
			    	if (extension_index != string::npos) 
			    	{
					extension = m_objectName.substr(extension_index);
			    	}
				
			}
			else if(BucketVersion == "Enable")
			{
				size_t extension_index = m_objectName.find_last_of(".");
			    	
			    	if (extension_index != string::npos) {
					extension = m_objectName.substr(extension_index);
			    	}
			    	std::vector<std::vector<std::string>> result;
			    	m_creationTimestamp = getCurrentTimeStamp();
			    	
				std::ostringstream oss;
			    	oss << m_objectName.substr(0, extension_index) << "_" << m_creationTimestamp << extension;
			    	objName = oss.str();
					
			}
			std::string currentpath = get_current_dir_name();
			std::string path = currentpath + "/" + bucketName + "/" + m_objectName + "/";
			std::vector<std::string> parts;
			
			
			while(true)
			{
				DIR *dir;
				struct dirent *ent;
				if ((dir = opendir(path.c_str())) != NULL)
				{
					int flag=0;
					while ((ent = readdir(dir)) != NULL)
					{
						string name(ent->d_name);
						
						if(name !="." && name !="..")
						{
							parts.push_back(name);
						}
					}
					delete ent;
					if(flag == 0)
					{
						break;
					}
				}
				closedir(dir);
			}
			
			std::sort(parts.begin(), parts.end());

			size_t size;

			//char* data = new char[size];
			std::string obj = currentpath + "/upload/" + bucketName + "/" + objName;
			
			std::fstream fs(obj, std::fstream::out | std::fstream::trunc);
			for(int i=0; i<number_of_files; i++)
			{
				std::string objectpath = path + "part_" + std::to_string(i + 1) + ".txt";
				ifstream ifile(objectpath.c_str(), ios::in);
				ofstream ofile(obj, ios::out | ios::app);
				if (!ifile.is_open()) 
				{
					m_error = "Folder Not Deleted";
					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Object");
					err->set_errorcode("Folder Not Deleted");
					err->set_key(m_objectName);
					err->set_errormessage(m_error);
			    		cout << objectpath << " : file not found" << endl;
			    		return;
		       	}
		       	ofile << ifile.rdbuf();
			}
	 		current_path = get_current_dir_name();
			std::string full_path1 = current_path + "/upload/" + bucketName + "/encrypt" + extension ;
			
			using namespace CryptoPP;
    			unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
    			unsigned char *iv =  (unsigned char *)"0123456789012345";
    			size_t key_size = strlen((const char*)key);

    			CBC_Mode<AES>::Encryption        encryptor(key, key_size, iv);

    			FileSource(obj.c_str(), true, new StreamTransformationFilter(encryptor, new FileSink(full_path1.c_str())));
	 		remove(obj.c_str());
			std::string newname = currentpath + "/upload/" + bucketName + "/" + objName;
			if(rename(full_path1.c_str(), newname.c_str())!=0)
			{
				cout << "Failed to rename" << endl;
				return;
			}
			std::ifstream iF(obj.c_str(), std::ios::in | std::ios::binary);
			if (!iF)
			{
				std::cout << "File not found!";
				m_error = "File not found!";
				auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("File not found");
				err->set_key(m_objectName);
				err->set_errormessage(m_error);
				return;
			}

			iF.seekg(0, std::ios::end);
			int siz=iF.tellg();
			
			ostringstream str1;
			str1 << siz;
			m_fileSize = str1.str();
		
			m_fileSize.append("_bytes");
			m_bExists = true;
			m_creationTimestamp = getCurrentTimeStamp();
			m_objectId = "object" + getUniqueId();
			
			
			
			m_bucketName = re[0][1];
			long long int filesizeinbytes = sizebyte();
			//int filesize = stoi(objectRequest.file());
			//filesizeinbytes += filesize;

			if(filesizeinbytes > kb  && re[0][0] != "NULL")
			{
				std::string current_path= get_current_dir_name();
				std::string full_path = current_path + "/upload/" + re[0][1] + "/" + objName;
				int ret = remove(full_path.c_str());
				m_error = "Size is full for this bucket please update the bucket size";
				auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("Size is full");
				err->set_key(m_objectName);
				err->set_errormessage(m_error);
				cout << "Size is full" << endl;
				return;
			}
			
			pathforcheck = current_path + "/" + bucketName ;
			
			boost::system::error_code ec;
			boost::filesystem::remove_all(pathforcheck.c_str(), ec);
			
			if (ec)
			{
				m_error = "Folder Not Deleted";
				auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("Folder Not Deleted");
				err->set_key(m_objectName);
				err->set_errormessage(m_error);
				std::cout << "Folder Not Deleted" << std::endl;
				return;
				
			}
			if(BucketVersion == "Disable")
			{
			
				int retcode = d.executeQuery("Insert into Msys_Objects " \
					    			"(objectId, VersionStatus,objectName, objectsize,creationTimestamp, bucketId) " \
					    			"values " \
		"('"+ m_objectId +"', '"+ BucketVersion +"', '"+ objName +"', '"+ m_fileSize +"', '"+ m_creationTimestamp +"',  '"+ m_bucketId +"')");


				if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed" << std::endl;
					m_error = "ExecuteQuery Failed";
					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Object");
					err->set_errorcode("ExecuteQuery Failed");
					err->set_key(m_objectName);
					err->set_errormessage(m_error);
					return;
				}
			}
			else if(BucketVersion == "Enable")
			{
				m_versionId = "versionId" + getUniqueId();
							
				int retcode = d.executeQuery("Insert into Msys_Objects " \
					    			"(objectId, VersionStatus,objectName, objectsize,objectVersion,creationTimestamp, bucketId) " \
					    			"values " \
		"('"+ m_objectId +"', '"+ BucketVersion +"', '"+ objName +"', '"+ m_fileSize +"', '" + m_versionId + "', '"+ m_creationTimestamp +"',  '"+ m_bucketId +"')");


				if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed" << std::endl;
					m_error = "ExecuteQuery Failed";
					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Object");
					err->set_errorcode("ExecuteQuery Failed");
					err->set_key(m_objectName);
					err->set_errormessage(m_error);
					return;
				}
			}
			
		}
		else 
		{
			m_error = "File already exists cannot able to complete the process";
			auto err = objectResponse.add_errorinfo();

			err->set_errortype("Object");
			err->set_errorcode("File already exists");
			err->set_key(m_objectName);
			err->set_errormessage(m_error);
			return;
		}
	}
	else 
	{
		m_error = "File count is not matching please use List Multipart option to verify the part you have uploaded";
		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Object");
		err->set_errorcode("file count not match");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		return;
	}

	
	
}
void object::abortMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d)
{
	std::vector<std::vector<std::string>> re;
	int retcode1 = d.executeQuery("select  bucketName from Msys_Buckets where bucketId = '" + m_bucketId +"'",re);
	
	if (retcode1 != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return ;
	}
	
	if(re.empty())
	{
		std::cout << "result is empty" << std::endl;
		m_error = "result is empty";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		return;
	}
	
	if(re[0][0] == "NULL")
	{
		std::cout << "result is NULL" << std::endl;
		m_error = "result is NULL";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		return;
	}

	std::string current_path = get_current_dir_name();
	std::string pathforcheck = current_path + "/" + re[0][0] + "/" + m_objectName;
	boost::system::error_code ec;
	std::ifstream iF(pathforcheck.c_str(), std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
		m_error = "File not found!";
		auto err = objectResponse.add_errorinfo();
		err->set_errortype("Object");
		err->set_errorcode("File not found");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		std::cout << "Folder Not Deleted" << std::endl;
		return ;
	}
	boost::filesystem::remove_all(pathforcheck.c_str(), ec);
	if (ec)
	{
		m_error = "Folder Not Deleted";
		auto err = objectResponse.add_errorinfo();
		err->set_errortype("Object");
		err->set_errorcode("Folder Not Deleted");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		std::cout << "Folder Not Deleted" << std::endl;
		return;
				
	}
}
void object::listMultiPartUpload(serverSocket& ss, s3service::s3object& objectRequest, s3service::s3object& objectResponse, database& d)
{

	std::vector<std::vector<std::string>> re;
	int retcode1 = d.executeQuery("select  bucketName from Msys_Buckets where bucketId = '" + m_bucketId +"'",re);
	if (retcode1 != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return ;
	}
	
	if(re.empty())
	{
		std::cout << "result is empty" << std::endl;
		m_error = "result is empty";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		return;
	}
	
	if(re[0][0] == "NULL")
	{
		std::cout << "result is NULL" << std::endl;
		m_error = "result is NULL";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		return;

	}
	std::string currentpath = get_current_dir_name();
	std::string path = currentpath + "/" + re[0][0] + "/" + m_objectName + "/";
	std::vector<std::string> parts;
			
			
	while(true)
	{
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(path.c_str())) != NULL)
		{
			int flag=0;
			while ((ent = readdir(dir)) != NULL)
			{
				string name(ent->d_name);
						
				if(name !="." && name !="..")
				{
					parts.push_back(name);
				}
			}
			delete ent;
			if(flag == 0)
			{
				break;
			}
		}
		else if((dir = opendir(path.c_str())) == NULL)
		{
	       	m_error = "Error at listing Multi part upload.";
       		auto err = objectResponse.add_errorinfo();

        		err->set_errortype("No Object");
			err->set_errorcode("function:listMultiPartUpload");
			err->set_key(m_objectName);
			err->set_errormessage(m_error);
			return;
		}
		
				closedir(dir);
	}
			
	std::sort(parts.begin(), parts.end());
	std::string sendfiles;
	for(int i=0; i< parts.size(); i++)
	{
		sendfiles += parts[i];
		sendfiles += " ";
	}
	objectResponse.set_filename(sendfiles);
}
long object::getFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}
/*int object::uploadFile(serverSocket& ss, string file_name)
{
    int len = 64 * 1024;
    char sdbuf[len];
    try
    {
        bzero(sdbuf, len);
        int fs_block_sz;
        //FILE *fs = fopen(file_name.c_str(), "r");
        int fileFd = open(file_name.c_str(), O_RDONLY);
        if (fileFd < 0)
        {
            //printf("ERROR: File %s not found.\n", file_name.c_str());
            std::cout  <<  "File not found" << file_name;
        }
        
        int cli_id= ss.get_clientfd();
        
        while ((fs_block_sz = read(fileFd, sdbuf, len)) > 0)
        {
            if (send(cli_id, sdbuf, fs_block_sz, 0) < 0)
            {
                std::cout  <<  "ERROR: Failed to send file " << file_name.c_str() << " errno = " << errno ;
                return EXIT_FAILURE;
            }
            bzero(sdbuf, len);
            std::cout  <<  "-" ;
        }
        std::cout  <<  "Upload of file " << file_name.c_str()  << "was successfull" ;
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << std::endl;
        std::cout  <<  "function:UploadFile";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}*/
int object::downloadFile(serverSocket& ss, string file_name, unsigned long long bytes, std::string bucketname, s3service::s3object& objectResponse, std::string BucketVersion , database & d)
{		

	std::vector<std::vector<std::string>> re;
	double kb = 0;
	int retcode1 = d.executeQuery("select Memory_space_IN_bytes from Msys_Buckets where bucketName = '" + bucketname +"'",re);
	
	if (retcode1 != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return EXIT_FAILURE;
	}
	
	if(re.empty())
	{
		std::cout << "result is empty" << std::endl;
		m_error = "result is empty";

		auto err = objectResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchKey");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
	}
	
	if(re[0][0] != "NULL")
	{
		std::string inkb = re[0][0];
		stringstream m;
		m << inkb;
		m >> kb;
	}


	if(BucketVersion == "Enable")
	{
		size_t extension_index = m_objectName.find_last_of(".");
	    	string extension;
	    	if (extension_index != string::npos) {
			extension = m_objectName.substr(extension_index);
	    	}
	    	std::vector<std::vector<std::string>> result;
	    	m_creationTimestamp = getCurrentTimeStamp();
	    	
		std::ostringstream oss;
	    	oss << m_objectName.substr(0, extension_index) << "_" << m_creationTimestamp << extension;
	    	m_objectName = oss.str();
	    	
		std::string current_path = get_current_dir_name();
		std::string full_path = current_path + "/upload/" + bucketname + "/";
	    	std::string m_object_N = full_path + m_objectName;
	    	
		if (!m_bExists)
	   	{

	   		    int len = 600;
			    char buffer[len];
			    int bytes_read = 0;
			    try
			    {
			    	//open(filename, O_RDWR|O_CREAT, 0666)
				int fileFd = open(m_object_N.c_str(), O_CREAT|O_RDWR|O_CREAT,0666);
				if (fileFd < 0)
				{
				    //printf("ERROR: File %s not found.\n", file_name.c_str());
				    std::cout  <<  "File not found" << file_name << std::endl;
				}
				unsigned long long tot = 0;
			       int cli_id= ss.get_clientfd();
			       
			       int i=0;
			       
			       stringstream m;
				m << m_filesize;
				int x = 0;
				m >> x;
				int totalsize = 0;
				bytes_read = read(cli_id, buffer, sizeof(buffer));
				while(true)
				{
				    if (bytes_read > 0)
				    {
				   	
					write(fileFd, buffer, bytes_read);
					totalsize += bytes_read;
					
				    }else if(bytes_read == 0){
					return EXIT_FAILURE;
				    }
				    if (totalsize == x)
				    	break;
				    bytes_read = read(cli_id, buffer, sizeof(buffer));
				    
				}
				m_bucketName = bucketname;
				long long int filesizeinbytes = sizebyte();
				if(filesizeinbytes > kb  && re[0][0] != "NULL")
				{
					std::string current_path= get_current_dir_name();
					std::string full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
					int ret = remove(full_path.c_str());
					m_error = "Size is full for this bucket please update the bucket size";
					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Object");
					err->set_errorcode("Size is full");
					err->set_key(m_objectName);
					err->set_errormessage(m_error);
					cout << "Size is full" << endl;
					return EXIT_FAILURE;
				}
				m_objectId = "object" + getUniqueId();
				m_creationTimestamp = getCurrentTimeStamp();	
				m_versionId = "versionId" + getUniqueId();
				std::string filesize = to_string(x) + "_bytes";
				
				int retcode = d.executeQuery("Insert into Msys_Objects " \
				    			"(objectId, VersionStatus,objectName,objectsize, objectVersion, creationTimestamp, bucketId) " \
				    			"values " \
"('"+ m_objectId +"', '"+ BucketVersion +"', '"+ m_objectName +"','"+ filesize +"' , '"+ m_versionId +"' , '"+ m_creationTimestamp +"', '"+ m_bucketId +"')");
			

				if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed" << std::endl;
					m_error = "ExecuteQuery Failed";

					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Sender");
					err->set_errorcode("NoSuchKey");
				       err->set_key(m_objectName);
					err->set_errormessage(m_error);
					return EXIT_FAILURE;
				}

			    }
			    
			    catch (std::exception const &e)
			    {
				std::cerr << e.what() << std::endl;
				std::cout  <<  "function:DownloadFile";
				
			       std::cout << "ExecuteQuery Failed in Msys_DeleteMarker" << std::endl;
			       m_error = "Error at Multi part upload.";
			       auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("function:DownloadFile");
				err->set_key(m_objectName);
				err->set_errormessage(m_error);
				return EXIT_FAILURE;
			    }

	   		
	   	}
	   	else 
	   	{
	   	
	   		    int len = 600;
			    char buffer[len];
			    int bytes_read = 0;
			    try
			    {
			    	//open(filename, O_RDWR|O_CREAT, 0666)
				int fileFd = open(m_object_N.c_str(), O_CREAT|O_RDWR|O_CREAT,0666);
				if (fileFd < 0)
				{
				    //printf("ERROR: File %s not found.\n", file_name.c_str());
				    std::cout  <<  "File not found" << file_name << std::endl;
				}
				unsigned long long tot = 0;
			       int cli_id= ss.get_clientfd();
			       
			       int i=0;
			       
			       stringstream m;
				m << m_filesize;
				int x = 0;
				m >> x;
				int totalsize = 0;
				bytes_read = read(cli_id, buffer, sizeof(buffer));
				while(true)
				{
				    if (bytes_read > 0)
				    {
				   	
					write(fileFd, buffer, bytes_read);
					totalsize += bytes_read;
					
				    }else if(bytes_read == 0){
					return EXIT_FAILURE;
				    }
				    if (totalsize == x)
				    	break;
				    bytes_read = read(cli_id, buffer, sizeof(buffer));
				    
				}
				m_bucketName = bucketname;
				long long int filesizeinbytes = sizebyte();

				if(filesizeinbytes > kb  && re[0][0] != "NULL")
				{
					std::string current_path = get_current_dir_name();
					std::string full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
					
					int ret = remove(full_path.c_str());
					m_error = "Size is full for this bucket please update the bucket size";
					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Object");
					err->set_errorcode("Size is full");
					err->set_key(m_objectName);
					err->set_errormessage(m_error);
					cout << "Size is full" << endl;
					return EXIT_FAILURE;
				}
				m_objectId = "object" + getUniqueId();
				m_creationTimestamp = getCurrentTimeStamp();	
				m_versionId = "versionId" + getUniqueId();
				std::string filesize = to_string(x) + "_bytes";
				
				int retcode = d.executeQuery("Insert into Msys_Objects " \
				    			"(objectId, VersionStatus,objectName,objectsize, objectVersion, creationTimestamp, bucketId) " \
				    			"values " \
"('"+ m_objectId +"', '"+ BucketVersion +"', '"+ m_objectName +"','"+ filesize +"' , '"+ m_versionId +"' , '"+ m_creationTimestamp +"', '"+ m_bucketId +"')");
			

				if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed" << std::endl;
					m_error = "ExecuteQuery Failed";

					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Sender");
					err->set_errorcode("NoSuchKey");
				       err->set_key(m_objectName);
					err->set_errormessage(m_error);
					return EXIT_FAILURE;
				}

			    }
			    
			    catch (std::exception const &e)
			    {
				std::cerr << e.what() << std::endl;
				std::cout  <<  "function:DownloadFile";
				
			       std::cout << "ExecuteQuery Failed in Msys_DeleteMarker" << std::endl;
			       m_error = "Error at Multi part upload.";
			       auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("function:DownloadFile");
				err->set_key(m_objectName);
				err->set_errormessage(m_error);
				return EXIT_FAILURE;
			    }

	   	}
	   	
	   	
	   }
	  else if(BucketVersion == "Disable")
	  {
	  	if(!m_bExists){
	  	    std::string current_path= get_current_dir_name();
		    std::string full_path=current_path+"/upload/" + bucketname + "/" ;
		    std::string fullpath = full_path + file_name;
		    int len = 600;
		    char buffer[len];
		    int bytes_read = 0;
		    try
		    {
		    	//open(filename, O_RDWR|O_CREAT, 0666)
			int fileFd = open(fullpath.c_str(), O_CREAT|O_RDWR|O_CREAT,0666);
			if (fileFd < 0)
			{
			    //printf("ERROR: File %s not found.\n", file_name.c_str());
			    std::cout  <<  "File not found" << file_name << std::endl;
			}
			unsigned long long tot = 0;
		       int cli_id= ss.get_clientfd();
		       
		       int i=0;
		       
		       stringstream m;
			m << m_filesize;
			int x = 0;
			m >> x;
			int totalsize = 0;
			bytes_read = read(cli_id, buffer, sizeof(buffer));
			while(true)
			{
			    if (bytes_read > 0)
			    {
			   	
				write(fileFd, buffer, bytes_read);
				totalsize += bytes_read;
				
			    }else if(bytes_read == 0){
				return EXIT_FAILURE;
			    }
			    if (totalsize == x)
			    	break;
			    bytes_read = read(cli_id, buffer, sizeof(buffer));
			    
			}
			m_bucketName = bucketname;
			long long int filesizeinbytes = sizebyte();
			if(filesizeinbytes > kb  && re[0][0] != "NULL")
			{
				std::string current_path= get_current_dir_name();
				std::string full_path = current_path + "/upload/" + m_bucketName + "/" + m_objectName;
				int ret = remove(full_path.c_str());
				m_error = "Size is full for this bucket please update the bucket size";
				auto err = objectResponse.add_errorinfo();

				err->set_errortype("Object");
				err->set_errorcode("Size is full");
				err->set_key(m_objectName);
				err->set_errormessage(m_error);
				cout << "Size is full" << endl;
				return EXIT_FAILURE;
			}
			m_objectId = "object" + getUniqueId();
			std::string filesize = to_string(x) + "_bytes";
			m_creationTimestamp = getCurrentTimeStamp();	
			int retcode = d.executeQuery("Insert into Msys_Objects (objectId,VersionStatus, objectName, objectsize,  creationTimestamp, bucketId) " \
		"values "\
		"('"+ m_objectId +"', '"+ BucketVersion  +"', '"+ m_objectName +"', '"+ filesize+"', '"+ m_creationTimestamp +"', '"+ m_bucketId +"')");

				if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed" << std::endl;
					m_error = "ExecuteQuery Failed";

					auto err = objectResponse.add_errorinfo();

					err->set_errortype("Sender");
					err->set_errorcode("NoSuchKey");
				       err->set_key(m_objectName);
					err->set_errormessage(m_error);
					return EXIT_FAILURE;
				}
				//cout << BucketVersion << endl;
				//cout << filesize << endl;
				
    		}
	    
	    catch (std::exception const &e)
	    {
		std::cerr << e.what() << std::endl;
		std::cout  <<  "function:DownloadFile";
		
	       std::cout << "ExecuteQuery Failed in Msys_DeleteMarker" << std::endl;
	       m_error = "Error at Multi part upload.";
	       auto err = objectResponse.add_errorinfo();

		err->set_errortype("Object");
		err->set_errorcode("function:DownloadFile");
		err->set_key(m_objectName);
		err->set_errormessage(m_error);
		return EXIT_FAILURE;
	    }
    	}
    
    }


    return EXIT_SUCCESS;
}


