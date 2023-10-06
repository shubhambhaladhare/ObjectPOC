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

class serverSocket
{
    int sockFd = -1, clientFd = -1, portNum;
    sockaddr_in addr_in, newAddr_in;
    std::string addr;

    public:

    serverSocket(std::string addr = "", int portNum = 8081)
    {
        this->addr = addr;
        this->portNum = portNum;

    }

    std::string createAndBind(int connectCount)
    {
        sockFd = socket(AF_INET, SOCK_STREAM, 0);
	
        if (sockFd < 0)
        {
            return "Error while creating socket";
        }

        addr_in.sin_family = AF_INET;
        addr_in.sin_port = htons(portNum);

        if (addr == "")
        {
            addr_in.sin_addr.s_addr = INADDR_ANY;
        }
        else
        {
            int ret = inet_pton(AF_INET, addr.c_str(), &addr_in.sin_addr);

            if (ret <= 0)
            {
                return "Invalid address. Adress not supported";
            }
        }

        int ret = bind(sockFd, (const sockaddr*)&addr_in, sizeof(addr_in));

        if (ret < 0)
        {
            return "Server binding failed: " + std::to_string(errno);
        }

        ret = listen(sockFd, connectCount);
        if (ret < 0)
        {
        	return "Listen failed.";
        }

        return std::string();
    }

    std::string acceptConn()
    {
        socklen_t addr_len = 0;

        clientFd = accept(sockFd, (sockaddr*)&newAddr_in, &addr_len);

        if (clientFd < 0)
        {
            return "Accept failed";
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
            
            valRead = read(clientFd, buffer, BUFFER_SIZE);
	    /*if(std::strcmp(buffer,"!!!EOF!!!")==0)
            {
        	break;	
            }*/
            
            if (valRead > 0)
            {
            	data.insert(data.end(), buffer, buffer + valRead);
            	readSize += valRead;
            }
            
            if (readSize >= size)
            {
            	//std::cout << "break: " << size << " " << readSize << std::endl;
            	break;
            }
            else if (valRead == 0)
            {
            	break;
            }
            /*else
            {
		return "Read failed: " + std::to_string(errno);
            }*/


            memset(buffer, 0, BUFFER_SIZE);
        }

        return std::string();
    }

    auto writeSocket(char data[], int size)
	{
		int ret = send(clientFd, data, size, 0);
		if (ret != size)
		{
			return "Send failed " + std::to_string(errno);
		}

		return std::string();
	}

    ~serverSocket()
    {
        if (sockFd != -1)
        {
            close(sockFd);
        }

        if (clientFd != -1)
        {
            close(clientFd);
        }
    }
    int get_clientfd()
    {
    	return clientFd;
    }
};
