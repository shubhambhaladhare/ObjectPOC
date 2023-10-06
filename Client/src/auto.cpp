#include "auto.hpp"
#include <iostream>
#include "publisher.hpp"
#include "consumer.hpp"
#include "s3service.pb.h"
#include "clientsocket.hpp"
#include <thread>
#include <sstream>

using namespace Pistache;
using namespace Pistache::Rest;
using namespace std;

void sendoperation(publisher* pub, std::string operation, std::string suboperation, std::string accountName, std::string password)
{
    s3service::serviceRequestResponse clirequest;
	if (operation == "Account")
	{
		clirequest.set_entitytype(s3service::serviceRequestResponse::ACCOUNT);
		
		auto accountRequest  = clirequest.add_account();
		
		if (suboperation=="CreateAccount")
		{
			accountRequest->set_accop(s3service::s3account::CREATE_ACCOUNT);	
		}
		else if(suboperation == "DELETE_ACCOUNT")
		{
			accountRequest->set_accop(s3service::s3account::DELETE_ACCOUNT);			
		}
		accountRequest->set_accountname(accountName);
			
		accountRequest->set_password(password);
	}
		
		auto serializedProto = clirequest.SerializeAsString();
		
		pub->publishMessage(serializedProto, "test");
		
		sleep(3);
}

Pistache::Rest::Route::Result handleOperation(publisher* pub, const Rest::Request& request, Http::ResponseWriter response)
{
	    const auto& queryParams = request.query();
	    std::string operation = queryParams.get("operation").value_or("");
	    std::string suboperation = queryParams.get("suboperation").value_or("");
	    std::string accountName = queryParams.get("accountname").value_or("");
	    std::string password = queryParams.get("password").value_or("");

	    std::cout << operation << std::endl;
	    std::cout << suboperation << std::endl;
	    std::cout << accountName << std::endl;
	    std::cout << password << std::endl;

	    sendoperation(pub, operation, suboperation, accountName, password);

	    while (true)
	    {
		if(!consumer::m_consumeRequestMsg.empty())
		{
			s3service::serviceRequestResponse serverresponse;

			serverresponse.ParseFromString(consumer::m_consumeRequestMsg[0]);
			
			auto entityType = serverresponse.entitytype();
			std::cout<<6<<std::endl;
			if (entityType == s3service::serviceRequestResponse::ACCOUNT)
			{
				for (int i = 0; i < serverresponse.account_size(); i++)
				{
					auto accountResponse = serverresponse.account(i);

					auto accountOp = accountResponse.accop();

					if (accountOp == s3service::s3account::CREATE_ACCOUNT)
					{
						if (accountResponse.errorinfo_size() == 0)
						{	
							
							auto key = accountResponse.keys(0);
							std::string acc = accountResponse.accountid();
							std::string acckey = key.accesskeyid();
							std::string seckey = key.secretkey();
							
							std::string result = "Account created successfully \nAccountId: " +  acc + "\nAccessKeyId: " + acckey +"\nSecretKey:" + seckey;
							std::cout<<result;
							response.send(Http::Code::Ok,result);
							

						}
					
						else
						{
							s3service::errorDetails error = accountResponse.errorinfo(0);

							auto errorCode = error.errorcode();
							std::string errortype = error.errortype();
							std::string errorcode = error.errorcode();
							std::string errmsg = error.errormessage();
							
							std::string result = "Encountered an error while createAccount():\nError Type:" + errortype + "\nError Code: "+ errorcode + "\nError Message: "+ errmsg;
							response.send(Http::Code::Ok,result);
							
							
						}
					}
					else
					{
						if (accountResponse.errorinfo_size() == 0)
						{
							response.send(Http::Code::Ok,"Account deleted successfully");
						}
						else
						{
							s3service::errorDetails error = accountResponse.errorinfo(0);

							auto errorCode = error.errorcode();
							std::string errortype = error.errortype();
							std::string errorcode = error.errorcode();
							std::string errmsg = error.errormessage();
							
							std::string result = "Encountered an error while deleteAccount():\nError Type:" + errortype + "\nError Code: "+ errorcode + "\nError Message: "+ errmsg;
							response.send(Http::Code::Ok,result);
							
						}
					}
					
				}
			}
			consumer::m_consumeRequestMsg.erase(consumer::m_consumeRequestMsg.begin());
			break;
				
		}
	    }
}






