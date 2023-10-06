#pragma once


#include<iostream>
#include<exception>
#include<sstream>

class S3Exception : public std::exception
{
private : 
	std::stringstream Formatedmessage;
	std :: string message;
	std::string Filename;
	std::string FunctionName; 
	unsigned long LineNo; 

public:

	S3Exception(std::string Msg, std::string FunctionName = "",
		std::string Filename = "", unsigned long LineNo = 0 )
	{
		this->Filename.clear();
		this->FunctionName.clear();
		this->LineNo = 0;
		this->message = Msg;
		Formatedmessage << Msg;

		if (!FunctionName.empty())
		{
			Formatedmessage << " :: " << FunctionName;
			this->FunctionName = FunctionName;
		}
		if (!Filename.empty())
		{
			Formatedmessage << " :: " << Filename;
			this->Filename = Filename;
		}
		if (LineNo)
		{
			Formatedmessage << "(" << LineNo << ")";
			this->LineNo = LineNo; 
		}
	}

	virtual std::string what()
	{
		return this->message;
	}

	std::string getFormatedMessage()
	{
		return this->Formatedmessage.str();
	}

	std::string getFilename()
	{
		return this->Filename;
	}

	std::string getFunctionName()
	{
		return this->FunctionName;
	}

	unsigned long getLineNo()
	{
		return this->LineNo; 
	}
};
