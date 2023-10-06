#pragma once

#include "s3service.pb.h"

#include <crypto++/base64.h>
#include <crypto++/hex.h>
#include <crypto++/rsa.h>
#include <crypto++/aes.h>
#include <crypto++/files.h>
#include <crypto++/randpool.h>
#include <crypto++/validate.h>
#include <crypto++/modes.h>
#include <crypto++/osrng.h>
#include <cstring>


#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

/*#include "pch.h"
#include <iostream>
#include "aes.h"

#include "osrng.h"
using CryptoPP::AutoSeededRandomPool;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::exit;
using CryptoPP::Exception;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::CBC_Mode;
using CryptoPP::AES;



#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptlib.h"
using CryptoPP::Exception;

#include "hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;

#include "aes.h"
using CryptoPP::AES;

#include "ccm.h"
using CryptoPP::CBC_Mode;

#include "assert.h"*/


using namespace std;

class EntityBase
{
	void getEntityResponse(s3service::serviceRequestResponse &response);
	void displayEntityResponse(s3service::serviceRequestResponse &response);

	public:

	std::string getUniqueId()
	{
		std::srand(std::time(nullptr));

		return std::to_string(std::rand());
	}

	std::string getCurrentTimeStamp()
	{
		time_t rawtime;
  		struct tm *timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, 80, "%Ft%H:%M:%Sz", timeinfo);

		return buffer;
	}
	
	static std::string Encrypt(const std::string& plainText) 
	{
		using CryptoPP::AutoSeededRandomPool;
		using std::endl;
		using std::string;
		using std::exit;
		using CryptoPP::Exception;
		using std::cout;
		using std::cerr;
		using std::endl;
		using CryptoPP::HexEncoder;
		using CryptoPP::HexDecoder;
		using CryptoPP::StringSink;
		using CryptoPP::StringSource;
		using CryptoPP::StreamTransformationFilter;
		using CryptoPP::CBC_Mode;
		using CryptoPP::AES;
		
		
		std::string key = "7D227C0C5CFABF2945C8A32254EF7B647D227C0C5CFABF2945C8A32254EF7B64";
	  	byte bkey[key.length()];
	  	std::memcpy(bkey, key.data(), key.length());
		
		std::string iv = "5B11D58F752B51F1294DC38933BBB1105B11D58F752B51F1294DC38933BBB110";
	  	byte biv[iv.length()];
	  	std::memcpy(biv, iv.data(), iv.length());
	  	
	  	
		string cipher, encoded;
		try
		{
			//cout << "plain text: " << plainText << endl;
			CBC_Mode< AES >::Encryption e;
			e.SetKeyWithIV(bkey, sizeof(key), biv);
			StringSource s(plainText, true,new HexEncoder(new StringSink(encoded)));
			#if 0
				StreamTransformationFilter filter(e);
				filter.Put((const byte*)plain.data(), plain.size());
				filter.MessageEnd();
				const size_t ret = filter.MaxRetrievable();
				cipher.resize(ret);
				filter.Get((byte*)cipher.data(), cipher.size());
			#endif
		}
		catch (const CryptoPP::Exception& e)
		{
			cerr << e.what() << endl;
			exit(1);
		}
		//cout << "cipher text: " << encoded << endl;
		return encoded;
	}
	static std::string Decrypt(const std::string& cipherText) 
	{
		std::string newString;
		int len = cipherText.length();
		
		for(int i = 0; i < len; i += 2)
		{
			std::string byte = cipherText.substr(i,2);
			char chr = (char) (int) strtol(byte.c_str(), NULL, 16);
			newString.push_back(chr);
		}
		
		//cout << newString << endl;
	  	return newString;
	}
};
