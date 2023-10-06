#include "publisher.hpp"
#include "consumer.hpp"
#include "s3service.pb.h"
#include "clientsocket.hpp"

#include <iostream>
#include <thread>
#include <cstddef>
#include <experimental/filesystem>
#include <bits/stdc++.h>
#include <vector>

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



#include <crypto++/base64.h>
#include <crypto++/hex.h>
#include <crypto++/rsa.h>
#include <crypto++/aes.h>
#include <crypto++/files.h>
#include <crypto++/randpool.h>
#include <crypto++/validate.h>
#include <crypto++/modes.h>
#include <crypto++/osrng.h>

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#include <sstream>

//#include "auto.hpp"


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




////


/////


#include <iostream>

#define AUTOMATION 0

#if AUTOMATION 
#define AUTOMATION_FEATURE_ENABLED
#endif

//using namespace Pistache;
//using namespace Pistache::Rest;


//using namespace std;

std::string breakclient = "Ok";
int th3stop = 0;
int getobj =0;
std::string bucketName2;
std::string inputFile2;

std::string getRequestId()
{
	std::srand(std::time(nullptr));

	return "id" + std::to_string(std::rand());
}







bool isInteger(const std::string &s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}






int filecount(std::string path)
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
  	cout << endl;
  	return 0;
  }

  return i-2;
}


double sizebyte(std::string objectname) {
	std::string current_path = get_current_dir_name();
	std::string full_path = current_path + "/" + objectname;
	std::ifstream iF(full_path.c_str(), std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
		return false;
	}

	iF.seekg(0, std::ios::end);
	long long int siz=iF.tellg();
    return siz;
  }

auto readFromFile(clientSocket& cs, const std::string& objectPath)
{

	std::ifstream iF(objectPath, std::ios::in | std::ios::binary);

	if (!iF)
	{
		std::cout << "File not found!";
		return false;
	}
	
	const int BUFFER_SIZE = 50000;
	char* buffer = new char[BUFFER_SIZE];
	int bytesRead;
	
	th3stop = 0;
	iF.seekg(0, std::ios::end);
	size_t size = iF.tellg();
	iF.seekg(0, std::ios::beg);
	int a=0;
	int t=1;
	//std::cout<<"uploding"<<std::endl;
	while (iF && (bytesRead = iF.readsome(buffer, BUFFER_SIZE)) > 0) {
        	if(th3stop == 0)
        	{
        	auto ret = cs.writeSocket(buffer, BUFFER_SIZE);
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
        }
    	/*char END_MARKER[] = "!!!EOF!!!";
    	auto ret = cs.writeSocket(END_MARKER, sizeof(END_MARKER));
    	if (ret != "")
	{
		std::cout << ret << std::endl;
		return false;
	}*/
	
	iF.close();
        
	if(th3stop == 0)
	{
		//std::cout<<"upload completed"<< std::endl;
	}
	
	
	/*std::ifstream iF(objectPath, std::ios::in | std::ios::binary);

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
        iF.close();
        //while(till end of file){
	auto ret = cs.writeSocket(data, size);
	//}

	if (ret != "")
	{
		std::cout << ret << std::endl;
		return false;
	}

	objRequest.set_len(size);*/

	

	return true;
}


 std::string splitFile1(const std::string & inputFile, const std::string & bucketName,  const std::string & outputPrefix, int chunkSize)
{
	std::string current_path= get_current_dir_name();
	std::string full_path = current_path + "/multipartupload/" + bucketName + "/" + inputFile ;
	boost::system::error_code ec;
	boost::filesystem::remove_all(full_path.c_str(), ec);
	if (ec)
	{
		std::cout << "Folder Not Deleted" << std::endl;
					
	}
	//cout << inputFile << endl;
	
  	std::ifstream input(inputFile, std::ios::binary);

	full_path = current_path + "/multipartupload/" + bucketName + "/" + inputFile + "/" ;
  	if (input.is_open()) 
  	{
  	    boost::filesystem::create_directories(full_path);
  	    
    	    // Get the file size
	    input.seekg(0, std::ios::end);
	    std::streampos fileSize = input.tellg();
	    input.seekg(0, std::ios::beg);

	    // Calculate the number of chunks
	    int numChunks = (fileSize + chunkSize - 1) / chunkSize;

	    // Read and write each chunk
	    for (int i = 0; i < numChunks; ++i) 
	    {
		      // Create or overwrite the output file
		      std::ofstream output(full_path + outputPrefix +std::to_string(i + 1) + ".txt", std::ios::binary);

		      if (output.is_open()) 
		      {
				std::vector < char > buffer(chunkSize);

				// Read a chunk of data from the input file
				input.read(buffer.data(), chunkSize);

				// Write the chunk to the output file
				output.write(buffer.data(), input.gcount());

				output.close();
      		      } 
      		     else 
      		     {
        		std::cout << "Failed to open output file: " << outputPrefix + std::to_string(i + 1) + ".txt" << std::endl;
        		std::string current_path= get_current_dir_name();
			std::string full_path = current_path + "/multipartupload/" + bucketName + "/" + inputFile + "/";
			boost::system::error_code ec;
			boost::filesystem::remove_all(full_path.c_str(), ec);
			if (ec)
			{
				std::cout << "Folder Not Deleted" << std::endl;
					
			}
        		return "";
      		     }
    	  }

    	  input.close();
    	  return full_path;
  	} 
  	else 
  	{
    		std::cout << "Failed to open the input file." << std::endl;
    		return " ";
  	}
}



 std::string splitFile(const std::string & inputFile,  const std::string & outputPrefix, int chunkSize)
{
	std::string current_path= get_current_dir_name();
	std::string full_path=current_path+"/multipartupload/"+ inputFile + "/";
	boost::filesystem::create_directories(full_path);
  	std::ifstream input(inputFile, std::ios::binary);

  	if (input.is_open()) 
  	{
    	    // Get the file size
	    input.seekg(0, std::ios::end);
	    std::streampos fileSize = input.tellg();
	    input.seekg(0, std::ios::beg);

	    // Calculate the number of chunks
	    int numChunks = (fileSize + chunkSize - 1) / chunkSize;

	    // Read and write each chunk
	    for (int i = 0; i < numChunks; ++i) 
	    {
		      // Create or overwrite the output file
		      std::ofstream output(full_path + outputPrefix +std::to_string(i + 1) + ".txt", std::ios::binary);

		      if (output.is_open()) 
		      {
				std::vector < char > buffer(chunkSize);

				// Read a chunk of data from the input file
				input.read(buffer.data(), chunkSize);

				// Write the chunk to the output file
				output.write(buffer.data(), input.gcount());

				output.close();
      		      } 
      		     else 
      		     {
        		std::cout << "Failed to open output file: " << outputPrefix + std::to_string(i + 1) + ".txt" << std::endl;
        		return "";
      		     }
    	  }

    	  input.close();

    	  //std::cout << "File split successfully." << std::endl;
    	  return full_path;
  	} 
  	else 
  	{
    		std::cout << "Failed to open the input file." << std::endl;
    		full_path = current_path+"/multipartupload/" + inputFile;
    		remove(full_path.c_str());
    		return " ";
  	}
}
void writeToFile(clientSocket csRead, const std::string& filename, int len)
{
	std::string current_path= get_current_dir_name();
	std::string full_path=current_path+"/download/";
	boost::filesystem::create_directories(full_path);
	size_t extension_index = filename.find_last_of(".");
	std::vector<char> data;
	string extension;
	if (extension_index != string::npos)
	{
		extension = filename.substr(extension_index);
	}
	std::ostringstream oss;
	oss << filename.substr(0, extension_index);
	string new_filename = oss.str();
	int i=0;
	string file = new_filename;
	new_filename += extension;
	while(true)
	{
		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(full_path.c_str())) != NULL)
		{
			int flag=0;
			while ((ent = readdir(dir)) != NULL)
			{
				string name(ent->d_name);
				if (name.find(new_filename) == 0)
				{
					i++;
					new_filename = file + "("+to_string(i)+")" + extension;
					flag=1;
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
	full_path = current_path+"/download/"+new_filename;
	std::ofstream oF(full_path.c_str(), std::ios::out | std::ios::binary);

	auto ret = csRead.readSocket(data, len);
	//cout << data << endl;
	if (ret != "")
	{
		std::cout << ret << std::endl;
	}
    oF.write(&data[0], len);
    oF.close(); 
    sleep(7);   
    	current_path= get_current_dir_name();
	std::string full_path1 = current_path+"/download/";
    	std::string p = full_path1 + "decrypt" + extension;
    	//cout << p.c_str() << endl;
    	
    	
    	using namespace CryptoPP;

    	unsigned char *key = (unsigned char *)"01234567890123456789012345678901";
    	unsigned char *iv =  (unsigned char *)"0123456789012345";
    	size_t key_size = strlen((const char*)key);

	CBC_Mode<AES>::Decryption  decryptor(key, key_size, iv);
	//cout << full_path.c_str() << endl;
	//cout << p.c_str() << endl;
    	FileSource(full_path.c_str(), true, new StreamTransformationFilter(decryptor, new FileSink(p.c_str())));
    	remove(full_path.c_str());
    	std::string newname = full_path1+new_filename;
    	if(rename(p.c_str(), newname.c_str())!=0)
	{
		cout << "Failed to rename" << endl;
		return;
	}
}

std::string getObjectName(const std::string& path)
{
	std::experimental::filesystem::path p(path);

	return p.filename().string();
}

bool isNumber(const std::string& memory_space)
{
	for(char const &ch: memory_space)
	{
		if(std::isdigit(ch) == 0 && ch != '.')
			return false;
	}
	return true;
}

void createAndPublishRequestPacket(publisher* pub)
{
	clientSocket cs("127.0.0.1", 8081);
	cs.createAndConnect();
	int sockFd = cs.get_sockfd();

	while (true)
	{
		int putobj = 0;
		int sleeptime = 1;
		getobj = 0;
		std::string objectPath1 = "";
		s3service::serviceRequestResponse request,response;

		std::cout << "Which action you wanna take?:" << std::endl;
		std::cout << "1. Account" << std::endl;
		std::cout << "2. User" << std::endl;
		std::cout << "3. AccessKey" << std::endl;
		std::cout << "4. Bucket" << std::endl;
		std::cout << "5. Object" << std::endl;
		//std::cout << std::endl;
		//auto entityType = response.entitytype();
		//std::cout << entityType << std::endl;
		
		std::string choice;
		std::cin >> choice;

		if (choice == "1")
		{
			request.set_entitytype(s3service::serviceRequestResponse::ACCOUNT);
			int flag = 0;
			auto accountRequest  = request.add_account();
			while(true)
			{
				std::cout << "Which action?:" << std::endl;
				std::cout << "1. Create Account" << std::endl;
				std::cout << "2. Delete Account" << std::endl;
				std::cout << "3. Go back" << std::endl;
				std::cin >> choice;

				if (choice == "1")
				{
					accountRequest->set_accop(s3service::s3account::CREATE_ACCOUNT);
					break;
				}
				else if(choice == "2")
				{
					accountRequest->set_accop(s3service::s3account::DELETE_ACCOUNT);
					break;
				}
				else if(choice == "3")
				{
					flag = 1;
					break;
				}
				else 
				{
					std::cout << "Enter the valid option from 1 or 2" << std::endl;
					continue;
				}
			}
			
			if(flag == 1)
			{
				continue;
			}
				
			std::string accountName, password;
			while(true){
				std::cout << "Enter account name: ";
				char name;
				int coun=0;
				name = getc(stdin);
				if(name != '\n')
					accountName+=name;
					int i=0;
					
				do{
					name = getc(stdin);
					if(name == '\n' && coun==0)
						cout << "You have entered new line, Please Enter the valid account name" << endl;
					else if(name != '\n')
					{
						accountName+=name;
						coun=1;
					}

				}while(name!='\n');
				if(coun)
					break;
        		
        		}
			accountRequest->set_accountname(accountName);
			while(true){
				std::cout << "Enter password: ";
				char pwd;
				int cou=0;
			        pwd=getc(stdin);
			        if(pwd!='\n')
					password+=pwd;
				
				do{
					pwd=getc(stdin);
					if(pwd!='\n')
					{
						password+=pwd;
						cou=1;
					}
					if(pwd == '\n' && cou==0)
						cout << "You have entered new line, Please Enter the valid account password" << endl;

				}while(pwd!='\n');
				if(cou)
					break;
        		
        		}
			accountRequest->set_password(password);

			if (choice == "1")
			{
				accountRequest->set_accop(s3service::s3account::CREATE_ACCOUNT);
			}
			else if(choice == "2")
			{
				accountRequest->set_accop(s3service::s3account::DELETE_ACCOUNT);
			}
			else 
			{
			}
		}
		else if (choice == "2")
		{
			request.set_entitytype(s3service::serviceRequestResponse::USER);
			int flag = 0;
			auto userRequest  = request.add_user();
			while(true)
			{
				std::cout << "Which action?:" << std::endl;
				std::cout << "1. Create User" << std::endl;
				std::cout << "2. Update User" << std::endl;
				std::cout << "3. Delete User" << std::endl;
				std::cout << "4. Go back" << std::endl;

				std::cin >> choice;
				if (choice == "1")
				{
					userRequest->set_userop(s3service::s3user::CREATE_USER);
					break;
				}
				else if (choice == "2")
				{
					
					std::string newName;
        			while(true){
					std::cout << "Enter New username: ";
					char nameU;
					int cou=0;
					nameU=getc(stdin);
					if(nameU!='\n')
						newName+=nameU;
					
					do{
						nameU=getc(stdin);
						if(nameU!='\n')
						{
							newName+=nameU;
							cou=1;
						}
						if(nameU == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid New username" << endl;

				}while(nameU!='\n');
				if(cou)
					break;
        		
        		}

					userRequest->set_newusername(newName);

					userRequest->set_userop(s3service::s3user::UPDATE_USER);
					break;
				}
				else if(choice == "3")
				{
					userRequest->set_userop(s3service::s3user::DELETE_USER);
					break;
				}
				else if(choice == "4")
				{
					flag = 1;
					break;
				}
				else 
				{
					std::cout << "Enter the valid option from 1 to 3" << std::endl;
					continue;
				}
			}
			if(flag == 1)
			{
				continue;
			}
			std::string accessKey, secretKey;

			std::cout << "Enter Anthorization keys: " << std::endl;
				while(true){
					std::cout << "AccessKey: ";
					char accKey;
					int cou=0;
					accKey=getc(stdin);
					if(accKey!='\n')
						accessKey+=accKey;
					
					do{
						accKey=getc(stdin);
						if(accKey!='\n')
						{
							accessKey+=accKey;
							cou=1;
						}
						if(accKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid AccessKey" << endl;

				}while(accKey!='\n');
				if(cou)
					break;
        		
        		}
        		while(true){
					std::cout << "SecretKey: ";
					char secKey;
					int cou=0;
					secKey=getc(stdin);
					if(secKey!='\n')
						secretKey+=secKey;
					
					do{
						secKey=getc(stdin);
						if(secKey!='\n')
						{
							secretKey+=secKey;
							cou=1;
						}
						if(secKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid SecretKey" << endl;

				}while(secKey!='\n');
				if(cou)
					break;
        		
        		}

			auto auth = userRequest->add_auth();

			auth->set_accesskey(accessKey);
			auth->set_secretkey(secretKey);

			

			std::string userName;
       		while(true){
					std::cout << "Enter userName: ";
					char Uname1;
					int cou=0;
					Uname1=getc(stdin);
					if(Uname1!='\n')
						userName+=Uname1;
					
					do{
						Uname1=getc(stdin);
						if(Uname1!='\n')
						{
							userName+=Uname1;
							cou=1;
						}
						if(Uname1 == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid userName" << endl;

				}while(Uname1!='\n');
				if(cou)
					break;
        		
        		}

			userRequest->set_username(userName);


		}
		else if (choice == "3")
		{
			request.set_entitytype(s3service::serviceRequestResponse::ACCESSKEY);
			int flag = 0;
			auto keyRequest  = request.add_accesskey();
			std::string accessKey,secretKey;
			while(true)
			{
				std::cout << "Which action?:" << std::endl;
				std::cout << "1. Create Key" << std::endl;
				std::cout << "2. Delete Key" << std::endl;
				std::cout << "3. Change Status" << std::endl;
				std::cout << "4. Get Last Used Time" << std::endl;
				std::cout << "5. Go back" << std::endl;
				std::cin >> choice;
				if (choice == "1")
				{
					keyRequest->set_accessop(s3service::s3accesskey::CREATE_KEY);
					break;
				}
				else if (choice == "2")
				{
					
				while(true){
					std::cout << "Enter accessKeyId: ";
					char accKey;
					int cou=0;
					accKey=getc(stdin);
					if(accKey!='\n')
						accessKey+=accKey;
					
					do{
						accKey=getc(stdin);
						if(accKey!='\n')
						{
							accessKey+=accKey;
							cou=1;
						}
						if(accKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid accessKeyId" << endl;

				}while(accKey!='\n');
				if(cou)
					break;
        		
        		}

					keyRequest->set_accesskeyid(accessKey);

					keyRequest->set_accessop(s3service::s3accesskey::DELETE_KEY);
					break;
			}
				else if (choice == "3")
				{

				while(true){
					std::cout << "Enter accessKeyId ";
					char accKey;
					int cou=0;
					accKey=getc(stdin);
					if(accKey!='\n')
						accessKey+=accKey;
					
					do{
						accKey=getc(stdin);
						if(accKey!='\n')
						{
							accessKey+=accKey;
							cou=1;
						}
						if(accKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid accessKeyId" << endl;

				}while(accKey!='\n');
				if(cou)
					break;
        		
        		}
					keyRequest->set_accesskeyid(accessKey);

					

					std::string status;
					while(true){
					std::cout << "Enter new status:(Active, Inactive) ";
					char sta;
					int cou=0;
					sta=getc(stdin);
					if(sta!='\n')
						status+=sta;
					
					do{
						sta=getc(stdin);
						if(sta!='\n')
						{
							status+=sta;
							cou=1;
						}
						if(sta == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid statusd" << endl;

				}while(sta!='\n');
				if(cou)
					break;
        		
        		}		
					keyRequest->set_status(status);

					keyRequest->set_accessop(s3service::s3accesskey::CHANGE_KEY_STATUS);
					break;
				}
				else if(choice == "4")
				{
				while(true){
					std::cout << "Enter accessKeyId ";
					char accKey;
					int cou=0;
					accKey=getc(stdin);
					if(accKey!='\n')
						accessKey+=accKey;
					
					do{
						accKey=getc(stdin);
						if(accKey!='\n')
						{
							accessKey+=accKey;
							cou=1;
						}
						if(accKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid accessKeyId" << endl;

				}while(accKey!='\n');
				if(cou)
					break;
        		
        		}


				        keyRequest->set_accesskeyid(accessKey);

					keyRequest->set_accessop(s3service::s3accesskey::LAST_USED_TIME);
					break;
				}
				else if(choice == "5")
				{
					flag=1;
					break;
				}
				else 
				{
					std::cout << "Enter the valid option from 1 to 4" << std::endl;
					continue;
				}
			}
			if(flag ==1)
			{
				continue;
			}

			

			std::cout << "Enter Anthorization keys: " << std::endl;
			
			if(choice == "1"){
			while(true){
					std::cout << "Enter accessKeyId ";
					char accKey;
					int cou=0;
					accKey=getc(stdin);
					if(accKey!='\n')
						accessKey+=accKey;
					
					do{
						accKey=getc(stdin);
						if(accKey!='\n')
						{
							accessKey+=accKey;
							cou=1;
						}
						if(accKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid accessKeyId" << endl;

				}while(accKey!='\n');
				if(cou)
					break;
        		
        		}
        		}
        		while(true){
					std::cout << "SecretKey: ";
					char secKey;
					int cou=0;
					secKey=getc(stdin);
					if(secKey!='\n')
						secretKey+=secKey;
					
					do{
						secKey=getc(stdin);
						if(secKey!='\n')
						{
							secretKey+=secKey;
							cou=1;
						}
						if(secKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid SecretKey" << endl;

				}while(secKey!='\n');
				if(cou)
					break;
        		
        		}
			auto auth = keyRequest->add_auth();
			auth->set_accesskey(accessKey);
			auth->set_secretkey(secretKey);

			

			std::string userName;
        		while(true){
					std::cout << "Enter userName: ";
					char uname;
					int cou=0;
					uname=getc(stdin);
					if(uname!='\n')
						userName+=uname;
					
					do{
						uname=getc(stdin);
						if(uname!='\n')
						{
							userName+=uname;
							cou=1;
						}
						if(uname == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid userName" << endl;

				}while(uname!='\n');
				if(cou)
					break;
        		
        		}
			keyRequest->set_username(userName);
		}
		else if (choice == "4")
		{
			request.set_entitytype(s3service::serviceRequestResponse::BUCKET);

			auto bucketRequest  = request.add_bucket();
			int flag = 0;
			while(true)
			{
				std::cout << "Which action?:" << std::endl;
				std::cout << "1. Create Bucket" << std::endl;
				std::cout << "2. Delete Bucket" << std::endl;
				std::cout << "3. Put Bucket Version" << std::endl;
				std::cout << "4. Get Bucket Version" << std::endl;
				std::cout << "5. List Objects of Bucket" << std::endl;  
				std::cout << "6. List Object Versions of Bucket" << std::endl; 
				std::cout << "7. Add Lifecyle Rule for the bucket" << std::endl;
				std::cout << "8. Storage Quota" << std::endl;
				std::cout << "9. Storage Lens" << std::endl;
				std::cout << "10. Go back" << std::endl;  
				/*std::cout << "3. Put Bucket Tag" << std::endl;
				std::cout << "4. Get Bucket Tag" << std::endl;
				std::cout << "5. Put Bucket Version" << std::endl;*/
				std::cin >> choice;
				
				if (choice == "1")
				{
					bucketRequest->set_buckop(s3service::s3bucket::CREATE_BUCKET);
					break;
				}
				else if (choice == "2")
				{
					bucketRequest->set_buckop(s3service::s3bucket::DELETE_BUCKET);
					break;
				}
				else if(choice== "3")
				{
					bucketRequest->set_buckop(s3service::s3bucket::PUT_BUCKET_VERSION);
					break;
				}
				/*else if (choice == 3)
				{
					std::string key, value;

					std::cout << "Enter tag";

					std::cin >> key;
					std::cin >> value;


					auto tag = bucketRequest->add_tag();

					tag->set_key(key);
					tag->set_value(value);

					bucketRequest->set_buckop(s3service::s3bucket::PUT_BUCKET_TAG);
				}*/
				else if(choice== "4")
				{
					bucketRequest->set_buckop(s3service::s3bucket::GET_BUCKET_VERSION);
					break;
				}
				else if(choice== "5")
				{
					bucketRequest->set_buckop(s3service::s3bucket::LIST_OBJECT);
					break;
				}
				else if(choice== "6")
				{
					bucketRequest->set_buckop(s3service::s3bucket::LIST_OBJECT_VERSIONS);
					break;
				}
				else if(choice == "7")
				{
					
					//while(1)
					//{
						std::string expiry_date;
						cout << "Set expiry limit: ";
						cin >> expiry_date;
						//if( expiry_date > "0" && expiry_date <= "10")
						//{
							bucketRequest->set_expiry_date(expiry_date);
							//break;
						//}
						//else
						//{
							//std::cout<<"expiry_date must greater than 0 and less then 10"<< std::endl;
						//}
					
					//}
					
					bucketRequest->set_buckop(s3service::s3bucket::SET_LIFECYCLE_RULE);
					break;
				}
				else if(choice == "8")
				{
					std::string memory_space;
					std::string choices;
					
					cout << "1. Set Memory sapce for bucket in KB" << endl;
					cout << "2. Set Memory sapce for bucket in MB" << endl;
					cout << "3. Set Memory sapce for bucket in GB" << endl;
					cout << "Enter the option: ";
					char memo1;
						
					memo1 = getc(stdin);
					std::string memory_space1;
					int cou1 = 0;
					int cou2 = 0;
					if(memo1 !='\n')
						memory_space1 += memo1;
						
					do{
						memo1 = getc(stdin);
						if(memo1 != '\n')
						{
							memory_space1 += memo1;
							cou1 ++;
							break;
						}
						if(memo1 == '\n' && cou1 == 0)
						{
							cou2++;
							if(cou2 !=4)
								cout << "You have entered new line, Please Enter the valid Number" << endl;
							
						}
						

					}while(memo1 != '\n' || cou2 != 4);
					choices = memory_space1;
					std::string memory;
					//cout << choices << endl;
					if(choices == "1" || choices == "2" || choices == "3")
					{
						char memo;
						cout << "Enter Memory sapce: ";
						//memo = getc(stdin);
						int cou = 0;
						int cou1 = 0;
						memo = getc(stdin);
						if(memo!='\n')
						memory_space += memo;
						
						
						do{
							memo = getc(stdin);
							if(memo != '\n')
							{
								memory_space += memo;
								cou=1;
							}
							if(memo == '\n' && cou == 0)
							{
								cou1++;
								if(cou1 != 4)
									cout << "You have entered new line, Please Enter the valid Number" << endl;
								if(cou1 == 4)
									cout << "Contorl exit" << endl;
							}

						}while((memo != '\n' || cou1 != 4) && (memory_space == "" || memo != '\n') );
						
						cout << memory_space << endl;
						if(memory_space > "0.0" && isNumber(memory_space))
						{
							
							if(choices == "1")
							{
								stringstream m;
								m << memory_space;
								double x = 0;
								m >> x;
								memory = to_string(x * 1000);	
							}
							else if(choices == "2")
							{
								stringstream m;
								m << memory_space;
								double x = 0;
								m >> x;
								memory = to_string(x * 1000000);
							}
							else if (choices == "3")
							{
								stringstream m;
								m << memory_space;
								double x = 0;
								m >> x;
								memory = to_string(x * 1000000000);
								//cout << memory << endl;
							}
							
						}
						else if(memo != '\n')
						{
							cout << "Enter Positive number" << endl;
							flag = 1;
							
						}
						else if(memo == '\n')
						{
							flag = 1;
							continue;
						}
					}
					else if(memo1 != '\n')
					{
						cout << "Enter the vaild option" << endl;
						flag = 1;
						//continue;
					}
					else if(memo1 == '\n')
					{
						flag = 1;
						continue;
					}
					bucketRequest->set_storagequota(memory);
					//cout << bucketRequest->storagequota() << endl;
					bucketRequest->set_buckop(s3service::s3bucket::SET_STORAGE_QUOTA);
					break;
				}
				else if(choice == "9")
				{
					bucketRequest->set_buckop(s3service::s3bucket::GET_STORAGE_LENS);
					break;
				}		
				else if(choice == "10")
				{
					flag = 1;
					break;
				}
				/*else if(choice==4)
				{
					bucketRequest->set_buckop(s3service::s3bucket::GET_BUCKET_TAG);
				}*/
				else
				{
					std::cout << "Enter valid option from 1 to 6 " << std::endl;
					continue;
				}
			
			}
			if(flag == 1)
				continue;
				
			std::string accessKey, secretKey;

			std::cout << "Enter Anthorization keys: " << std::endl;

				
				
				
			while(true){
					std::cout << "AccessKey: ";
					char accKey;
					int cou=0;
					accKey=getc(stdin);
					if(accKey!='\n')
						accessKey+=accKey;
					
					do{
						accKey=getc(stdin);
						if(accKey!='\n')
						{
							accessKey+=accKey;
							cou=1;
						}
						if(accKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid AccessKey" << endl;

				}while(accKey!='\n');
				if(cou)
					break;
        		
        		}

        		while(true){
					std::cout << "SecretKey: ";
					char secKey;
					int cou=0;
					secKey=getc(stdin);
					if(secKey!='\n')
						secretKey+=secKey;
					
					do{
						secKey=getc(stdin);
						if(secKey!='\n')
						{
							secretKey+=secKey;
							cou=1;
						}
						if(secKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid SecretKey" << endl;

				}while(secKey!='\n');
				if(cou)
					break;
        		
        		}
        		
        		int flagout = 0;
        		for(int i=0; i < accessKey.length(); i++)
        		{
        			if(isupper(accessKey[i]))
				{
					cout << "AccessKeys should not have capital letter" << endl;
					cout << endl;
					flagout = 1;
					break;
				}
				
				else if (accessKey[i] == ' ') 
			       {
                                   cout << "AccessKeys should not contain spaces" << endl;
                                   cout << endl;
                                   flagout = 1;
                                   break;
                               }

			       else if (accessKey[i] == '%')
                                {
                                     cout << "AccessKey should not contain % special Characters" << endl;
                                     cout << endl;
                                     flagout = 1;
                                      break;
                                }
                                else if (accessKey[i] == '#')
                                 {
                                      cout << "AccessKey should not contain  #  special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                      break;
                                 }

                                 else if (accessKey[i] == '&')
                                 {
                                      cout << "AccessKey should not contain  &  special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                      break;
                                 }


                                else if (accessKey[i] == '*')
                                 {
                                      cout << "AccessKey should not contain * special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                      break;
                                 }
                                else if (accessKey[i] == '/')
                                 {
                                      cout << "AccessKey should not contain / special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                      break;
                                 }

        		}
        		
        		for(int i=0; i < secretKey.length(); i++)
        		{
        			if(isupper(secretKey[i]))
				{
					cout << "SecretKeys should not have capital letter" << endl;
					cout << endl;
					flagout = 1;
					break;
				}
				
				else if (secretKey[i] == ' ') 
				 {
                                      cout << "secretKey should not contain spaces" << endl;
                                      cout << endl;
                                      flagout = 1;
                                       break;
                                 }

				 else if (secretKey[i] == '%')
                                 {
                                      cout << "secretKey should not contain %  special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                       break;
                                 }
                                 else if (secretKey[i] == '#')
                                 {
                                      cout << "secretKey should not contain  #  special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                       break;
                                 }

                                 else if (secretKey[i] == '&')
                                 {
                                      cout << "secretKey should not contain  &  special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                       break;
                                 }


                                 else if (secretKey[i] == '*')
                                 {
                                      cout << "secretKey should not contain * special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                       break;
                                 }
				 else if (secretKey[i] == '/')

                                 {
                                      cout << "secretKey should not contain / special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                       break;
                                 }
        		}
        		
        		if(flagout ==1)
			{
				continue;
			}

			auto auth = bucketRequest->add_auth();

			auth->set_accesskey(accessKey);
			auth->set_secretkey(secretKey);

			std::string bucketName;
			
			while(true){
					std::cout << "Enter bucket name: ";
					char buck_name;
					int cou=0;
					buck_name=getc(stdin);
					if(buck_name!='\n')
						bucketName+=buck_name;
					
					do{
						buck_name=getc(stdin);
						if(buck_name!='\n')
						{
							bucketName+=buck_name;
							cou=1;
						}
						if(buck_name == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid bucket name" << endl;

				}while(buck_name!='\n');
				if(cou)
					break;
        		
        		}
			bucketRequest->set_bucketname(bucketName);


		}
		else if (choice == "5")
		{	
			request.set_entitytype(s3service::serviceRequestResponse::OBJECT);
			int flag = 0;
			int flagout = 0;
			auto objectRequest  = request.add_object();
			std::string m_objname;
			std::string m_bucketname;
			while(true)
			{
				std::cout << "Which action?:" << std::endl;
				std::cout << "1. Put Object" << std::endl;
				std::cout << "2. Delete Object" << std::endl;
				std::cout << "3. Get Object"  << std::endl;
				std::cout << "4. Get Object by version" << std::endl;
				std::cout << "5. List Delete Marker Object" << std::endl;
				std::cout << "6. Get Delete Marker Object" << std::endl;
				//std::cout << "7. Multi Part upload" << std::endl;
				std::cout << "7. Initialize Multipat Objects" << std::endl;
				std::cout << "8. Put Multipart Objects (Upload part Numbers)"  << std::endl;
				std::cout << "9. Complete Multipart Objects"  << std::endl;
				std::cout << "10. Abort Multipart Objects"  << std::endl;
				std::cout << "11. List Multipart Objects"  << std::endl;

				std::cout <<"12.Put ObjectLocking"<<std::endl;

			   	std::cout <<"13. Set ObjectRetention"<<std::endl;
				std::cout << "14. Go back" << std::endl;
				std::cin >> choice;
				std::string objectName;
				if (choice == "1")
			       {
			       	
			       	
			       				
					while(true)
					{
						std::cout << "Enter ObjectName: ";
						char Obj_name;
						int cou=0;
						int flag1=0;
						Obj_name=getc(stdin);
						if(Obj_name!='\n')
							objectPath1+=Obj_name;
					
						do{
							Obj_name=getc(stdin);
							if(Obj_name!='\n')
							{
								objectPath1+=Obj_name;
								cou=1;
							}
							if(Obj_name == '\n' && cou==0)
								cout << "You have entered new line, Please Enter the valid ObjectName" << endl;

						  }while(Obj_name!='\n');
				
						/*auto ret = readFromFile(cs, objectPath1, *objectRequest);
							
						if (ret == false)
						{
							std::cout << "readFromFile failed" << std::endl;
							objectPath1="";
							continue;
						}*/
							
						if(cou)
							break;
					
        				}
        				//std::string full_path = get_current_dir_name();
					std::ifstream iF(objectPath1, std::ios::in | std::ios::binary);

					if (!iF)
					{
						std::cout << "File not found!";
						continue;
					}
					iF.seekg(0, std::ios::end);
					size_t size = iF.tellg();
					iF.seekg(0, std::ios::beg);
					objectRequest->set_len(size);
					iF.close();
					putobj = 1;
					
					
					objectRequest->set_objectop(s3service::s3object::PUT_OBJECT);
					objectName = getObjectName(objectPath1);
					objectRequest->set_objectname(objectName);
					break;
			     	}
				else if (choice == "2")
				{
					objectRequest->set_objectop(s3service::s3object::DELETE_OBJECT);
					break;
				}
				else if (choice == "3")
				{
					objectRequest->set_objectop(s3service::s3object::GET_OBJECT);
					sleeptime=0;
					break;
					
				}
				else if(choice == "4")
				{
					std::string versionId1;
					while(true)
					{
						std::cout << "Enter Object Version: ";
						char versionid;
						int cou=0;
						versionid = getc(stdin);
						if(versionid != '\n')
							versionId1 += versionid;
							
						do{
							versionid = getc(stdin);
							if(versionid != '\n')
							{
								versionId1 += versionid;
								cou=1;
							}
							if(versionid == '\n' && cou==0)
								cout << "You have entered new line, Please Enter the valid Object Version" << endl;

						}while(versionid !='\n');
						if(cou)
							break;
					
					}
					objectRequest->set_getobjectbyversionid(versionId1);
					objectRequest->set_objectop(s3service::s3object::GET_OBJECT_BY_VERSION);
					break;
				}
				else if(choice == "5")
				{
					objectRequest->set_objectop(s3service::s3object::LIST_DELETE_MARKER_OBJECT);
					break;
				}
				else if(choice == "6")
				{
					std::string versionId1;
					while(true)
					{
						std::cout << "Enter Object Delete MarkerId: ";
						char versionid;
						int cou=0;
						versionid = getc(stdin);

						if(versionid != '\n')
							versionId1 += versionid;
							
						do{
							versionid = getc(stdin);
							if(versionid != '\n')
							{
								versionId1 += versionid;
								cou=1;
							}
							if(versionid == '\n' && cou==0)
								cout << "You have entered new line, Please Enter the valid Object Version" << endl;

						}while(versionid !='\n');
						if(cou)
							break;
					
					}
					objectRequest->set_markerid(versionId1);
					objectRequest->set_objectop(s3service::s3object::GET_DELETE_MARKER_OBJECT);
					break;
				}
				//////********* Below one is multipart upload you can uncommand to check it running********//////
				/*else if(choice == "7")
				{
					std:: cout << "Object extension must be .txt" << endl;
					std::cout << endl;
					std::cout << "Enter ObjectName: ";
					std::string objectPath1;
					char Obj_name;
					int cou=0;
					int flag1=0;
					Obj_name=getc(stdin);
					if(Obj_name!='\n')
					objectPath1+=Obj_name;
							
					do{
						Obj_name=getc(stdin);
						if(Obj_name!='\n')
						{
							objectPath1+=Obj_name;
							cou=1;
						}
						if(Obj_name == '\n' && cou==0)
						cout << "You have entered new line, Please Enter the valid ObjectName" << endl;

					}while(Obj_name!='\n');
					size_t extension_index = objectPath1.find_last_of(".");
					string extension;
					if (extension_index != string::npos)
					{
						extension = objectPath1.substr(extension_index);
					}
					if(extension != ".txt")
					{
						cout << "filename must .txt" << endl;
						flag = 1;
						break;
					}
					if(extension == ".txt")
					{
						std::string outputPrefix = "part_";
						std::ifstream iF(objectPath1.c_str(), std::ios::in | std::ios::binary);

						if (!iF)
						{
							std::cout << "File not found!";
							break;
						}

						iF.seekg(0, std::ios::end);
						int siz=iF.tellg();
						ostringstream str1;
						str1 << siz;
						
						cout << str1.str() << endl;
						objectRequest->set_filesize(str1.str());
					
						std::string path = splitFile(objectPath1,outputPrefix, 1000000); 
						//cout << path << endl;
						int filenumbers = filecount(path);
						//cout << filenumbers << endl;
						//cout << filenumbers << endl;
						int i;
						for(i=0; i<filenumbers; i++)
						{
							std::string objectpath = path + "part_" + std::to_string(i + 1) + ".txt";
							//cout << objectpath << endl;
							readFromFile(cs, objectpath,  *objectRequest);
						}
						cout << i << endl;
						int ret=remove(path.c_str());
						objectRequest->set_objectname(objectPath1);
						objectRequest->set_objectop(s3service::s3object::PUT_MULTIPART_OBJECT);
					}
					break;
					
				}*/
		
				else if(choice=="7")
				{
					//removeall(objectname);

							std::string objectPath1;
							char Obj_name;
							int cou =0;
							int flag1=0;
							
							while(flag1 != 1)
							{
								std:: cout << "Object extension must be .txt" << endl;
								std::cout << endl;
								std::cout << "Enter ObjectName: ";
								
								
								Obj_name = getc(stdin);
								if(Obj_name!='\n')
									objectPath1 += Obj_name;
								
								do{
									Obj_name=getc(stdin);
									if(Obj_name!='\n')
									{
										objectPath1+=Obj_name;
										cou=1;
									}
									if(Obj_name == '\n' && cou==0)
									cout << "You have entered new line, Please Enter the valid ObjectName" << endl;

								}while(Obj_name!='\n');
								if(cou)
									break;
								if(cou == 3)
									flag1 = 1;
					
							}
							int fl = 0;
							for(auto i:objectPath1)
							{
								if(i == ' ')
								{
									cout << "name should not have space" << endl;
									fl = 1;
									break;
								}
							}
							
							if ( fl )
								continue;
							std::string bucketName;
					
							while(true)
							{
								std::cout << "Enter bucket name: ";
								char buck_name;
								int cou=0;
								buck_name=getc(stdin);
								if(buck_name!='\n')
									bucketName+=buck_name;
								
								do{
									buck_name = getc(stdin);
									if(buck_name!='\n')
									{
										bucketName+=buck_name;
										cou=1;
									}
									if(buck_name == '\n' && cou==0)
										cout << "You have entered new line, Please Enter the bucket name" << endl;

								}while(buck_name!='\n');
								if(cou)
									break;
					
							}
							fl = 0;
							for(auto i:bucketName)
							{
								if(i == ' ')
								{
									cout << "name should not have space" << endl;
									fl = 1;
									break;
								}
							}
							
							if ( fl )
								continue;
							
							flagout = 0;
							for(int i=0; i< bucketName.length(); i++)
							{
								if(isupper(bucketName[i]))
								{
									cout << "Bucket Name should not have capital letter" << endl;
									cout << endl;
									flagout = 1;
									break;
								}
							}
							if(flagout ==1)
							{
								continue;
							}
							bucketName2 = bucketName;
							inputFile2  = objectPath1;
							objectRequest->set_bucketname(bucketName);
							size_t extension_index = objectPath1.find_last_of(".");
							string extension;
							if (extension_index != string::npos)
							{
								extension = objectPath1.substr(extension_index);
							}
							if(extension != ".txt")
							{
								cout << "filename must .txt" << endl;
								flag = 1;
								continue;
							}
							if(extension == ".txt")
							{
														
								m_objname = objectPath1;/////////////////////////////
								m_bucketname = bucketName;
								std::string outputPrefix = "part_";
								//std::string current_path1 = get_current_dir_name();
								double filesizeinbytes1 = sizebyte(objectPath1); 
								double userselpart;
								//double check;
								int option;
								int cou1 = 0;
								cout << "how to split files" << endl;
								cout << "1. By part split" << endl;
								cout << "2. By chucksize" << endl;
								double par;
								cin >> option;
								if( option == 1)
								{
									string userselpart1;
									int c=0;
									while(true)
									{	
										cou =0;
										cou1 = 0;
										userselpart1 = "";
										cout << "Select how many part need to have: ";
										char parin;
		 								parin = getc(stdin);
										if(parin!='\n')
											userselpart1 += parin;
										
										do{
											parin = getc(stdin);
											if(parin !='\n')
											{
												userselpart1 += parin;
												cou=1;
											}
											if(parin == '\n' && cou == 0)
											{
												cou1++;
												if(cou1 !=4)
													cout << "You have entered new line, Please Enter the Valid number" << endl;
												else if(cou1 ==4)
													cout << "Control exit" << endl;
											}
											else if(parin =='\b')
												cout << "You have entered space, Please Enter the valid number" << endl;
											if(parin <= '0' && parin !='\n')
											{
												cout << "Number should be positive" << endl;
												//continue;
											}

										}while((parin!='\n' || cou1 != 4 ) && (userselpart1 =="" || parin!='\n'));	
										
										if(userselpart1 != "\n" && cou1 != 4 && isInteger(userselpart1))
										{
											userselpart = stoi(userselpart1);
											if(userselpart <= 0)
											{
												cout << "Enter a positive numebr" << endl;
												
											}}
										else {
											cout << "Invalid input. Please enter a valid positive number." << endl;
											
										}
										c++;
										if(userselpart >= 1)
											break;
										if(c == 2)
											break;
										
									}
									if(userselpart1 == "")
									{
										continue;
									}
									
									if(userselpart1 != "\n")
									{
										userselpart = stoi(userselpart1);
										
										if(userselpart <= 0)
										{
											continue;
										}
									}

									if(userselpart <= 0)
									{
										continue;
									}
									//if(userselpart <= 0)
									//{
										//cout << "Number should be positive" << endl;
										//continue;
									//}
									userselpart = stoi(userselpart1);
									par = filesizeinbytes1 / userselpart;
								}
								else if(option == 2)
								{
									string userselpart1;
									int c=0;
									while(true)
									{
										userselpart1="";
										cout << "Min size would be 2mb max will be half of the size of the file" << endl;
										char parin;
										
										cou = 0;
										cou1 = 0;		     								        		parin = getc(stdin);
										if(parin !='\n')
											userselpart1 += parin;
													
										do{
											parin = getc(stdin);
											if(parin !='\n')
											{
												userselpart1 += parin;
												cou=1;
											}
											
											if(parin == '\n' && cou == 0)
											{
												cou1++;
												if(cou1 !=4)
													cout << "You have entered new line, Please Enter the Valid number" << endl;
												else if(cou1 ==4)
													cout << "Control exit" << endl;
											}
										}while((parin!='\n' || cou1 != 4 ) && (userselpart1 == "" || parin!= '\n'));
										if(userselpart1 != "\n" && cou1 != 4 && isInteger(userselpart1))
										{
											userselpart = stoi(userselpart1);
											if(userselpart <= 0)
											{
												cout << "Enter a positive numebr" << endl;
												
											}
											
										}else {
                    cout << "Invalid input. Please enter a valid positive number." << endl;
                }

										c++;
										double maxlim = (filesizeinbytes1/2);
										double che = userselpart * 1000000;

										
									
									
										if(userselpart >= 1 && (che < 2000000 || che > maxlim))
											break;
										else if((che < 2000000 || che > maxlim))
										{
											cout << "File cannot split please choose the correct chuck size min of 2mb and max of half of the file size" << endl;
										}
										
											
										if(c == 2)
											break;

									}
									if(userselpart1 == "")
									{	
										continue;
									}


									if(userselpart1 != "\n" && isNumber(userselpart1))
									{
										userselpart = stoi(userselpart1);
									}
									 								
									if(userselpart <= 0)
									{
										//cout << "Number should be positive" << endl;
										continue;
									}
									
									//cout << filesizeinbytes1 << endl;

									
									par = userselpart * 1000000;
									
								}
								else 
								{
									cout << "Enter a vaild option" << endl;
									continue;
								}
								std::string path = splitFile1(objectPath1, bucketName, outputPrefix, par); 
								if(path ==  " ")
								{
									cout << endl;
									 continue;
								}
								int filenumbers = filecount(path);
								objectRequest->set_objectname(objectPath1);
							}
							long long int filesizeinbytes = sizebyte(objectPath1);
							objectRequest->set_file(to_string(filesizeinbytes));
							objectRequest->set_objectop(s3service::s3object::INIT_MULTIPART_OBJECT);
							
							break;
					}
					
					else if(choice=="8")
					{
						std::string objname;
						//cout << "Enter the object name: ";
						//cin >> objname;
						char Obj_name;
						int cou=0;
						int flag1=0;
						while(flag != 1)
						{
							std:: cout << "Object extension must be .txt" << endl;
							std::cout << endl;
							std::cout << "Enter ObjectName: ";
							
								
							Obj_name = getc(stdin);
							if(Obj_name != '\n')
								objname += Obj_name;
								
							do{
								Obj_name = getc(stdin);
								if(Obj_name != '\n')
								{
									objname += Obj_name;
									cou=1;
								}
								if(Obj_name == '\n' && cou==0)
									cout << "You have entered new line, Please Enter the valid ObjectName" << endl;

							}while(Obj_name!='\n');
								if(cou)
									break;
								if(cou == 3)
									flag = 1;
									
					
						}
						std::string parts;
						//cin >> parts;
						//std::string objectPath1;
						//char Obj_name;
						cou=0;
						flag1=0;
						while(flag != 1)
						{
							std:: cout << "Enter the object part: " ;
							Obj_name = getc(stdin);
							if(Obj_name != '\n')
								parts += Obj_name;
								
							do{
								Obj_name = getc(stdin);
								if(Obj_name != '\n')
								{
									parts += Obj_name;
									cou=1;
								}
								if(Obj_name == '\n' && cou==0)
									cout << "You have entered new line, Please Enter the valid ObjectName" << endl;

							}while(Obj_name!='\n');
								if(cou)
									break;
								if(cou == 3)
									flag = 1;
									
					
						}
						
							std::string bucketName;
					
							while(true)
							{
								std::cout << "Enter bucket name: ";
								char buck_name;
								int cou=0;
								buck_name=getc(stdin);
								if(buck_name!='\n')
									bucketName+=buck_name;
								
								do{
									buck_name=getc(stdin);
									if(buck_name!='\n')
									{
										bucketName+=buck_name;
										cou=1;
									}
									if(buck_name == '\n' && cou==0)
										cout << "You have entered new line, Please Enter the bucket name" << endl;

								}while(buck_name!='\n');
								if(cou)
									break;
					
							}
							
							flagout = 0;
							for(int i=0; i< bucketName.length(); i++)
							{
								if(isupper(bucketName[i]))
								{
									cout << "Bucket Name should not have capital letter" << endl;
									cout << endl;
									flagout = 1;
									break;
								}
							}
							if(flagout ==1)
							{
								continue;
							}
							objectRequest->set_bucketname(bucketName);
						
						objectPath1= "multipartupload/" +  bucketName + "/" + objname + "/" + parts;
						std::ifstream iF(objectPath1, std::ios::in | std::ios::binary);
						if (!iF)
						{
							std::cout << "File not found!";
							continue;
						}
						iF.seekg(0, std::ios::end);
						size_t size = iF.tellg();
						iF.seekg(0, std::ios::beg);
						objectRequest->set_len(size);
						iF.close();
						putobj = 1;
						/*auto ret = readFromFile(cs, objectPath);
						if (ret == false)
						{
							std::cout << "readFromFile failed" << std::endl;
							continue;
						}*/
						std::string objpat = objname + "/" + parts;
						objectRequest->set_objectname(objpat);
						objectRequest->set_objectop(s3service::s3object::PUT_MULTIPART);
						break;
							
					}
					else if(choice=="9")
					{
						std::string current_path = get_current_dir_name();
						std::string objname;
						//cout << "Enter the object name: ";
						//cin >> objname;
						char Obj_name;
						int cou=0;
						int flag1=0;
						while(flag != 1)
						{
							std:: cout << "Object extension must be .txt" << endl;
							std::cout << endl;
							std::cout << "Enter ObjectName: ";
							
								
							Obj_name = getc(stdin);
							if(Obj_name != '\n')
								objname += Obj_name;
								
							do{
								Obj_name = getc(stdin);
								if(Obj_name != '\n')
								{
									objname += Obj_name;
									cou=1;
								}
								if(Obj_name == '\n' && cou==0)
									cout << "You have entered new line, Please Enter the valid ObjectName" << endl;

							}while(Obj_name!='\n');
								if(cou)
									break;
								if(cou == 3)
									flag = 1;
						}
						
						       std::string bucketName;
					
							while(true)
							{
								std::cout << "Enter bucket name: ";
								char buck_name;
								int cou=0;
								buck_name=getc(stdin);
								if(buck_name!='\n')
									bucketName+=buck_name;
								
								do{
									buck_name=getc(stdin);
									if(buck_name!='\n')
									{
										bucketName+=buck_name;
										cou=1;
									}
									if(buck_name == '\n' && cou==0)
										cout << "You have entered new line, Please Enter the bucket name" << endl;

								}while(buck_name!='\n');
								if(cou)
									break;
					
							}
							
							flagout = 0;
							for(int i=0; i< bucketName.length(); i++)
							{
								if(isupper(bucketName[i]))
								{
									cout << "Bucket Name should not have capital letter" << endl;
									cout << endl;
									flagout = 1;
									break;
								}
							}
							if(flagout ==1)
							{
								continue;
							}
							
							objectRequest->set_bucketname(bucketName);
						

						objectRequest->set_objectname(objname);
						std::string path = current_path + "/multipartupload/" + bucketName + "/" + objname + "/" ;
						
						int filenumbers = filecount(path);
						if(filenumbers ==0)
						{
							continue;
						}
						objectRequest->set_filecount(to_string(filenumbers));
						/*boost::system::error_code ec;
						boost::filesystem::remove_all(path.c_str(), ec);
						if (ec)
						{
							std::cout << "Folder Not Deleted" << std::endl;
							
						}*/
						//int ret = remove(path.c_str());
						objectRequest->set_objectop(s3service::s3object::COMPLETE_MULTIPART_OBJECT);
						break;
						
					}

					else if (choice=="10")
					{
						objectRequest->set_objectop(s3service::s3object::ABORT_MULTIPART_OBJECT);
						break;

					}
						
						
					else if (choice=="11")
					{
						objectRequest->set_objectop(s3service::s3object::LIST_MULTIPART_OBJECT);
						break;
					}

				        else if (choice == "12")
						 {
							    objectRequest->set_objectop(s3service::s3object::PUT_OBJECT_LOCKING);// it send request
							     
							       break;
						 }

                                        
              				else if (choice == "13")
				       	{
						   
						   
							std::cout << "Enter retention date (mm/dd/yyyy): ";

  						    std::string retentionDate;
						    std::cin >> retentionDate;
					// std::cout<<"input retentiondate in clientside"<<retentionDate<<std::endl;


						// Use regular expressions to validate the date format (mm/dd/yyyy)
						

					       std::regex dateRegex("(0[1-9]|1[0-2])/(0[1-9]|[12][0-9]|3[01])/\\d{4}");


						if (!std::regex_match(retentionDate, dateRegex)) {
   							 std::cout << "Invalid date format. Please use mm/dd/yyyy format." << std::endl;
   							 // Handle the error or prompt the user to enter the date again.
								} else {
   								 // Date format is correct then we can proceed to set the retention date.
   								 objectRequest->set_objectop(s3service::s3object::SET_OBJECT_RETENTION);
 								   objectRequest->set_retention_duration(retentionDate);

								 //  std::cout<<"retentiondate in clientside"<<retentionDate<<std::endl;
  									  break;
								}
							}
					
			    
					else if(choice == "14")
					{	
						 flag = 1;
						 break;
					}
					else 
					{
						std::cout << "Invalid option Please enter from 1 to 12" << std::endl;
						continue;
				        }

				}//while end;
				
				if(flag==1)
				{
					continue;
				}
					    		
				if(flagout ==1)
				{
					continue;
				}
			

				std::string accessKey, secretKey;

				std::cout << "Enter Authorization keys: " << std::endl;
				while(true)
				{
					std::cout << "AccessKey: ";
					char accKey;
					int cou=0;
					accKey=getc(stdin);
					if(accKey!='\n')
						accessKey+=accKey;
						
					do{
						accKey=getc(stdin);
						if(accKey!='\n')
						{
							accessKey+=accKey;
							cou=1;
						}
						if(accKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid AccessKey" << endl;

					   }while(accKey!='\n');
					if(cou)
						break;
        		
        			}
        			while(true)
        			{
					std::cout << "SecretKey: ";
					char secKey;
					int cou=0;
					secKey=getc(stdin);
					if(secKey!='\n')
						secretKey+=secKey;
						
					do{
						secKey=getc(stdin);
						if(secKey!='\n')
						{
							secretKey+=secKey;
							cou=1;
						}
						if(secKey == '\n' && cou==0)
							cout << "You have entered new line, Please Enter the valid SecretKey" << endl;

					}while(secKey!='\n');
					if(cou)
						break;
				
				}
        		
        		
				for(int i=0; i< accessKey.length(); i++)
				{
					if(isupper(accessKey[i]))
					{
						cout << "AccessKeys should not have capital letter" << endl;
						cout << endl;
						flagout = 1;
						break;
					}
									
				else if (accessKey[i] == ' ') 
			       {
                                   cout << "AccessKeys should not contain spaces" << endl;
                                   cout << endl;
                                   flagout = 1;
                                   break;
                               }

			       else if (accessKey[i] == '%')
                                {
                                     cout << "AccessKey should not contain % special Characters" << endl;
                                     cout << endl;
                                     flagout = 1;
                                      break;
                                }
                                else if (accessKey[i] == '#')
                                 {
                                      cout << "AccessKey should not contain  #  special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                      break;
                                 }

                                 else if (accessKey[i] == '&')
                                 {
                                      cout << "AccessKey should not contain  &  special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                      break;
                                 }


                                else if (accessKey[i] == '*')
                                 {
                                      cout << "AccessKey should not contain * special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                      break;
                                 }
                                else if (accessKey[i] == '/')
                                 {
                                      cout << "AccessKey should not contain / special Characters" << endl;
                                      cout << endl;
                                      flagout = 1;
                                      break;
                                 }
				}
					
				for(int i=0; i< secretKey.length(); i++)
				{
					if(isupper(secretKey[i]))
					{
						cout << "SecretKeys should not have capital letter" << endl;
						cout << endl;
						flagout = 1;
						break;
					}
									
					 else if (secretKey[i] == ' ') 
				 	 {
                                     	 	cout << "secretKey should not contain spaces" << endl;
                                      	cout << endl;
                                      	flagout = 1;
                                       	break;
                                 	  }

					 else if (secretKey[i] == '%')
		                         {
		                              cout << "secretKey should not contain %  special Characters" << endl;
		                              cout << endl;
		                              flagout = 1;
		                               break;
		                         }
		                         else if (secretKey[i] == '#')
		                         {
		                              cout << "secretKey should not contain  #  special Characters" << endl;
		                              cout << endl;
		                              flagout = 1;
		                               break;
		                         }

		                         else if (secretKey[i] == '&')
		                         {
		                              cout << "secretKey should not contain  &  special Characters" << endl;
		                              cout << endl;
		                              flagout = 1;
		                               break;
		                         }


		                         else if (secretKey[i] == '*')
		                         {
		                              cout << "secretKey should not contain * special Characters" << endl;
		                              cout << endl;
		                              flagout = 1;
		                               break;
		                         }
					 else if (secretKey[i] == '/')

		                         {
		                              cout << "secretKey should not contain / special Characters" << endl;
		                              cout << endl;
		                              flagout = 1;
		                               break;
		                         }
				}
					
				if(flagout == 1)
				{
					if(choice == "7")
					{
							
						boost::system::error_code ec;
						std::string current_path= get_current_dir_name();
						std::string full_path = current_path + "/multipartupload/" +m_bucketname + "/" + m_objname;
						boost::filesystem::remove_all(full_path.c_str(), ec);
						if (ec)
						{
								std::cout << "Folder Not Deleted" << std::endl;
								
						}
					}
					continue;
				}

			
			

				auto auth = objectRequest->add_auth();

				auth->set_accesskey(accessKey);
				auth->set_secretkey(secretKey);
				if(choice !="7" && choice !="8" && choice !="9")
				{

					std::string bucketName, objectPath;
					
					while(true)
					{
						std::cout << "Enter bucket name: ";
						char buck_name;
						int cou=0;
						buck_name=getc(stdin);
						if(buck_name!='\n')
							bucketName+=buck_name;
							
						do{
							buck_name=getc(stdin);
							if(buck_name!='\n')
							{
								bucketName+=buck_name;
								cou=1;
							}
							if(buck_name == '\n' && cou==0)
								cout << "You have entered new line, Please Enter the bucket name" << endl;

						}while(buck_name!='\n');
						if(cou)
							break;
					
					}
					objectRequest->set_bucketname(bucketName);
				}
			
				if(choice!= "1" && choice !="4" && choice != "6" && choice != "7"  && choice != "9"  && choice != "8")
				{
					std::string objectPath;								
					while(true)
					{
						std::cout << "Enter ObjectName: ";
						char Obj_name;
						int cou=0;
						Obj_name=getc(stdin);
						if(Obj_name!='\n')
							objectPath+=Obj_name;
							
						do{
							Obj_name=getc(stdin);
							if(Obj_name!='\n')
							{
								objectPath+=Obj_name;
								cou=1;
							}
							if(Obj_name == '\n' && cou==0)
								cout << "You have entered new line, Please Enter the valid Object name" << endl;

						}while(Obj_name!='\n');
						if(cou)
							break;
					
					}
					std::string objectName = getObjectName(objectPath);
					objectRequest->set_objectname(objectName);
						
				}	
					
		}
		else 
		{
			std::cout << "Invalid option Please enter from 1 to 5" << std::endl;
			continue;
		}
				auto requestId = getRequestId();

				std::cout << "Request Id: " << requestId << std::endl;
				request.set_requestid(requestId);

				auto serializedProto = request.SerializeAsString();
				
			    	fd_set readSet;
			        FD_ZERO(&readSet);
			        FD_SET(sockFd, &readSet);

			    // Set timeout to 0 for non-blocking check
			       timeval timeout;
			       timeout.tv_sec = 0;
			       timeout.tv_usec = 0;
			       int result = select(sockFd+1, &readSet, nullptr, nullptr, &timeout);
			    //cout << "Result: " << result << endl;
			       if (result == -1) {
				 //Error occurred while checking socket
				  std::cerr << "Error in select(): " << strerror(errno) << std::endl;
			       } else if (result == 0) {
				// Socket is connected
				  pub->publishMessage(serializedProto, "test");
				  
				  if(putobj == 1)
				  {
					std::string name =  objectPath1;
					//std::cout << name << std::endl;
					std::thread th3(readFromFile,std::ref(cs),name);
			
					if (th3.joinable())
					{
						th3.join();
					}
			
				   }	
				
			      } else {
			   	// Socket is connected
				  std::cout << "Socket is not connected" << std::endl;
				  breakclient = "no";
				  close(sockFd);
				//cout << breakclient << endl;
				 return;
			     }
			     while(1)
			    {
				if(getobj == 1 || sleeptime == 1)
				{
					sleep(4);
					break;
				}
			    }
		
			    // sleep(7);
		
	}
	//break;
}

void displayConsumedMessage(clientSocket& csRead, s3service::serviceRequestResponse response)
{
	auto entityType = response.entitytype();

	std::cout << "Request Id: " << response.requestid() << std::endl;
	//std::cout << entityType << std::endl;
	if (entityType == s3service::serviceRequestResponse::ACCOUNT)
	{
		for (int i = 0; i < response.account_size(); i++)
		{
			auto accountResponse = response.account(i);

			auto accountOp = accountResponse.accop();

			if (accountOp == s3service::s3account::CREATE_ACCOUNT)
			{
				if (accountResponse.errorinfo_size() == 0)
				{
					std::cout << "Account created successfully" << std::endl;
					std::cout << "AccountId	: " << accountResponse.accountid() << std::endl;
					auto key = accountResponse.keys(0);
					std::cout << "AccessKeyId	: " << key.accesskeyid() << std::endl;
					std::cout << "SecretKey	: " << key.secretkey() << std::endl;
					

					
				}
				else
				{
					s3service::errorDetails error = accountResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error while createAccount(): " << std::endl;
					std::cout << "Error Type                                : " << error.errortype() << std::endl;
					std::cout << "Error Code                                : " << error.errorcode() << std::endl;
					std::cout << "Error Message                             : " << error.errormessage() << std::endl;
				}
			}
			else
			{
				if (accountResponse.errorinfo_size() == 0)
				{
					std::cout << "Account deleted successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = accountResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error while deleteAccount(): " << std::endl;
					std::cout << "Error Type                                : " << error.errortype() << std::endl;
					std::cout << "Error Code                                : " << error.errorcode() << std::endl;
					std::cout << "Error Message                             : " << error.errormessage() << std::endl;
				}
			}
		}
	}
	else if (entityType == s3service::serviceRequestResponse::SERVER)
	{
		for (int i = 0; i < response.server_size(); i++)
		{
			auto serverResponse = response.server(i);
			auto serverOp = serverResponse.serverop();
			if(serverOp = s3service::S3server::Serverdown)
			{
				if(serverResponse.errorinfo_size() != 0)
				{
					s3service::errorDetails error = serverResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error while createUser(): " << std::endl;
					std::cout << "Error Type                             : " << error.errortype() << std::endl;
					std::cout << "Error Code                             : " << error.errorcode() << std::endl;
					std::cout << "Error Message                          : " << error.errormessage() << std::endl;
					break;
				}
			}
			
		} 
	}
	else if (entityType == s3service::serviceRequestResponse::USER)
	{
		for (int i = 0; i < response.user_size(); i++)
		{
			auto userResponse = response.user(i);

			auto userOp = userResponse.userop();

			if (userOp == s3service::s3user::CREATE_USER)
			{
				if (userResponse.errorinfo_size() == 0)
				{
					//userResponse.set_arn("1234");
					std::cout << "User created successfully" << std::endl;
					std::cout << "User Arn   : " << userResponse.arn() << std::endl;
					std::cout << "User Id    : " << userResponse.userid() << std::endl;
					std::cout << "Test Arn   : " << userResponse.test() << std::endl;
					std::cout << "Create Date: " << userResponse.createdate() << std::endl;
				}
				else
				{
					s3service::errorDetails error = userResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error while createUser(): " << std::endl;
					std::cout << "Error Type                             : " << error.errortype() << std::endl;
					std::cout << "Error Code                             : " << error.errorcode() << std::endl;
					std::cout << "Error Message                          : " << error.errormessage() << std::endl;
				}
			}
			else if (userOp == s3service::s3user::DELETE_USER)
			{
				if (userResponse.errorinfo_size() == 0)
				{
					std::cout << "User deleted successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = userResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error deleteUser(): "  << std::endl;
					std::cout << "Error Type                       : " << error.errortype() << std::endl;
					std::cout << "Error Code                       : " << error.errorcode() << std::endl;
					std::cout << "Error Message                    : " << error.errormessage() << std::endl;
				}
			}
			else
			{
				if (userResponse.errorinfo_size() == 0)
				{
					std::cout << "User updated successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = userResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error updateUser(): " << std::endl;
					std::cout << "Error Type                       : " << error.errortype() << std::endl;
					std::cout << "Error Code                       : " << error.errorcode() << std::endl;
					std::cout << "Error Message                    : " << error.errormessage() << std::endl;
				}
			}
		}
	}
	else if (entityType == s3service::serviceRequestResponse::ACCESSKEY)
	{
		for (int i = 0; i < response.accesskey_size(); i++)
		{
			auto keyResponse = response.accesskey(i);

			auto keyOp = keyResponse.accessop();

			if (keyOp == s3service::s3accesskey::CREATE_KEY)
			{
				if (keyResponse.errorinfo_size() == 0)
				{
					std::cout << "Key created successfully" << std::endl;
					std::cout << "AccessKey        : " << keyResponse.accesskeyid() << std::endl;
					std::cout << "SecretKey        : " << keyResponse.secretkey() << std::endl;
					std::cout << "AccessKeySelector: " << keyResponse.accesskeyselector() << std::endl;
					std::cout << "CreationDate     : " << keyResponse.createdate() << std::endl;
					std::cout << "Status           : " << keyResponse.status() << std::endl;
				}
				else
				{
					s3service::errorDetails error = keyResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error while createKey(): "  << std::endl;
					std::cout << "Error Type                            : " << error.errortype() << std::endl;
					std::cout << "Error Code                            : " << error.errorcode() << std::endl;
					std::cout << "Error Message                         : " << error.errormessage() << std::endl;
				}
			}
			else if (keyOp == s3service::s3accesskey::DELETE_KEY)
			{
				if (keyResponse.errorinfo_size() == 0)
				{
					std::cout << "Key deleted successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = keyResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error deleteKey(): " << std::endl;
					std::cout << "Error Type                      : " << error.errortype() << std::endl;
					std::cout << "Error Code                      : " << error.errorcode() << std::endl;
					std::cout << "Error Message                   : " << error.errormessage() << std::endl;
				}
			}
			else if (keyOp == s3service::s3accesskey::CHANGE_KEY_STATUS)
			{
				if (keyResponse.errorinfo_size() == 0)
				{
					std::cout << "Key status updated successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = keyResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error changeKeyStatus(): " << std::endl;
					std::cout << "Error Type                            : " << error.errortype() << std::endl;
					std::cout << "Error Code                            : " << error.errorcode() << std::endl;
					std::cout << "Error Message                         : " << error.errormessage() << std::endl;
				}
			}
			else
			{
				if (keyResponse.errorinfo_size() == 0)
				{
					std::cout << "Last Used Time: " << keyResponse.lastuseddate() << std::endl;
					std::cout << "ServiceName: " << keyResponse.servicename() << std::endl;
				}
				else
				{
					s3service::errorDetails error = keyResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error getLastUsedTime(): " << std::endl;
					std::cout << "Error Type                            : " << error.errortype() << std::endl;
					std::cout << "Error Code                            : " << error.errorcode() << std::endl;
					std::cout << "Error Message                         : " << error.errormessage() << std::endl;
				}
			}
		}
	}
	else if (entityType == s3service::serviceRequestResponse::BUCKET)
	{
		for (int i = 0; i < response.bucket_size(); i++)
		{
			auto bucketResponse = response.bucket(i);

			auto bucketOp = bucketResponse.buckop();

			if (bucketOp == s3service::s3bucket::CREATE_BUCKET)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
					std::cout << "Bucket created successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error while createBucket(): "  << std::endl;
					std::cout << "Error Type                               : " << error.errortype() << std::endl;
					std::cout << "Error Code                               : " << error.errorcode() << std::endl;
					std::cout << "Error Message                            : " << error.errormessage() << std::endl;
				}
			}
			else if(bucketOp == s3service::s3bucket::SET_STORAGE_QUOTA)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
					std::cout << "Storage space is set for the bucket" << std::endl;
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error Storagequota(): " << std::endl;
					std::cout << "Error Type                         : " << error.errortype() << std::endl;
					std::cout << "Error Code                         : " << error.errorcode() << std::endl;
					std::cout << "Error Message                      : " << error.errormessage() << std::endl;
				}
			}
			else if(bucketOp == s3service::s3bucket::GET_STORAGE_LENS)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
				std::string type;
				
				std::string total_memory = bucketResponse.total_size();
				std::string size_consumed = bucketResponse.size_consumed();
				std::string free_space = bucketResponse.free_space();
				
				
				stringstream m;
				m << total_memory;
				double memory = 0;
				m >> memory;
				
				
				stringstream n;
				n << size_consumed;
				double SizeConsumed = 0;
				n >> SizeConsumed;
				
				
				
				stringstream o;
				o << free_space;
				double FreeSpace = 0;
				o >> FreeSpace;
				
				double consumed_Per = (SizeConsumed/memory)*100;
				double free_Per = (FreeSpace/memory)*100;
					
				if (bucketResponse.errorinfo_size() == 0)
				{
					
					if( memory >= 1000 && memory < 1000000)
					{
						memory = memory / 1000;
						type = "kb";
					}
					else if(memory >= 1000000 && memory < 1000000000)
					{
						memory = memory / 1000000;
						type = "Mb";
					}
					else if(memory >= 1000000000)
					{
						memory = memory / 1000000000;
						type = "Gb";
					}
				}
				
				std::cout << "Totak Size of Bucket            : " << memory <<" "<< type <<std::endl;
					      
				
				if (bucketResponse.errorinfo_size() == 0)
				{
					if( SizeConsumed >= 1000 && SizeConsumed < 1000000)
					{
						SizeConsumed = SizeConsumed / 1000;
						type = "kb";
					}
					else if(SizeConsumed >= 1000000 && SizeConsumed < 1000000000)
					{
						SizeConsumed = SizeConsumed / 1000000;
						type = "Mb";
					}
					else if(SizeConsumed >= 1000000000)
					{
						SizeConsumed = SizeConsumed / 1000000000;
						type = "Gb";
					}
				}
				
				std::cout << "Storage Space Consumed Now      : " << SizeConsumed <<" "<< type <<std::endl;
					      
				
				if (bucketResponse.errorinfo_size() == 0)
				{
					
					if( FreeSpace >= 1000 && FreeSpace < 1000000)
					{
						FreeSpace = FreeSpace / 1000;
						type = "kb";
					}
					else if(FreeSpace >= 1000000 && FreeSpace < 1000000000)
					{
						FreeSpace = FreeSpace / 1000000;
						type = "Mb";
					}
					else if(FreeSpace >= 1000000000)
					{
						FreeSpace = FreeSpace / 1000000000;
						type = "Gb";
					}
				}
				
				std::cout << "Free Storage Space left         : " << FreeSpace <<" "<< type <<std::endl;
					   
				if(free_Per > 0)
					{	   
						std::cout << "Free space left in              : " << free_Per  <<" %"<<std::endl;
					}
					else
					{
						free_Per = 0;
						std::cout << "Free space left in              : " << free_Per  <<" %"<<std::endl;
					}
					
					if(consumed_Per > 0)
					{	   
						std::cout << "Bucket Space Consumed Now       : " << consumed_Per<<" %"<<std::endl;
					}
					else
					{
						consumed_Per = 0;
						std::cout << "Bucket Space Consumed Now       : " << consumed_Per<<" %"<<std::endl;
					}
					
					std::cout << endl;
					
					float count = 0;
					std::string str = bucketResponse.storagelens();
					std::string str1 = bucketResponse.storagelens();
					char* string1 = &str1[0];
					std::string word = "";
					int index=0;
	   				for (auto x : str)
	    				{
						if (x == ' ')
						{
		    						std::cout << "Number of objects with extension " <<  word << std::endl;
		   						word = "";
		   						
						}
						else
						{
		    					word = word + x;
						}
	    				}
	    					
	    				for(int i=0; i<str1.length();i++)
	    				{
	    					if(isdigit(str1[i]))
	   					{
	   						char a = str1[i];
	   						float converttonumber = stoi(&a);
	   						count += converttonumber;
	   					}
	    				}
	    				cout << endl;
	   				//std::cout << "list of objects with extension " << word << std::endl;
	   				for(int i=0; i < str1.length();i++)
	   				{
	   					if(!isdigit(str[i]) && str[i] != ' ' && str[i] !=':')
	   					{
	   						cout << str1[i];
	   					}
	   					if(isdigit(str1[i]))
	   					{
	   						cout << " files percentage is ";
	   				
	   						char a = str1[i];
	   						float converttonumber = stoi(&a);
	   						
	   						float peroffiles = (converttonumber/count)*100;
	   						printf("%.0f%%", peroffiles);
	   							
	   						//cout << "%2f" << peroffiles << endl;
	   						cout << endl;
	   					}
	   				}
	   				
	   				std::cout << endl;
	   				
	   				std::cout << "Total Number of Object : " << count<<std::endl;	
	   					
				}
				
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error Storage Lens(): " << std::endl;
					std::cout << "Error Type                         : " << error.errortype() << std::endl;
					std::cout << "Error Code                         : " << error.errorcode() << std::endl;
					std::cout << "Error Message                      : " << error.errormessage() << std::endl;
				}
			}
			else if (bucketOp == s3service::s3bucket::SET_LIFECYCLE_RULE)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
					std::cout << "Lifecyle rule is successfully set for the bucket" << std::endl;
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error set_lifecycle_rule(): " << std::endl;
					std::cout << "Error Type				 : " << error.errortype() << std::endl;
					std::cout << "Error Code				 : " << error.errorcode() << std::endl;
					std::cout << "Error Message				 : " << error.errormessage() << std::endl;
				}
			}
			
			else if (bucketOp == s3service::s3bucket::DELETE_BUCKET)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
					std::cout << "Bucket deleted successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error deleteBucket(): " << std::endl;
					std::cout << "Error Type			   : " << error.errortype() << std::endl;
					std::cout << "Error Code			   : " << error.errorcode() << std::endl;
					std::cout << "Error Message			   : " << error.errormessage() << std::endl;
				}
			}
			else if (bucketOp == s3service::s3bucket::PUT_BUCKET_TAG)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
					std::cout << "Bucket Tag updated successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error putBucketTag(): " << std::endl;
					std::cout << "Error Type                         : " << error.errortype() << std::endl;
					std::cout << "Error Code                         : " << error.errorcode() << std::endl;
					std::cout << "Error Message                      : " << error.errormessage() << std::endl;
				}
			}
			else if(bucketOp == s3service::s3bucket::PUT_BUCKET_VERSION)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
				 	std::cout << "Version is " <<bucketResponse.version() << std::endl;
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error putBucketVersion(): " << std::endl;
					std::cout << "Error Type                             : " << error.errortype() << std::endl;

					std::cout << "Error Code                             : " << error.errorcode() << std::endl;
					std::cout << "Error Message                          : " << error.errormessage() << std::endl;
				}
			}
			
		        else if(bucketOp == s3service::s3bucket::GET_BUCKET_VERSION)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
				 	std::cout << "Version is " << bucketResponse.version() << std::endl;
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error Getbucketversion(): " << std::endl;
					std::cout << "Error Type                             : " << error.errortype() << std::endl;

					std::cout << "Error Code                             : " << error.errorcode() << std::endl;

					std::cout << "Error Message                          : " << error.errormessage() << std::endl;
				}
			}
			else if (bucketOp == s3service::s3bucket::LIST_OBJECT_VERSIONS)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
					std::cout << "List of Object: " << std::endl;
					std::cout << std::endl;
					std::string str=bucketResponse.list_object_versions();
					std::string word = "";
					std::vector<std::string> updateprefix;
					updateprefix.push_back("Object Name   : ");
					updateprefix.push_back("Object Size   : ");
					updateprefix.push_back("Version Status: ");
					updateprefix.push_back("Version ID    : ");
					int index=0;
					int totalobject=0;
   					for (auto x : str)
    					{
        					if (x == ' ')
        					{
            						std::cout << updateprefix[index++] <<  word << std::endl;
           						word = "";
           						if(index%4==0)
           						{
           							totalobject++;
           							index=0;
           							std::cout << std::endl;
           							std::cout << std::endl;
           						}
        					}
        					else {
            						word = word + x;
        					}
    					}
   					std::cout << word << std::endl;
   					std::cout << "\n" << std::endl;
   					std::cout << "Total Object: " << totalobject << std::endl;
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error listObjectversion(): " << std::endl;
					std::cout << "Error Type                              : " << error.errortype() << std::endl;

					std::cout << "Error Code                              : " << error.errorcode() << std::endl;
					std::cout << "Error Message                           : " << error.errormessage() << std::endl;
				}
			}
			

			else if (bucketOp == s3service::s3bucket::LIST_OBJECT)
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
					std::cout << "List of Object: " << std::endl;
					std::cout << std::endl;
					std::string str=bucketResponse.list_object();
					std::string word = "";
					int index=0;
   					for (auto x : str)
    					{
        					if (x == ' ')
        					{
            						std::cout <<  word << std::endl;
           						word = "";
        					}
        					else {
            						word = word + x;
        					}
    					}
   					std::cout << word << std::endl;
   					std::cout << std::endl;
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error listObject(): " << std::endl;
					std::cout << "Error Type                       : " << error.errortype() << std::endl;

					std::cout << "Error Code                       : " << error.errorcode() << std::endl;
					std::cout << "Error Message                    : " << error.errormessage() << std::endl;
				}
			}
			else
			{
				if (bucketResponse.errorinfo_size() == 0)
				{
					std::cout << "Following are all the tags: " << std::endl;

					for (int i = 0; i < bucketResponse.tag_size(); i++)
					{
						std::cout << "Tag" << i+1 << ":" << std::endl;
						auto tag = bucketResponse.tag(i);

						std::cout << tag.key() << "->" << tag.value() << std::endl;
					}
				}
				else
				{
					s3service::errorDetails error = bucketResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error getLastUsedTime(): " << std::endl;
					std::cout << "Error Type                            : " << error.errortype() << std::endl;
					std::cout << "Error Code                            : " << error.errorcode() << std::endl;
					std::cout << "Error Message                         : " << error.errormessage() << std::endl;
				}
			}

		}
	}
	else if (entityType == s3service::serviceRequestResponse::OBJECT)
	{
		for (int i = 0; i < response.object_size(); i++)
		{
			auto objectResponse = response.object(i);

			auto objectOp = objectResponse.objectop();

			if (objectOp == s3service::s3object::PUT_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "Object created successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);
					th3stop = 1;
					auto errorCode = error.errorcode();

					std::cout << "Encountered an error while createObject(): "  << std::endl;
					std::cout << "Error Type                               : " << error.errortype() << std::endl;
					std::cout << "Error Code                               : " << error.errorcode() << std::endl;
					std::cout << "Error Message                            : " << error.errormessage() << std::endl;
				}
			}
			else if (objectOp == s3service::s3object::INIT_MULTIPART_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "Object path set successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error InitMultiPartUpload(): " << std::endl;
					std::cout << "Error Type                                : " << error.errortype() << std::endl;
					std::cout << "Error Code                                : " << error.errorcode() << std::endl;
					std::cout << "Error Message                             : " << error.errormessage() << std::endl;
					
					
        		std::string current_path= get_current_dir_name();
			std::string full_path = current_path + "/multipartupload/" + bucketName2;
			boost::system::error_code ec;
			boost::filesystem::remove_all(full_path.c_str(), ec);
			if (ec)
			{
				std::cout << "Folder Not Deleted" << std::endl;
					
			}
				}
			}
			
			else if(objectOp == s3service::s3object::LIST_MULTIPART_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "****List of multipart Object****" << std::endl;
					std::cout << std::endl;
					std::string str = objectResponse.filename();
					std::string word = "";
					int index=0;
   					for (auto x : str)
    					{
        					if (x == ' ')
        					{
            						std::cout <<  word << std::endl;
           						word = "";
        					}
        					else {
            						word = word + x;
        					}
    					}
   					std::cout << word << std::endl;
   					std::cout << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error listMultiPartUpload(): " << std::endl;
					std::cout << "Error Type                                : " << error.errortype() << std::endl;

					std::cout << "Error Code                                : " << error.errorcode() << std::endl;
					std::cout << "Error Message                             : " << error.errormessage() << std::endl;
				
				}
			}
			
			else if(objectOp == s3service::s3object::ABORT_MULTIPART_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "Object abort successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error abortMultiPartUpload(): " << std::endl;
					std::cout << "Error Type                                 : " << error.errortype() << std::endl;
					std::cout << "Error Code                                 : " << error.errorcode() << std::endl;
					std::cout << "Error Message                              : " << error.errormessage() << std::endl;
				}
			}
			else if(objectOp == s3service::s3object::PUT_MULTIPART)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "Object put successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error PutMultiPartUpload(): " << std::endl;
					std::cout << "Error Type                               : " << error.errortype() << std::endl;
					std::cout << "Error Code                               : " << error.errorcode() << std::endl;
					std::cout << "Error Message                            : " << error.errormessage() << std::endl;
				}
			}
			else if (objectOp == s3service::s3object::COMPLETE_MULTIPART_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "Object uploaded successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error completeMultiPartUpload(): " << std::endl;
					std::cout << "Error Type                                    : " << error.errortype() << std::endl;
					std::cout << "Error Code                                    : " << error.errorcode() << std::endl;
					std::cout << "Error Message                                 : " << error.errormessage() << std::endl;
				}
			}
			
			else if (objectOp == s3service::s3object::DELETE_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "Object deleted successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error deleteObject(): " << std::endl;
					std::cout << "Error Type                         : " << error.errortype() << std::endl;
					std::cout << "Error Code                         : " << error.errorcode() << std::endl;
					std::cout << "Error Message                      : " << error.errormessage() << std::endl;
				}
			}
			
			else if(objectOp == s3service::s3object::LIST_DELETE_MARKER_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					
					std::cout << objectResponse.listdeletemarker() << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error in Deletemarker ListdeletemarkerObject(): " << std::endl;
					std::cout << "Error Type                                                   : " << error.errortype() << std::endl;
					std::cout << "Error Code                                                   : " << error.errorcode() << std::endl;
					std::cout << "Error Message                                                : " << error.errormessage() << std::endl;
				}
			}
			else if (objectOp == s3service::s3object::GET_DELETE_MARKER_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "Object reterived successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error getDeleteMarkerobject(): " << std::endl;
					std::cout << "Error Type                                  : " << error.errortype() << std::endl;
					std::cout << "Error Code                                  : " << error.errorcode() << std::endl;
					std::cout << "Error Message                               : " << error.errormessage() << std::endl;
				}
			}
			else if (objectOp == s3service::s3object::GET_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					
					writeToFile(csRead, objectResponse.objectname(), objectResponse.len());
					std::cout << "Object Retrived successfully" << std::endl;
					getobj=1;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error getobject(): " << std::endl;
					std::cout << "Error Type                      : " << error.errortype() << std::endl;
					std::cout << "Error Code                      : " << error.errorcode() << std::endl;
					std::cout << "Error Message                   : " << error.errormessage() << std::endl;
				}
			}
			
			else if(objectOp == s3service::s3object::PUT_MULTIPART_OBJECT)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "Object Uploaded successfully" << std::endl;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error putmultipartobject(): " << std::endl;
					std::cout << "Error Type                               : " << error.errortype() << std::endl;
					std::cout << "Error Code                               : " << error.errorcode() << std::endl;
					std::cout << "Error Message                            : " << error.errormessage() << std::endl;
				}
			}
			else if(objectOp == s3service::s3object::GET_OBJECT_BY_VERSION)
			{
				if (objectResponse.errorinfo_size() == 0)
				{
					std::cout << "Object Retrived successfully" << std::endl;
					writeToFile(csRead, objectResponse.objectname(), objectResponse.len());
					getobj=1;
				}
				else
				{
					s3service::errorDetails error = objectResponse.errorinfo(0);

					auto errorCode = error.errorcode();

					std::cout << "Encountered an error getObjectbyversion(): " << std::endl;
					std::cout << "Error Type                               : " << error.errortype() << std::endl;
					std::cout << "Error Code                               : " << error.errorcode() << std::endl;
					std::cout << "Error Message                            : " << error.errormessage() << std::endl;
				}
			}



		       else if (objectOp == s3service::s3object::PUT_OBJECT_LOCKING)
            	      {
               		       if (objectResponse.errorinfo_size() == 0)
                	       {

				        
                   			 std::cout << "Object lock settings updated successfully" << std::endl;
                   			 bool lockMode = objectResponse.locking();
                   			 std::cout << "Object Lock Mode: " << lockMode << std::endl;
	
        		        }
               			 else
                		{
                   			 s3service::errorDetails error = objectResponse.errorinfo(0);

                   			 auto errorCode = error.errorcode();

                   			 std::cout << "Encountered an error updating object lock settings: " << std::endl;
                   			 std::cout << "Error Type: " << error.errortype() << std::endl;
               			         std::cout << "Error Code: " << error.errorcode() << std::endl;
                                         std::cout << "Error Message: " << error.errormessage() << std::endl;
                                 }
                       }

		      else if (objectOp == s3service::s3object::SET_OBJECT_RETENTION)
		      {
			    if (objectResponse.errorinfo_size() == 0) 
			    {
       				 std::cout << "Object retention set successfully" << std::endl;
       				 string retentionDuration = objectResponse.retention_duration();
       				 std::cout << "Retention Duration: " << retentionDuration << std::endl;
    				}
			    else
			    {
     				   s3service::errorDetails error = objectResponse.errorinfo(0);

       				    auto errorCode = error.errorcode();

        			 std::cout << "Encountered an error setting object retention: " << std::endl;
       				 std::cout << "Error Type: " << error.errortype() << std::endl;
       			         std::cout << "Error Code: " << error.errorcode() << std::endl;
       				 std::cout << "Error Message: " << error.errormessage() << std::endl;
   			 }

		      }



		}
	}
}

int main()
{
	publisher pub("", "", "", "", "requestExchange");

	auto headers = pub.getHeaders();

	headers.insert({"Content-type", "text/text"});
	headers.insert({"Content-encoding", "UTF-8"});

	pub.addHeaders(headers);

	auto err = pub.init();

	if(err.size() != 0)
	{
		std::cout << "Error: " << err << std::endl;
	    return false;
	}

	err = pub.createAndBindQueue("requestQueue", "test");

	if(err.size() != 0)
	{
		std::cout << "Error: " << err << std::endl;
	    return false;
	}

	consumer cObj("", "", "", "", "responseQueue", "client");

	err = cObj.init();

	if (err.size() != 0)
	{
		std::cout << "Error consumer::init(): " << err << std::endl;
		return false;
	}

	err = cObj.bindToExchange("responseExchange", "test");

	if (err.size() != 0)
	{
		std::cout << "Error consumer::bindToExchange(): " << err << std::endl;
		return false;
	}
//#ifndef AUTOMATION_FEATURE_ENABLED
	clientSocket csRead("127.0.0.1", 9090);

	 auto ret = csRead.createAndConnect();

	if (ret != "")
	{
		std::cout << "cdRead.createAndConect() failed " << ret << std::endl;
		return -1;
	}
	
	std::thread th1(&createAndPublishRequestPacket, &pub);

	std::thread th2(&consumer::consumeMessage, &cObj);

	while (true)
	{
		for (size_t i = 0; i < consumer::m_consumeRequestMsg.size(); i++)
		{
			s3service::serviceRequestResponse response;

			response.ParseFromString(consumer::m_consumeRequestMsg[i]);

			std::cout << std::endl;
			std::cout << "*****************************************" << std::endl;
			std::cout << std::endl;

			displayConsumedMessage(csRead, response);

			std::cout << std::endl;
			std::cout << "*****************************************" << std::endl;
			std::cout << std::endl;

			consumer::m_consumeRequestMsg.erase(consumer::m_consumeRequestMsg.begin() + i);
			i--;
		}
	}

	th1.join();
	th2.join();
/*#else
	std::thread th2(&consumer::consumeMessage, &cObj);
	
	clientSocket csRead("127.0.0.1", 9090);

	auto ret = csRead.createAndConnect();

	if (ret != "")
	{
		std::cout << "cdRead.createAndConect() failed " << ret << std::endl;
		return -1;
	}
	
	clientSocket cs("127.0.0.1", 8081);
	
	cs.createAndConnect();
	
	Pistache::Address addr(Pistache::Ipv4::any(), Pistache::Port(8083));
	
	Pistache::Http::Endpoint server(addr);
	
	server.options().threads(1);
	
	Rest::Router router;
	
	Rest::Routes::Get(router, "/client", [&](const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) {
        	handleOperation(&pub, request, std::move(response));
        	return Pistache::Rest::Route::Result::Ok;
        });
    	server.setHandler(router.handler());
	std::cout<< "starting server on port 8083, http://localhost:8083/client?operation=<Account>&suboperation=<CreateAccount>&accountname=<accountname>&password=<password>"  <<std::endl;
    	server.serve();
    
    	server.shutdown();
    	
	
#endif*/
    return 0;
}
