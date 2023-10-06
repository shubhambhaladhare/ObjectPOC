#include <iostream>
#include <string>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <stdio.h>
#include <cstdlib>
#include <string.h>
#include <vector>

#pragma once

using namespace std;

class database
{
   SQLHENV henv;
   SQLHDBC hdbc;
   SQLHSTMT hstmt;
   SQLRETURN retcode;

   public:

   std::string createConnection(const std::string& serverName, const std::string& userName, const std::string& password);

   int executeQuery(const std::string& query);

   int executeQuery(const std::string& query, std::vector<std::vector<std::string>>& out);

   ~database();
};
