#include "ldapserver.hpp"
#include<iostream>
using namespace std;

//Get an handler to an LDAP connection
Ldap::Ldap()
{
		char url_base[]="ldap://localhost";
		char *url=url_base;
		int status=ldap_initialize(&ldp,url);
		if(status!=LDAP_SUCCESS)
		{
			cerr<<"ldap init failed "<<ldap_err2string(status)<<endl;
		}
		else
		{
			cout<<"ldap init successfull"<<endl;
		}
}
void Ldap::connect()
{
        int ldap_version=LDAP_VERSION3;

        int res=ldap_set_option(ldp,LDAP_OPT_PROTOCOL_VERSION,&ldap_version);
        if(res!=LDAP_SUCCESS)
        {
            cerr<<"Ldap_set_option"<<ldap_err2string(res)<<endl;

        }
        else
        {
            cout<<"Set LDAPv3 client version"<<endl;
        }
}

//Bind to the server
void Ldap::bindServer(char *ldapDN ,char *passWord)
{
    	struct berval **cred=NULL;
    	struct berval servercred;
    	servercred.bv_val=passWord;
    	//servercred.bv_len=sizeof("prabh@123")-1;
    	servercred.bv_len=strlen(passWord);

        int result=ldap_sasl_bind_s(ldp,ldapDN,LDAP_SASL_SIMPLE,&servercred,NULL,NULL,cred);
        if(result!=LDAP_SUCCESS)
        {
            cerr<<"ldap_sasl_bind_s "<<ldap_err2string(result)<<endl;
        }
        else{
            cout<<"ldap connnection successfull"<<endl;

        }
}

void Ldap::addEntry(char *newEntry,char *cnValue,char *snValue)
{
		char object_val1[4]="top";
		char object_val2[10]="Person";
		char object_val3[22]="organizationalPerson";
		char object_val4[20]="inetOrgPerson";
	    char *object_vals[6]={object_val1,object_val2,object_val3,object_val4,NULL};
	    char *cn_vals[3]={cnValue,NULL};
	    char *sn_vals[3]={snValue,NULL};
    	ldapmod **mods;
        int mode=3;

        //construct the array of lDaPMod structure representing an attributes of the new entry
        mods=(LDAPMod**) malloc((mode+1) * sizeof(LDAPMod *));
        if(mods==NULL){
            cout<<"couldn't allocate a memory"<<endl;
            exit(1);
        }

        for(int i=0;i<=mode;i++)
        {
            if((mods[i]=(LDAPMod *)malloc(sizeof(LDAPMod)))==NULL)
            {
                cout<<"can' allocate memory for modes elements"<<endl;
                exit(1);
            }
        }

        mods[0]->mod_op=0;
        mods[0]->mod_type=(char *)("objectclass");
        mods[0]->mod_values=object_vals;
        mods[1]->mod_op=0;
        mods[1]->mod_type=(char *)("cn");
        mods[1]->mod_values= cn_vals;
        mods[2]->mod_op=0;
        mods[2]->mod_type =(char *)"sn";
        mods[2]->mod_values= sn_vals;
        mods[3]=NULL;

        //Perform an add operation`
        int rc=ldap_add_ext_s(ldp,newEntry,mods,NULL,NULL);

        if(rc!=LDAP_SUCCESS)
        {
        	cout<<"result"<<endl;
            cerr<<"ldap_add_ext_s "<<ldap_err2string(rc)<<endl;


        }
        else{
            cout<<"Entry added successfully"<<newEntry<<endl;
        }

        ldap_memfree(mods);
}


void Ldap::deleteEntry(char *entry)
{
        int result=0;
        result=ldap_delete_ext_s(ldp,entry,NULL,NULL);
        if(result!=LDAP_SUCCESS)
        {
            cerr<<"lda_delete_ext_s failure"<<ldap_err2string(result)<<endl;

        }
        else{
            cout<<"deleted sucessfully"<<endl;
        }
}

void Ldap::searchEntry(const char *sBaseDN,char *filter)
{
    	LDAPMessage *res=NULL,*e;
    	int scope=LDAP_SCOPE_SUBTREE;
        int enteries_found=0;
        char *dn;
        int sr_res=ldap_search_ext_s(ldp,sBaseDN,scope,filter,NULL,0,NULL,NULL,NULL,0,&res);
        if(sr_res!=LDAP_SUCCESS)
        {
            cerr<<"ldap_search_ext s failure"<<ldap_err2string(sr_res)<<endl;

        }
        else{
            cout<<"search successfull"<<endl;

        }
        enteries_found=ldap_count_entries(ldp,res);
        if(enteries_found==0)
            cout<<"ldap serach didn't return any data"<<endl;
        else
        {
            cout<<"ldap search returned "<<enteries_found<<" objects"<<endl;
        }

        /*cycle to iterate over the found enteries*/

        for(e=ldap_first_entry(ldp,res);e!=NULL;e=ldap_next_entry(ldp,e))
        {
            if((dn=ldap_get_dn(ldp,e))!=NULL)
            {
                cout<<dn<<endl;
                ldap_memfree(dn);
            }
        }

        ldap_msgfree(res);
}

Ldap::~Ldap()
{
       //ldap_msgfree(res);
       ldap_unbind_ext_s(ldp,NULL,NULL);
}
