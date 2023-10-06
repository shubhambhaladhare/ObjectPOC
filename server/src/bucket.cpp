#include "bucket.hpp"
#include <map>



void bucket::setClassVariables(database& d)
{
	std::vector<std::vector<std::string>> result;

	int retcode = d.executeQuery("Select * from Msys_Buckets where bucketName = '" + m_bucketName + "' AND accountId = '" + m_accountId + "'", result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}

	if (result.empty())
	{
		m_bExists = false;
		m_arn = "";
		m_creationTimestamp = "";
		m_bucketId = "";
		m_tags = {};
		m_bucketVersion="";
		m_bucketNameCheck = "";
		m_memory ="";
		//m_expiry = "";
	}

	else
	{

		m_bExists = true;
		m_bucketId = result[0][0];
		m_creationTimestamp = result[0][4];
		m_arn = result[0][3];
		m_bucketNameCheck = result[0][1];
		m_bucketVersion = result[0][2];
		//m_expiry = result[0][6];
	}
}

void bucket::get_storagelens(s3service::s3bucket& bucketResponse,database& d)
{
	std::vector<std::vector<std::string>> res;
	std::vector<std::vector<std::string>> res2;
	std::map<string, string> filesCount;
	if(m_bExists)
	{
		int retcode = d.executeQuery("select objectName from Msys_Objects where bucketId = '"+ m_bucketId +"'",res);

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
		   
		if(res.size() >=0)
		{
			for(int i=0; i<res.size(); i++)
			{      
				size_t extension_index = res[i][0].find_last_of(".");
				std::string extension;
		    
				if (extension_index != std::string::npos)
				{
					extension = res[i][0].substr(extension_index);
				}
				std::vector<std::vector<std::string>> res1 ;
				int retcode = d.executeQuery("select count(objectName) from Msys_Objects where bucketId = '"+ m_bucketId +"' AND objectName like '%"+extension+"' ",res1);
				

				if (retcode != 0)
				{
					std::cout << "ExecuteQuery Failed" << std::endl;
					return;
				}
				filesCount[extension] = res1[0][0];
							
			}
			map<string, string>::iterator itr;
			std::string countoffiles;
			for(itr = filesCount.begin(); itr != filesCount.end(); ++itr)
			{
				
				countoffiles += itr->first + ":" +itr->second + " ";
			}
			
			bucketResponse.set_storagelens(countoffiles);
		
			    

	 	}
	 	 retcode = d.executeQuery("select Memory_space_in_bytes from Msys_Buckets where bucketId = '"+ m_bucketId +"'",res2);

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
		std::string total_size = res2[0][0];
		
		bucketResponse.set_total_size(total_size);
		
	 	long long int memory = sizebyte();
	 	stringstream m;
		m << res2[0][0];
		double x = 0;
		m >> x;
	 	std::string size_consumed = to_string(memory);
	 	
	 	bucketResponse.set_size_consumed(size_consumed);
	 	
	 	std::string free_space = to_string(x - (memory));
	 	
	 	bucketResponse.set_free_space(free_space);
	}
	else 
	{
		m_error = "Bucket Name doesnot matchs";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);	
	}
}
void bucket::setTags(std::map<std::string, std::string>& m)
{
	m_tags = m;
}

bucket::bucket(const std::string& bucketName, const std::string& accountId, database& d)
{
    m_bucketName = bucketName;
    m_accountId = accountId;
    m_bExists = false;

    setClassVariables(d);
}
void bucket::createBucket(s3service::s3bucket& bucketResponse, database& d)
{
    if (m_bExists)
    {
        m_error = "The requested bucket name is not available. The bucket namespace is shared by all users of the system. Please select a different name and try again.";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("BucketAlreadyExists");
		err->set_errormessage(m_error);
    }
	else
	{
		m_memory = "1000.000000";
	   m_arn = "arn:aws:s3:::" + m_bucketName;
	   m_creationTimestamp = getCurrentTimeStamp();
	   m_bucketId = "bucket" + getUniqueId();
	   m_bExists = true;
	   m_bucketVersion="Disable";	
	   std::string current_path= get_current_dir_name();
	   std::string full_path=current_path +"/upload/" + m_bucketName;
	   if(!(boost::filesystem::create_directories(full_path)))
	   {
	        m_error = "The specified bucket folder does not created";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucketfolder");
		err->set_errormessage(m_error);
	   	return;
	   }
	//if(a == m_accountId)
	   int retcode = d.executeQuery("Insert into Msys_Buckets (bucketId, bucketName,bucketVersion, arn, creationTimestamp, accountId,Memory_space_IN_bytes ) values ('" + m_bucketId + "', '" + m_bucketName + "', '" + m_bucketVersion + "', '"+ m_arn + "', '" + m_creationTimestamp + "', '" + m_accountId + "', '"+m_memory+"')");

	   if (retcode != 0)
	   {
		   std::cout << "ExecuteQuery Failed" << std::endl;
		   return;
	   }
	}
}
void bucket::set_lifecycle_rule(s3service::s3bucket& bucketResponse,database& d, std::string expire_date)
{
	if (!m_bExists)
    	{
        	m_error = "The specified bucket does not exists";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);
    	}
    	/*else if((m_bExists) && m_expiry !="")
    	{
        	m_error = "expiry limit is already set";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);
    	}*/
	else if((m_bExists) && (m_bucketName == m_bucketNameCheck))
	{
		int retcode = d.executeQuery("Update Msys_Buckets SET expiry_limit = '" + expire_date + "' where bucketId = '" + m_bucketId + "' AND accountId = '" + m_accountId + "'");

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			m_error = "ExecuteQuery Failed in archive2";

			auto err = bucketResponse.add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("NoSuchBucket");
			err->set_errormessage(m_error);
			return;
		}
		
	std::vector<std::vector<std::string>> result;
	int ret1 = d.executeQuery("select b.expiry_limit,abs(DateDiff(dd,convert(DATE, UPPER(o.creationTimestamp)), convert(Date,SYSDATETIME()))) expiry_Date, b.bucketName, o.objectName,o.objectId,o.path from Msys_Buckets b join Msys_Objects o on b.bucketId = o.bucketId where b.bucketId = '" +m_bucketId + "' AND  b.accountId = '" + m_accountId + "'",result);

		if (ret1 == -1)
		{
			std::cout << "ExecuteQuery Failed in fetching Lifecyle rule" << std::endl;
			return ;
		}
		
		int len = result.size();
		vector<string> details;
		for(int i=0;i<len;i++)
		{
				if(result[i][0] <= result[i][1])
				{
					if(result[i][5] != "NULL")
					{
						std::string ObjectName = result[i][3];
						std::vector<std::vector<std::string>> res;
						int retcode = d.executeQuery("select count(path) from Msys_Objects where objectName= '" + ObjectName + "'", res);
						if (retcode != 0)
						{
							std::cout << "ExecuteQuery Failed" << std::endl;
							return;
						}
						std::vector<std::vector<std::string>> r;
					        retcode = d.executeQuery("select o.CommonId from Msys_Buckets b join Msys_Objects o on b.bucketId = o.bucketId where o.objectName = '"+ ObjectName +"'AND  b.accountId = '" + m_accountId + "' AND o.CommonID <> 'NULL'", r);
					        	//cout << r[0][0];
					        if (retcode != 0)
						{
							std::cout << "ExecuteQuery Failed 1" << std::endl;
							return;
						}
						
					        if(!r.empty() && r[0][0] !="NULL")
					        {
					        	std::vector<std::vector<std::string>> res1;
					        	retcode = d.executeQuery("select o.bucketId from Msys_Objects o join Msys_Buckets b on b.bucketId = o.bucketId where o.objectName = '"+ ObjectName +"'AND  b.accountId = '" + m_accountId + "'AND o.bucketId <> '"+m_bucketId+"'",res1);
					        	//cout << res1[0][0] << endl;
					        	//cout << r[0][0] << endl;
					        	if (retcode != 0)
							{
								std::cout << "ExecuteQuery Failed 2" << std::endl;
								return;
							}
							
					        	if(!res1.empty())
					        	{
					        		retcode = d.executeQuery("update Msys_Objects set CommonId = '"+ r[0][0] +"'	where bucketId = '" + res1[0][0] +"'AND ObjectName = '"+ObjectName+"'");
					        	}
					        	if (retcode != 0)
							{
								std::cout << "ExecuteQuery Failed 3" << std::endl;
								return;
							}
					        	
					        }	
						
						if(res[0][0]=="1")
						{
							
							std::string current_path = get_current_dir_name();
							std::string full_path = current_path + "/upload/" + ObjectName;
							int ret = remove(full_path.c_str());
							if (ret != 0)
							{
								std::cout << "Error in removing file in Lifecyle rule" << std::endl;
							}
							
						}


						std::string objectId = result[i][4];
						
						retcode = d.executeQuery("DELETE from Msys_Objects Where objectId = '" + objectId + "'");

						if (retcode != 0)
						{
							std::cout << "ExecuteQuery Failed in deleting Lifecyle rule" << std::endl;
						}
						return;
					}
					else 
					{
						std::string bucketName = result[i][2];
						std::string ObjectName = result[i][3];
						std::string current_path = get_current_dir_name();
						std::string full_path = current_path + "/upload/" + bucketName + "/" + ObjectName;
						int ret = remove(full_path.c_str());

						if (ret != 0)
						{
							std::cout << "Error in removing file in Lifecyle rule" << std::endl;
						}
						std::string objectId = result[i][4];
						
						int retcode = d.executeQuery("DELETE from Msys_Objects Where objectId = '" + objectId + "'");

						if (retcode != 0)
						{
							std::cout << "ExecuteQuery Failed in deleting Lifecyle rule" << std::endl;
						}
					}

			      }
		}
	
	}
	else 
	{
		m_error = "Bucket Name doesnot matchs";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);
	}
	
	
}


long long int bucket::sizebyte() {
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
    if(object != "." && object != "..")
    {
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
  }

  closedir( d );

  return total;
  }
void bucket::set_storageQuota(s3service::s3bucket& bucketResponse,database& d, std::string memoryspace)
{


	if(m_bExists)
	{
	
		long long int memory = sizebyte();
		stringstream m;
		m << memoryspace;
		long long int x = 0;
		m >> x;
		if(memory > x)
		{
			m_error = "Increase the memory sapce the file has memory space more than this";

			auto err = bucketResponse.add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("Memory space ");
			err->set_errormessage(m_error);
			return;
		}
		int ret1 = d.executeQuery("update Msys_Buckets set Memory_space_IN_bytes = '" + memoryspace +"' where bucketId = '"+ m_bucketId +"'");
		if (ret1 == -1)
		{
			std::cout << "ExecuteQuery Failed in updating storage space" << std::endl;
			m_error = "ExecuteQuery Failed in updating storage space";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("ExecuteQuery");
		err->set_errormessage(m_error);
			return ;
		}
		
		
	}
	else 
	{
		m_error = "Bucket Name doesnot matchs";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);
		return;
	}
}

void bucket::deleteBucket(s3service::s3bucket& bucketResponse, database& d)
{
    	if (!m_bExists)
    	{
        	m_error = "The specified bucket does not exists";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);
    	}
	else if((m_bExists) && (m_bucketName == m_bucketNameCheck))
	{

		std::string current_path = get_current_dir_name();
		std::string full_path = current_path + "/upload/" + m_bucketNameCheck;
		boost::system::error_code ec;
		boost::filesystem::remove_all(full_path.c_str(), ec);
		if (ec)
		{
			std::cout << "Folder Not Deleted" << std::endl;
			m_error = "Folder Not Deleted";

			auto err = bucketResponse.add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("NoSuchBucket");
			err->set_errormessage(m_error);
			std::cout << ec.message() << endl;
			return;
			
		}
		
		std::vector<std::vector<std::string>> res;
		
		int retcode = d.executeQuery("select count(*) from Msys_Objects WHERE bucketId = '" + m_bucketId + "'", res);
		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			m_error = "ExecuteQuery Failed";

			auto err = bucketResponse.add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("NoSuchBucket");
			err->set_errormessage(m_error);
			return;
		}
		
		if(res[0][0] != "0")
		{
		
						std::vector<std::vector<std::string>> r;
					        retcode = d.executeQuery("select CommonId,objectName from Msys_Objects where bucketId = '"+ m_bucketId  +"' AND CommonID <> 'NULL'", r);
					        	//cout << r[0][0];
					        if (retcode != 0)
						{
							std::cout << "ExecuteQuery Failed 1" << std::endl;
							return;
						}
						
					        if(!r.empty() && r[0][0] !="NULL")
					        {
					        	for(int i=0; i<r.size(); i++)
					        	{
					        	/*std::vector<std::vector<std::string>> res1;
					        	retcode = d.executeQuery("select bucketId from Msys_Objects where objectName = '"+ r[i][1] + "'AND bucketId <> '"+m_bucketId+"'",res1);
					        	//cout << res1[0][0] << endl;
					        	//cout << r[0][0] << endl;
					        	if (retcode != 0)
							{
								std::cout << "ExecuteQuery Failed 2" << std::endl;
								return;
							}
							
					        	if(!res1.empty())
					        	{*/
					        		retcode = d.executeQuery("update Msys_Objects set CommonId = '"+ r[i][0] +"'	where bucketId <>'" + m_bucketId +"'AND ObjectName = '"+r[i][1]+"'");
					        	//}
					        	if (retcode != 0)
							{
								std::cout << "ExecuteQuery Failed 3" << std::endl;
								return;
							}
							}
					        	
					        }
			int retcode = d.executeQuery("Delete from Msys_Objects WHERE bucketId = '" + m_bucketId + "'");
			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed" << std::endl;
				m_error = "ExecuteQuery Failed";

				auto err = bucketResponse.add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("NoSuchBucket");
				err->set_errormessage(m_error);
				return;
			}
		}
	
    		retcode = d.executeQuery("Delete from Msys_Buckets WHERE bucketId = '" + m_bucketId + "'");

    		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			m_error = "ExecuteQuery Failed";

			auto err = bucketResponse.add_errorinfo();

			err->set_errortype("Sender");
			err->set_errorcode("NoSuchBucket");
			err->set_errormessage(m_error);
			return;
		}
					
	}
	else 
	{
		m_error = "The specified bucket does not exists";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);
	}
}

void bucket::putBucketTags(std::map<std::string, std::string>& tags, s3service::s3bucket& bucketResponse, database& d)
{
	std::vector<std::vector<std::string>> result;

	if (!m_bExists)
    {
        m_error = "The specified bucket does not exists";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);
    }
	else
	{
		for (auto it = tags.begin(); it != tags.end(); it++)
		{
			if (m_tags.find(it->first) != m_tags.end())
			{
				m_error = "Cannot provide multiple Tags with the same key";

				auto err = bucketResponse.add_errorinfo();

				err->set_errortype("Sender");
				err->set_errorcode("InvalidTag");
				err->set_errormessage(m_error);	

				break;
			}
			else
			{
				m_tags.insert({it->first, it->second});

				int retcode = d.executeQuery("Insert into Msys_Bucket_Tags (bucketKey, bucketValue, bucketId) values ('" + it->first + "', '" + it->second + "', '" + m_bucketId + "')");

				if (retcode == -1)
				{
					std::cout << "Executequery failed" << std::endl;
					return;
				}
			}
		}
	}
}

void bucket::listBucketTags(s3service::s3bucket& bucketResponse)
{
	if (!m_bExists)
    {
        m_error = "The specified bucket does not exists";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);
    }
	else
	{
		for (auto it = m_tags.begin(); it != m_tags.end(); it++)
		{
			auto tag = bucketResponse.add_tag();

			tag->set_key(it->first);
			tag->set_value(it->second);
		}
	}
}
void bucket::list_object_version(s3service::s3bucket& bucketResponse,database& d)
{
	if(m_bExists)
	{
	   std::vector<std::vector<std::string>> result;
			int retcode = d.executeQuery("Select o.objectName, o.objectsize, o.VersionStatus, o.objectVersion from Msys_Buckets b JOIN Msys_Objects o ON b.bucketId=o.bucketId And b.bucketId='" + m_bucketId + "'", result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}
	std::string sendResponse;
	std::string str;
	for(int i=0;i<result.size();i++)
	  {
	  	for(int j=0;j<4;j++)
	  	{
	  	      str=result[i][j];
		      sendResponse.append(str);
		      sendResponse.append(" ");
		}
		
	  }
	  bucketResponse.set_list_object_versions(sendResponse);
	}
	else 
	{
		auto err = bucketResponse.add_errorinfo();

        	err->set_errortype("Sender");
        	err->set_errorcode("NoSuchBucket");
        	err->set_errormessage("No Objects are found in the database. Please check the Bucket Name");
	}
}

void bucket::list_object(s3service::s3bucket& bucketResponse,database& d)
{	
	if(m_bExists)
	{
	   std::vector<std::vector<std::string>> result;
			int retcode = d.executeQuery("Select o.objectName from Msys_Buckets b JOIN Msys_Objects o ON b.bucketId=o.bucketId And b.bucketId='" + m_bucketId + "'", result);

	if (retcode != 0)
	{
		std::cout << "ExecuteQuery Failed" << std::endl;
		return;
	}
	std::string sendResponse;
	std::string str;
	for(int i=0;i<result.size();i++)
	  {

	  	      str=result[i][0];
		      sendResponse.append(str);
		      sendResponse.append(" ");
		
		
	  }
	  bucketResponse.set_list_object(sendResponse);
	}
	else 
	{
		auto err = bucketResponse.add_errorinfo();

        	err->set_errortype("Sender");
        	err->set_errorcode("NoSuchBucket");
        	err->set_errormessage("No Objects are found in the database. Please check the Bucket Name");
	}
}

/*void bucket::deleteBucketTags(s3service::s3bucket& bucketResponse, database& d)
{
	if (!m_bExists)
    {
        m_error = "The specified bucket does not exists";

		auto err = bucketResponse.add_errorinfo();

		err->set_errortype("Sender");
		err->set_errorcode("NoSuchBucket");
		err->set_errormessage(m_error);
    }
	else
	{
		m_tags.clear();

		int retcode = d.executeQuery("DELETE from Msys_Bucket_Tags WHERE bucketKey = '" + m_tags + "' AND bucketId = '" + m_bucketId + "'");

		if (retcode != 0)
		{
			std::cout << "ExecuteQuery Failed" << std::endl;
			return;
		}
	}
}*/

////////////////////////////////////////////////////////////////change for bucket version/////////////////
void bucket::put_Bucket_Version(s3service::s3bucket& bucketResponse,database& d)
{
	if((m_bExists) && (m_bucketVersion != "Enable")){	
			m_bucketVersion="Enable";
		    	int retcode = d.executeQuery("UPDATE Msys_Buckets SET bucketVersion= '" +m_bucketVersion+"' WHERE bucketId = '" + m_bucketId + "'");

    			if (retcode != 0)
			{
				std::cout << "ExecuteQuery Failed" << std::endl;
				return;
			}
			bucketResponse.set_version(m_bucketVersion);
		}
	else if((m_bExists) && (m_bucketVersion == "Enable"))
	{
			auto err = bucketResponse.add_errorinfo();
			m_error = "Version is already exist for this bucket";
			err->set_errortype("Bucket Name"); 
			err->set_errorcode("No Bucket");
			err->set_errormessage(m_error);
	}
	else 
	{
			auto err = bucketResponse.add_errorinfo();
			m_error = "Bucket Name does not exist" ;
			err->set_errortype("Bucket Name"); 
			err->set_errorcode("No Bucket");
			err->set_errormessage(m_error);
	}
}

void bucket::get_Bucket_Version(s3service::s3bucket& bucketResponse,database& d)
{	
if(m_bExists){	
	std::vector<std::vector<std::string>> version;
	int retcode = d.executeQuery("SELECT bucketVersion from Msys_Buckets WHERE bucketId = '" + m_bucketId + "'",version);
    
        if (retcode != 0)
        {
            std::cout << "ExecuteQuery Failed" << std::endl;
            return;
        }
    bucketResponse.set_version(version[0][0]);
   }
   	else 
	{
			auto err = bucketResponse.add_errorinfo();
			m_error = "Bucket Name does not exist" ;
			err->set_errortype("Bucket Name"); 
			err->set_errorcode("No Bucket");
			err->set_errormessage(m_error);
	}
}

std::string bucket::getBucketName()
{
	return bucket::m_bucketName;
}

std::string bucket::getBucketId()
{
	return m_bucketId;
}

std::string bucket::getArn()
{
	return m_arn;
}

std::string bucket::getAccountId()
{
	return m_accountId;
}

std::string bucket::getCreationTimestamp()
{
	return m_creationTimestamp;
}

std::map<std::string, std::string> bucket::getTags()
{
	return m_tags;
}

std::string bucket::getErrorMessage()
{
	return m_error;
}

bool bucket::doesBucketExists()
{
	return m_bExists;
}

std::string bucket::getBucketVersion()
{
    return m_bucketVersion;
}


bucket::~bucket()
{

}
