#include "handler.hpp"
#include "consumer.hpp"
#include "publisher.hpp"
#include "S3Exception.h"
#include "mssql.hpp"
//#include "s3service.pb.h"
#include <iostream>
#include <thread>
#include <future>

#include <unistd.h>

/*#define EXPIRY_LIMIT 0
#define DIFF_BETWEEN_DATE 1
#define BUCKETNAME 2 
#define OBJECTNAME 3
#define OBJECTID 4*/


bool initDatabase(database& d)
{
	auto check = d.createConnection("MSSQLTest", "SA", "Master#123");

	if (check != "")
	{
		std::cout << "Database create connection failed: " << check << std::endl;
		return false;
	}

	std::vector<std::vector<std::string>> result;

	auto ret = d.executeQuery("select name from sys.databases where name = 'Msys_Db'", result);

	if (ret == -1)
	{
		std::cout << "Errno while checking for MsysDatabase in system" << std::endl;
		return false;
	}

	if (result.empty())
	{
		ret = d.executeQuery("create database Msys_Db");

		if (ret == -1)
		{
			std::cout << "Errno while creating MsysDb" << std::endl;
			return false;
		}
	}
	else
	{
		result.clear();
	}

	ret = d.executeQuery("use Msys_Db");

	if (ret == -1)
	{
		std::cout << "Error 'use Msys_Db'" << std::endl;
		return false;
	}

	ret = d.executeQuery("select name from sys.tables where name = 'Msys_Accounts'", result);

	if (ret == -1)
	{
		std::cout << "Error while checking sys.tables for Msys_Accounts" << std::endl;
		return false;
	}

	if (result.empty())
	{
		ret = d.executeQuery("create table Msys_Accounts(accountId varchar(255) PRIMARY KEY, accountName varchar(255) NOT NULL, password varchar(255) NOT NULL)");

		if (ret == -1)
		{
			std::cout << "Error while creating Msys_Accounts table" << std::endl;
			return false;
		}
	}
	else
	{
		result.clear();
	}

	ret = d.executeQuery("select name from sys.tables where name = 'Msys_Users'", result);

	if (ret == -1)
	{
		std::cout << "Error while checking sys.tables for Msys_Users" << std::endl;
		return false;
	}

	if (result.empty())
	{
		ret = d.executeQuery("create table Msys_Users(" \
				"userId varchar(255) PRIMARY KEY, " \
				"userName varchar(255) NOT NULL, " \
				"arn varchar(255) NOT NULL, " \
				"timestamp varchar(255) NOT NULL, " \
				"path varchar(255) NOT NULL, " \
				"accountId varchar(255) FOREIGN KEY REFERENCES Msys_Accounts(accountId))"
				);

		if (ret == -1)
		{
			std::cout << "Error while creating Msys_Users table" << std::endl;
			return false;
		}
	}
	else
	{
		result.clear();
	}

	ret = d.executeQuery("select name from sys.tables where name = 'Msys_Access_Keys'", result);

	if (ret == -1)
	{
		std::cout << "Error while checking sys.tables for Msys_Access_Keys" << std::endl;
		return false;
	}

	if (result.empty())
	{
		ret = d.executeQuery("create table Msys_Access_Keys(" \ 
				"accessKeyId varchar(255) PRIMARY KEY, " \
				"secretKey varchar(255) NOT NULL, " \
				"creationTimestamp varchar(255) NOT NULL, " \
				"lastUsedTimestamp varchar(255), " \
				"status varchar(255) NOT NULL, " \
				"accountId varchar(255) FOREIGN KEY REFERENCES Msys_Accounts(accountId), " \
				"userId varchar(255) FOREIGN KEY REFERENCES Msys_Users(userId))"
				);

		if (ret == -1)
		{
			std::cout << "Error while creating Msys_Access_Keys table" << std::endl;
			return false;
		}
	}
	else
	{
		result.clear();
	}

	ret = d.executeQuery("select name from sys.tables where name = 'Msys_Buckets'", result);

	if (ret == -1)
	{
		std::cout << "Error while checking sys.tables for Msys_Buckets" << std::endl;
		return false;
	}
	//"accountId varchar(255) NOT NULL //FOREIGN KEY REFERENCES Msys_Accounts(accountId), "
	if (result.empty())
	{
		ret = d.executeQuery("create table Msys_Buckets(" \
				"bucketId varchar(255) PRIMARY KEY, " \
				"bucketName varchar(255) NOT NULL UNIQUE, " \
				"bucketVersion varchar(255) NOT NULL,"\
				"arn varchar(255) NOT NULL, " \
				"creationTimestamp varchar(255) NOT NULL, " \
				"accountId varchar(255) NOT NULL, "\  
				"expiry_limit varchar(255) , "\
				"Memory_space_IN_bytes varchar(255))");

		if (ret == -1)
		{
			std::cout << "Error while creating Msys_Buckets table" << std::endl;
			return false;
		}
	}
	else
	{
		result.clear();
	}

	ret = d.executeQuery("select name from sys.tables where name = 'Msys_Bucket_Tags'", result);

	if (ret == -1)
	{
		std::cout << "Error while checking sys.tables for Msys_Bucket_Tags" << std::endl;
		return false;
	}

	if (result.empty())
	{
		ret = d.executeQuery("create table Msys_Bucket_Tags(" \
				"bucketKey varchar(255) NOT NULL, " \
				"bcuketValue varchar(255) NOT NULL, " \
				"bucketId varchar(255) FOREIGN KEY REFERENCES Msys_Buckets(bucketId))"
				);

		if (ret == -1)
		{
			std::cout << "Error while creating Msys_Bucket_Tags table" << std::endl;
			return false;
		}
	}
	else
	{
		result.clear();
	}
	
	ret = d.executeQuery("select name from sys.tables where name = 'Msys_DeleteMarker'", result);


	if (ret == -1)
	{
		std::cout << "Error while checking sys.tables for Msys_DeleteMarker" << std::endl;
		return false;
	}

	if (result.empty())
	{
		ret = d.executeQuery("create table Msys_DeleteMarker(" \
				"objectId varchar(255) PRIMARY KEY, " \
				"VersionStatus varchar(255) NOT NULL, "\
				"objectName varchar(255) NOT NULL, " \
				"objectsize varchar(255) NOT NULL, "\
				"objectVersion varchar(255), "\
				"creationTimestamp varchar(255) NOT NULL, " \
				"bucketId varchar(255) FOREIGN KEY REFERENCES Msys_Buckets(bucketId), "\
				"deleteMarkerID varchar(255) NOT NULL, "\
				"bucketName varchar(255) NOT NULL,"
				"DeletedCreationTimestamp varchar(255))");

		if (ret == -1)
		{
			std::cout << "Error while creating Msys_Bucket_Tags table" << std::endl;
			return false;
		}
	}
	else
	{
		result.clear();
	}

		/*ret = d.executeQuery("select name from sys.tables where name = 'Msys_Multipartupload'", result);


	if (ret == -1)
	{
		std::cout << "Error while checking sys.tables for Msys_DeleteMarker" << std::endl;
		return false;
	}

	if (result.empty())
	{
		ret = d.executeQuery("create table Msys_Multipartupload(" \
				"parts varchar(255),"\
				"objectName varchar(255))");

		if (ret == -1)
		{
			std::cout << "Error while creating Msys_Multipartupload table" << std::endl;
			return false;
		}
	}
	else
	{
		result.clear();
	}*/
	
	ret = d.executeQuery("select name from sys.tables where name = 'Msys_Objects'", result);

	if (ret == -1)
	{
		std::cout << "Error while checking sys.tables for Msys_Objects" << std::endl;
		return false;
	}

	if (result.empty())
	{
	   /*	ret = d.executeQuery("create table Msys_Objects(" \
				"objectId varchar(255) PRIMARY KEY, " \
				"VersionStatus varchar(255) NOT NULL, "\
				"objectName varchar(255) NOT NULL, " \
				"objectsize varchar(255) NOT NULL, "\
				"objectVersion varchar(255), "\
				"creationTimestamp varchar(255) NOT NULL, " \
				"CommonId varchar(255),"\
				"path varchar(255),"\
				"bucketId varchar(255) FOREIGN KEY REFERENCES Msys_Buckets(bucketId),"\
			        "objectLocking varchar(255))"); */



		ret = d.executeQuery("create table Msys_Objects(" \
                                "objectId varchar(255) PRIMARY KEY, " \
                                "VersionStatus varchar(255) NOT NULL, "\
                                "objectName varchar(255) NOT NULL, " \
                                "objectsize varchar(255) NOT NULL, "\
                                "objectVersion varchar(255), "\
                                "creationTimestamp varchar(255) NOT NULL, " \
                                "CommonId varchar(255),"\
                                "path varchar(255),"\
                                "bucketId varchar(255) FOREIGN KEY REFERENCES Msys_Buckets(bucketId),"\
                                "objectLocking varchar(255),"\
                                 "objectRetention varchar(255) NOT NULL)");

		if (ret == -1)
		{
			std::cout << "Error while creating Msys_Objects table" << std::endl;
			return false;
		}
	}

	return true;
}
void checkLifecyleRule(database& d)
{	
	std::vector<std::vector<std::string>> result;
	int ret1 = d.executeQuery("select b.expiry_limit, abs(DateDiff(dd,convert(DATE, UPPER(o.creationTimestamp)), convert(Date,SYSDATETIME()))) expiry_Date, b.bucketName, o.objectName,o.objectId from Msys_Buckets b join Msys_Objects o on b.bucketId = o.bucketId",result);

		if (ret1 == -1)
		{
			std::cout << "ExecuteQuery Failed in fetching Lifecyle rule" << std::endl;
			return;
		}
		int len = result.size();
		vector<string> details;
		for(int i=0;i<len;i++)
		{
					if(result[i][5] != "NULL")
					{
						std::string bucketName = result[i][2];
						std::string ObjectName = result[i][3];
						std::vector<std::vector<std::string>> res;
						int retcode = d.executeQuery("select count(path) from Msys_Objects where objectName= '" + ObjectName + "'", res);
						if (retcode != 0)
						{
							std::cout << "ExecuteQuery Failed" << std::endl;
							return;
						}
						std::vector<std::vector<std::string>> r;
					        retcode = d.executeQuery("select CommonId from Msys_Objects where objectName = '"+ ObjectName +"', AND CommonID <> NULL", r);
					        if (retcode != 0)
						{
							std::cout << "ExecuteQuery Failed" << std::endl;
							return;
						}
						///////////////
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
					        retcode = d.executeQuery("select CommonId from Msys_Objects where objectName = '"+ ObjectName +"' AND CommonID <> 'NULL'", r);
					        	//cout << r[0][0];
					        if (retcode != 0)
						{
							std::cout << "ExecuteQuery Failed 1" << std::endl;
							return;
						}
						
					        if(!r.empty() && r[0][0] !="NULL")
					        {
					        	std::vector<std::vector<std::string>> res1;
					        	retcode = d.executeQuery("select bucketId from Msys_Objects where objectName = '"+ ObjectName + "'AND bucketName <> '"+bucketName+"'",res1);
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
						//////////////
				}
			
					else 
					{
					std::string bucketName = result[i][2];
					std::string ObjectName = result[i][3];
					std::string current_path= get_current_dir_name();
					std::string full_path = current_path + "/upload/" + bucketName + "/" + ObjectName;
					int ret = remove(full_path.c_str());

					if (ret != 0)

					{
						std::cout << "Error in removing file in Lifecyle rule to remove Object" << std::endl;
						return ;
					}
					std::string objectId = result[i][4];
					int retcode = d.executeQuery("DELETE from Msys_Objects Where objectId = '" + objectId + "'");

					if (retcode != 0)
					{
						std::cout << "ExecuteQuery Failed in deleting Lifecyle rule" << std::endl;
						return ;
					}

			      }
		}
}
int main()
{
//	std::string dn, password;
//	std::cout << "Enter the DN for ldap" << std::endl;
//	std::cin >> dn;
//
//	std::cout << "Enter the DN admin password"<<endl;
//	std::cin >> password;
//
//	int dn_len = dn.length();
//	int pwd_len = password.length();
//
//	char DN[dn_len+1];
//	char pwd[pwd_len+1];
//
//	strcpy(DN,dn.c_str());
//	strcpy(pwd,password.c_str());

	bool flag = false;

	consumer cObj("", "", "", "", "requestQueue", "server");

	auto err = cObj.init();

	if (err.size() != 0)
	{
		std::cout << "Error consumer::init(): " << err << "\n";
		return flag;
	}

	err = cObj.bindToExchange("requestExchange", "test");

	if (err.size() != 0)
	{
		std::cout << "Error consumer::bindToExchange(): " << err << "\n";
		return flag;
	}
	publisher pub("", "", "", "", "responseExchange");

	auto headers = pub.getHeaders();

	headers.insert({"Content-type", "text/text"});
	headers.insert({"Content-encoding", "UTF-8"});

	pub.addHeaders(headers);

	err = pub.init();

	if(err.size() != 0)
	{
		std::cout << "Error: " << err << std::endl;
		return false;
	}

	err = pub.createAndBindQueue("responseQueue", "test");

	if(err.size() != 0)
	{
		std::cout << "Error: " << err << std::endl;
		return false;
	}

	serverSocket ssWrite("127.0.0.1", 9090);

	auto ret = ssWrite.createAndBind(3);

	if (ret != "")
	{
		std::cout << "ssWrite.createAndBind() failed" << ret << std::endl;
		return -1;
	}

	ret = ssWrite.acceptConn();

	if (ret != "")
	{
		std::cout << "ssWrite.acceptCon() failed: " << ret << std::endl;
		return -1;
	}

	std::thread th1(&consumer::consumeMessage, &cObj);

	if (err.size() != 0)
	{
		std::cout << "Error consumer::consumeMessage(): " << err << "\n";
		return flag;
	}

	serverSocket ssRead("127.0.0.1", 8081);

	ret = ssRead.createAndBind(3);

	if (ret != "")
	{
		std::cout << "ssRead.createAndBind() failed" << ret << std::endl;
		return -1;
	}

	ret = ssRead.acceptConn();

	if (ret != "")
	{
		std::cout << "ssRead.acceptCon() failed: " << ret << std::endl;
		return -1;
	}

	database d;
	flag = initDatabase(d);

	if (!flag)
	{
		std::cout << "initDatabase() Failed" << std::endl;
		return -1;
	}

	flowHandler hFlow;

//	Ldap ld;
//
//	ld.connect();
//
//	ld.bindServer(DN,pwd);


////////////////////////////////
	//checkLifecyleRule(d);

	while (true)
	{
		
		if (!consumer::m_consumeRequestMsg.empty())
		{
			try
			{
				auto serializedProto = consumer::m_consumeRequestMsg[0];

				consumer::m_consumeRequestMsg.erase(consumer::m_consumeRequestMsg.begin());

				s3service::serviceRequestResponse request, response;

				request.ParseFromString(serializedProto);
				
				hFlow.performAction(ssRead, ssWrite, d, request, response);

				serializedProto = response.SerializeAsString();

				pub.publishMessage(serializedProto, "test");

				sleep(4);
			}
			catch(const std::exception& e)
			{
				
				/*s3service::serviceRequestResponse response;
				response.set_entitytype(s3service::serviceRequestResponse::SERVER);
				auto objectResponse = response.add_server();
				objectResponse->set_serverop(s3service::S3Server::Serverdown);
				auto err = objectResponse->add_errorinfo();
				err->set_errortype("Sender");
				err->set_errorcode("Server got exception");
				err->set_errormessage(e.what());
				auto serializedProto = response.SerializeAsString();

				pub.publishMessage(serializedProto, "test");
				//cout << serializedProto << endl;
				sleep(4);*/
				
				std::cout << e.what() << '\n';
				break;
			}
		}
	}
	th1.join();

	


	return flag;
}
