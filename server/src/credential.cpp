#include "credential.hpp"
credential::credential(string username)
{
	m_username = username;
}
credential::credential(string username,string password)
{
	m_password = password;
	m_username = username;
}
bool credential::isvalidPassword(s3service::s3account& accountResponse)
{
	//check the lenght of password it should be greater then 8
	//null password is not allowed
	//need to have spl character
	//min 1 upper case and min 1 lower case
	//1 number
	int len = m_password.length();
	if(!((len >=8)  && (len <=16)))//&& (m_password !=NULL)
	{
		auto err = accountResponse.add_errorinfo();
		m_error = "Password must have min of 8 to max of 16 character " + m_password + " re-enter password";
		err->set_errortype("Password");
		err->set_errorcode("Invalid password");
		err->set_errormessage(m_error);
		return false;
	}
		if(m_password.find(" ")!=std::string::npos)
		{
			auto err = accountResponse.add_errorinfo();
			m_error = "Password has some space " + m_password + " re-enter password";
			err->set_errortype("Password");
			err->set_errorcode("Invalid password");
			err->set_errormessage(m_error);
			return false;
		}
		
		
		if (true)
               {
                	int count = 0;

			// Check digits from 0 to 9
			for(int i = 0; i <= 9; i++)
			{
			    
			    // To convert int to string
			    string str1 = to_string(i);

			    if (m_password.find(str1) !=std::string::npos)
				count = 1;
			}
			if (count == 0)
			{
				auto err = accountResponse.add_errorinfo();
				m_error = "Password must have atleast one number" + m_password + " re-enter password";
				err->set_errortype("Password");
				err->set_errorcode("Invalid password");
				err->set_errormessage(m_error);
				return false;
			}
    		}
    		
    	       if (!((m_password.find("@") != std::string::npos) || (m_password.find("#") != std::string::npos)))
    	       {
				auto err = accountResponse.add_errorinfo();
				m_error = "Password must have atleast one special character either @ or #" + m_password + " re-enter password";
				err->set_errortype("Password");
				err->set_errorcode("Invalid password");
				err->set_errormessage(m_error);
				return false;
	       }
               if (true)
    		{
        		int count = 0;

			// Checking capital letters
			for(int i = 65; i <= 90; i++)
			{
			    
			    // Type casting
			    char c = (char)i;
			    string str1(1, c);
			    
			    if (m_password.find(str1) != std::string::npos)
				count = 1;
			}
			if (count == 0)
			{
				auto err = accountResponse.add_errorinfo();
				m_error = "Password must have atleast one captial letter" + m_password + " re-enter password";
				err->set_errortype("Password");
				err->set_errorcode("Invalid password");
				err->set_errormessage(m_error);
				return false;
	                }
    		}
    		
    		
    		if (true)
    		{
			int count = 0;

			// Checking small letters
			for(int i = 97; i <= 122; i++)
			{
			    
			    // Type casting
			    char c = (char)i;
			    string str1(1, c);

			    if (m_password.find(str1) != std::string::npos)
				count = 1;
			}
			if (count == 0)
			{
				auto err = accountResponse.add_errorinfo();
				m_error = "Password must have atleast one small letter" + m_password + " re-enter password";
				err->set_errortype("Password");
				err->set_errorcode("Invalid password");
				err->set_errormessage(m_error);
				return false;
	                }
   		 }

    // If all conditions fails
    return true;

}
bool credential::isvalidusername(s3service::s3user& userResponse)
{
	int len = m_username.length();
	int letter=0;
	int letter1=0;
	if((len >= 5) && (len<=20)  )//&& (m_username != NULL)
	{
		for(int i=0;i<len;i++)
		{
			if(m_username[i] ==' ')
			{
				auto err = userResponse.add_errorinfo();
				m_error = "Name should not have space " + m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide Username");
				err->set_errormessage(m_error);
				return false;
			}
			else if( ((m_username[i] >=97 ) && (m_username[i] <=122)))
			{
				letter=1;
				continue;
			}
			else if((m_username[i] >='0') && (m_username[i] <= '9' ))
			{
				letter1++;
			}
			else if((m_username[i] >=65 ) && (m_username[i] <=90))
			{
				auto err = userResponse.add_errorinfo();
				m_error = "Name should not have captial letters " + m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
			}
			else if(m_username[i] !='_')
			{
				auto err = userResponse.add_errorinfo();
				m_error = "Name should not have special character other than '_' " + m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide Username");
				err->set_errormessage(m_error);
				return false;
			}
			else if(m_username[0] =='_')
			{
				auto err = userResponse.add_errorinfo();
				m_error = "Name should not start with '_' special character " + m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide Username");
				err->set_errormessage(m_error);
				return false;
			}
			else if(m_username[i] =='_'&& m_username[i+1] =='_')
			{
				auto err = userResponse.add_errorinfo();
				m_error = "Name should not have too many '_' continuously " + m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide Username");
				err->set_errormessage(m_error);
				return false;
			}
			
		}
		if(letter!=1){
				auto err = userResponse.add_errorinfo();
				m_error = "Name should not have only numbers " +  m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide Username");
				err->set_errormessage(m_error);
				return false;
		}
		else if(letter1 == len)
		{
				auto err = userResponse.add_errorinfo();
				m_error = "Name should not have only numbers " +  m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide Username");
				err->set_errormessage(m_error);
				return false;
		}
		else if((m_username[0]>='0' && m_username[0]<='9'))
		{
				auto err = userResponse.add_errorinfo();
				m_error = "Name should not have number first " + m_username;
				err->set_errortype("Account Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		auto err = userResponse.add_errorinfo();
		m_error = "Name should have length min of 5 character and max 20 character " + m_username;
		err->set_errortype("User Name"); 
		err->set_errorcode("Invalide Username");
		err->set_errormessage(m_error);	
		return false;
	}
	
}

bool credential::isvalidaccname(s3service::s3account& accountResponse)
{
	int len = m_username.length();
	int letter=0;
	int letter1=0;
	if((len >= 5) && (len<=20) )//&& (m_username != NULL)
	{
		for(int i=0;i<len;i++)
		{
			if(m_username[i] ==' ')
			{
				auto err = accountResponse.add_errorinfo();
				m_error = "Name should not have space " + m_username;
				err->set_errortype("Account Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
			}
			else if( (m_username[i] >=97 ) && (m_username[i] <=122) )
			{
				letter=1;
				continue;
			}
			else if(((m_username[i] >='0') && (m_username[i] <= '9' )))
			{
				letter1++;
				continue;
			}
			else if((m_username[i] >= 65 ) && (m_username[i] <=90))
			{
				auto err = accountResponse.add_errorinfo();
				m_error = "Name should not have captial letters " + m_username;
				err->set_errortype("Account Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
			}
			else if((m_username[i] !='_' ))
			{
				auto err = accountResponse.add_errorinfo();
				m_error = "Name should not have special character other than '_' " + m_username;
				err->set_errortype("Account Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
			}
			
			else if((m_username[0] =='_' ))
			{
				auto err = accountResponse.add_errorinfo();
				m_error = "Name should not start with '_' special character " + m_username;
				err->set_errortype("Account Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
			}
			else if((m_username[i] =='_' && m_username[i+1] =='_'))
			{
				auto err = accountResponse.add_errorinfo();
				m_error = "Name should not have too many '_' continuously " + m_username;
				err->set_errortype("Account Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
			}
			
		}
		if(letter!=1){
				auto err = accountResponse.add_errorinfo();
				m_error = "Name should not have only numbers " +  m_username;
				err->set_errortype("Account Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
		}
		else if(letter1 == len)
		{
				auto err = accountResponse.add_errorinfo();
				m_error = "Name should not have only numbers " +  m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide Username");
				err->set_errormessage(m_error);
				return false;
		}
		else if((m_username[0]>='0' && m_username[0]<='9'))
		{
				auto err = accountResponse.add_errorinfo();
				m_error = "Name should not have number first " + m_username;
				err->set_errortype("Account Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		auto err = accountResponse.add_errorinfo();
		m_error = "Name should have length min of 5 character and max 20 character " + m_username;
		err->set_errortype("Account Name"); 
		err->set_errorcode("Invalide accountname");
		err->set_errormessage(m_error);	
		return false;
	}
	
}

bool credential::isvalidbucketname(s3service::s3bucket& bucketResponse)
{
	int len = m_username.length();
	int letter=0;
	int letter1=0;
	if((len >= 5) && (len<=20)  )//&& (m_username != NULL)
	{
		for(int i=0;i<len;i++)
		{
			if(m_username[i] ==' ')
			{
				auto err = bucketResponse.add_errorinfo();
				m_error = "Name should not have space " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			}
			else if( ((m_username[i] >=97 ) && (m_username[i] <=122) ))
			{
				letter=1;
				continue;
			}
			else if(((m_username[i] >='0') && (m_username[i] <= '9' )))
			{
				letter1++;
				continue;
			}
			else if(( (m_username[i] >=65 ) && (m_username[i] <=90 ))){
				auto err = bucketResponse.add_errorinfo();
				m_error = "Name should not have capital letters" + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			
			}
			else if(m_username[i] !='_')
			{
				auto err = bucketResponse.add_errorinfo();
				m_error = "Name should not have special character other than '_' " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			}
			else if(m_username[0] =='_' )
			{
				auto err = bucketResponse.add_errorinfo();
				m_error = "Name should not start with '_' special character " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			}
			else if(m_username[i] =='_' && m_username[i+1] =='_' )
			{
				auto err = bucketResponse.add_errorinfo();
				m_error = "Name should not have too many '_' continuously " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			}
			
		}
		if(letter!=1){
				auto err = bucketResponse.add_errorinfo();
				m_error = "Name should not have only numbers " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
		}
		else if(letter1 == len)
		{
				auto err = bucketResponse.add_errorinfo();
				m_error = "Name should not have only numbers " +  m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide Username");
				err->set_errormessage(m_error);
				return false;
		}
		else if((m_username[0]>='0' && m_username[0]<='9'))
		{
				auto err = bucketResponse.add_errorinfo();
				m_error = "Name should not have number first " + m_username;
				err->set_errortype("Account Name"); 
				err->set_errorcode("Invalide accountname");
				err->set_errormessage(m_error);
				return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		auto err = bucketResponse.add_errorinfo();
		m_error = "Name should have length min of 5 character and max 20 character " + m_username;
		err->set_errortype("Bucket Name"); 
		err->set_errorcode("Invalide Bucketname");
		err->set_errormessage(m_error);	
		return false;
	}

}


bool credential::isvalidbucketnameObj(s3service::s3object& objectResponse)
{
	int len = m_username.length();
	int letter=0;
	int letter1=0;
	if((len >= 5) && (len<=20)  )//&& (m_username != NULL)
	{
		for(int i=0;i<len;i++)
		{
			if(m_username[i] ==' ')
			{
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have space " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			}
			else if( ((m_username[i] >=97 ) && (m_username[i] <=122) ))
			{
				letter=1;
				continue;
			}
			else if(((m_username[i] >='0') && (m_username[i] <= '9' )))
			{
				letter1++;
				continue;
			}
			else if(( (m_username[i] >=65 ) && (m_username[i] <=90 ))){
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have capital letters " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			
			}
			else if(m_username[i] !='_')
			{
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have special character other than '_' " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			}
			else if(m_username[0] =='_' )
			{
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not start with '_' special character " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			}
			else if(m_username[i] =='_' && m_username[i+1] =='_' )
			{
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have too many '_' continuously " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
			}
			
		}
		if(letter!=1){
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have only numbers " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
		}
		else if(letter1 == len)
		{
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have only numbers " +  m_username;
				err->set_errortype("User Name"); 
				err->set_errorcode("Invalide Username");
				err->set_errormessage(m_error);
				return false;
		}
		else if((m_username[0]>='0' && m_username[0]<='9'))
		{
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have number first " + m_username;
				err->set_errortype("Bucket Name"); 
				err->set_errorcode("Invalide Bucketname");
				err->set_errormessage(m_error);
				return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		auto err = objectResponse.add_errorinfo();
		m_error = "Name should have length min of 5 character and max 20 character " + m_username;
		err->set_errortype("Bucket Name"); 
		err->set_errorcode("Invalide Bucketname");
		err->set_errormessage(m_error);	
		return false;
	}

}


bool credential::isvalidObjectname(s3service::s3object& objectResponse)
{
	int len = m_username.length();
	int letter=0;
	int letter1=0;
		size_t extension_index = m_username.find_last_of(".");
		
		for(int i=0;i<len;i++)
		{	
			if(extension_index > i)
			{
				if(m_username[i] ==' ')
				{
					auto err = objectResponse.add_errorinfo();
					m_error = "Name should not have space " + m_username;
					err->set_errortype("Object Name"); 
					err->set_errorcode("Invalide Objectname");
					err->set_errormessage(m_error);
					return false;
				}
				
				else if( ((m_username[i] >=97 ) && (m_username[i] <=122) ))
				{
					letter=1;
					continue;
				}
				else if(((m_username[i] >='0') && (m_username[i] <= '9' )))
				{
					letter1++;
					continue;
				}
				else if(( (m_username[i] >=65 ) && (m_username[i] <=90 ))){
					auto err = objectResponse.add_errorinfo();
					m_error = "Name should not have capital letters " + m_username;
					err->set_errortype("Object Name"); 
					err->set_errorcode("Invalide Objectname");
					err->set_errormessage(m_error);
					return false;
				
				}
			}
			else if(extension_index < i)
			{
				if( ((m_username[i] >=97 ) && (m_username[i] <=122) ))
				{
					letter=1;
					continue;
				}
				else
				{
					auto err = objectResponse.add_errorinfo();
					m_error = "Name should have small letters " + m_username;
					err->set_errortype("Object Name"); 
					err->set_errorcode("Invalide Objectname");
					err->set_errormessage(m_error);
					return false;
				}
			
			}
		}
		if(letter!=1){
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have only numbers " + m_username;
				err->set_errortype("Object Name"); 
				err->set_errorcode("Invalide Objectname");
				err->set_errormessage(m_error);
				return false;
		}
		else if(letter1 == len)
		{
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have only numbers " +  m_username;
				err->set_errortype("Object Name"); 
				err->set_errorcode("Invalide Objectname");
				err->set_errormessage(m_error);
				return false;
		}
		else if((m_username[0]>='0' && m_username[0]<='9'))
		{
				auto err = objectResponse.add_errorinfo();
				m_error = "Name should not have number first " + m_username;
				err->set_errortype("Object Name"); 
				err->set_errorcode("Invalide Objecttname");
				err->set_errormessage(m_error);
				return false;
		}
		else
		{
			return true;
		}

}




bool credential::isvalidMarkerId(s3service::s3object& objectResponse)
{
	int len = m_username.length();
	int letter=0;
	int letter1=0;
		for(int i=0;i<len;i++)
		{	
				if(m_username[i] ==' ')
				{
					auto err = objectResponse.add_errorinfo();
					m_error = "Delete marker should not have space " + m_username;
					err->set_errortype("Delete MarketId"); 
					err->set_errorcode("Invalide Delete MarketId");
					err->set_errormessage(m_error);
					return false;
				}
				
				else if( ((m_username[i] >=97 ) && (m_username[i] <=122) ))
				{
					letter=1;
					continue;
				}
				else if(((m_username[i] >='0') && (m_username[i] <= '9' )))
				{
					letter1++;
					continue;
				}
				else if(( (m_username[i] >=65 ) && (m_username[i] <=90 ))){
					auto err = objectResponse.add_errorinfo();
					m_error = "Delete marker should not have capital letters " + m_username;
					err->set_errortype("Delete MarketId"); 
					err->set_errorcode("Invalide Delete MarketId");
					err->set_errormessage(m_error);
					return false;
				
				}
				else
				{
					auto err = objectResponse.add_errorinfo();
					m_error = "Delete marker should not allow special character " + m_username;
					err->set_errortype("Markerid Name"); 
					err->set_errorcode("Invalide Markeridname");
					err->set_errormessage(m_error);
					return false;
				}
		}
		if(letter!=1){
				auto err = objectResponse.add_errorinfo();
				m_error = "Delete marker should not have only numbers " + m_username;
				err->set_errortype("Delete MarketId "); 
				err->set_errorcode("Invalide Delete MarketId");
				err->set_errormessage(m_error);
				return false;
		}
		else if(letter1 == len)
		{
				auto err = objectResponse.add_errorinfo();
				m_error = "Delete marker should not have only numbers " +  m_username;
				err->set_errortype("Delete MarketId"); 
				err->set_errorcode("Invalide Delete MarketId");
				err->set_errormessage(m_error);
				return false;
		}
		else if((m_username[0]>='0' && m_username[0]<='9'))
		{
				auto err = objectResponse.add_errorinfo();
				m_error = "Delete marker should not have number first " + m_username;
				err->set_errortype("Delete MarketId"); 
				err->set_errorcode("Invalide Delete MarketId");
				err->set_errormessage(m_error);
				return false;
		}
		else
		{
			return true;
		}

}

credential::~credential()
{
}
