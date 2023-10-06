#include "mssql.hpp"

std::string database::createConnection(const std::string& serverName, const std::string& userName, const std::string& password)
{
    auto retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    SQLCHAR retconstring[1024];
    SQLSMALLINT outlen;
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
	    return "SQLAllocHandle failed for Enviornment Variable";
	}

	retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
	    return "SQLSetEnvAttr failed";
	}

	retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		return "SQLAllocHandle failed for HDBC Variable";
	}

	retcode =  SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		return "SQLSetConnectAttr failed";
	}

	retcode = SQLConnect(hdbc, (SQLCHAR*) serverName.c_str(), SQL_NTS, (SQLCHAR*) userName.c_str(), userName.size(), (SQLCHAR*) password.c_str(), password.size());
	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		return "SQLConnect failed";
	}

	return std::string();
}

int database::executeQuery(const std::string& query)
{
	auto retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
	    return retcode;
	}

	retcode = SQLExecDirect(hstmt, (SQLCHAR*) query.c_str(), SQL_NTS);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		return retcode;
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

	return 0;
}

int database::executeQuery(const std::string& query, std::vector<std::vector<std::string>>& out)
{
    auto retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		return retcode;
	}

	retcode = SQLExecDirect(hstmt, (SQLCHAR*) query.c_str(), SQL_NTS);

	if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO))
	{
		return retcode;
	}

	SQLSMALLINT columns = 0;

	SQLNumResultCols(hstmt, &columns);

	while (SQL_SUCCEEDED(retcode = SQLFetch(hstmt)))
	{

		SQLUSMALLINT i;
		std::vector<std::string> row;

		try
		{
			for (i = 1; i <= columns; i++)
			{
				SQLINTEGER indicator;
				char buf[2048]= {'\0'};

				retcode = SQLGetData(hstmt, i, SQL_C_CHAR, buf, sizeof(buf), (SQLLEN*) &indicator);

				if (SQL_SUCCEEDED(retcode))
				{
					if (indicator == SQL_NULL_DATA)
					{
						strcpy(buf, "NULL");
					}

					if(buf != NULL)
					{
						std::string s;
						s = buf;

						if (!s.empty())
						{
							row.push_back(s);
						}
					}
				}
			}
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}

		out.push_back(row);
	}


	 SQLFreeHandle(SQL_HANDLE_STMT, hstmt);

	 return 0;
}

database::~database()
{
	 SQLDisconnect(hdbc);
	 SQLFreeHandle(SQL_HANDLE_DBC, hdbc);
	 SQLFreeHandle(SQL_HANDLE_ENV, henv);
}

