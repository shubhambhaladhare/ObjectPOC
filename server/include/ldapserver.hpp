#pragma once

#include<string>
#include<ldap.h>
using namespace std;

class Ldap
{
	LDAP *ldp;
	//char *url;

public:
	Ldap();

	void connect();

	void bindServer(char *ldapDN, char *passWord);

	void addEntry(char *aEntry,char *cnValue,char *snValue);

	void deleteEntry(char *dEntry);

	void searchEntry(const char *sBaseDN,char *filter);

	void setUrl();

	void ldapDn();


	~Ldap();
};
