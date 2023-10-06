#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
class clientSocket
{
    int sockFd , portNum;
    sockaddr_in addr_in;
    std::string addr;

    public:
    int get_sockfd()
    {
    	return sockFd;
    }
    clientSocket(std::string addr, int portNum = 8080)
    {
    	sockFd = -1;
        this->addr = addr;
        this->portNum = portNum;
        bzero((char *)&addr_in, sizeof(addr_in));
    }

    void setAddress(std::string addr)
    {
    	this->addr = addr;
    }

    std::string createAndConnect()
    {
        if (addr == "")
        {
            return "Set a valid IP address";
        }

        sockFd = socket(AF_INET, SOCK_STREAM, 0);
	
        if (sockFd < 0)
        {
            return "Error while creating socket";
        }

        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(portNum);

        int ret = inet_pton(AF_INET, addr.c_str(), &addr_in.sin_addr);

        if (ret <= 0)
        {
            return "Invalid address. Address not supported";
        }

        ret = connect(sockFd, (const sockaddr *)&addr_in, sizeof(addr_in));
        if (ret < 0)
        {
            return "Connection failed" + std::to_string(errno);
        }

        return std::string();
    }

    auto writeSocket(char data[], int size)
    {
    /*long long int totalSent = 0;
    int chunkSize = 10;
    int t=0;
    int byteleft = size;
    //std::cout << data << std::endl;
    //std::cout << sizeof(data) << std::endl;
    while(totalSent < size)
    {
    int rem = size-totalSent;
    //int cur_chunkSize = (rem<chunkSize)?rem:chunkSize;
    int ret = send(sockFd, data+totalSent, chunkSize, 0);
    if(ret<0)
    {
   	 return "Send failed " + std::to_string(errno);
    }
    totalSent += ret;
    byteleft -= ret;
    std::cout << totalSent << ", " << size << std::endl;
    }
    return std::string();*/
        int ret = send(sockFd, data, size, 0);

        if (ret != size)
        {
        	return "Send failed " + std::to_string(errno);
        }

        return std::string();
    }

    std::string readSocket(std::vector<char>& data, int size)
	{	
	    	int readSize = 0;
        	int valRead = 1;
        	int BUFFER_SIZE = 50000;
        	char buffer[BUFFER_SIZE] = {'\0'};
        	memset(buffer, 0, BUFFER_SIZE);
	
		while (true)
		{
		    
		    valRead = read(sockFd, buffer, BUFFER_SIZE);
		    if (valRead > 0)
		    {

		    		data.insert(data.end(), buffer, buffer + valRead);
		    		readSize += valRead;
		    		memset(buffer, 0, BUFFER_SIZE);
		 
		    	
		    }
		    
		    if (readSize >= size)
		    {
		    	break;
		    }
		    else if (valRead == 0)
		    {
		    	break;
		    }


		    //memset(buffer, 0, BUFFER_SIZE);
		}
		return std::string();

	}
	
	
	

    ~clientSocket()
    {
        if (sockFd != -1)
        {
            //close(sockFd);
        }
    }
};
