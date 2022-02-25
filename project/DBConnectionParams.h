/*
 * =====================================================================================
 *
 *       Filename:  dbconnection.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/04/2019 12:35:55 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#ifndef DBCONNECTION_H
#define DBCONNECTION_H 
#include "cpptoml.h"
#include <iostream>
#include "dbmanage.h"
#include <memory>
using namespace std;
using namespace std::string_literals;

namespace db {

	typedef enum DbProvider {
		err=0,
		postgres = 1,
		mssql
	} DbProvider;

	struct DBConnectionParamsException : public std::exception
	{
		DBConnectionParamsException(std::string msg) {
			m_msg = msg;
		}
		const char * what () const throw ()
		{
			return m_msg.c_str();
		}
		std::string m_msg;
	};


	class DBConnectionParams {
		public:
			DBConnectionParams(std::string user,std::string password,std::string dbname,std::string hostaddr,long int port ,DbProvider type ):
				m_user(user),
				m_password(password),
				m_hostaddr(hostaddr),			
				m_dbname(dbname),      
				m_port(port) ,
				m_dbtype(type )
		{
		}
			DBConnectionParams(){};
			void readConfiguration(std::string filename)
			{
			}
			std::string getConnectionString()
			{
				std::stringstream str;
				str<<"dbname="<<m_dbname<<" user="<<m_user<<" password="<<m_password<<" hostaddr="<<m_hostaddr<<" port="<<m_port;
				return str.str();
			}
			void setUser(std::string user) {
				m_user = user;
			}
			void setPassword(std::string pass) {
				m_password = pass;
			}
			void setPort(long int port) {
				m_port = port;
			}
			void setHosrAddr(std::string addr) {
				m_hostaddr = addr;
			}
			void setDbName(std::string dbname) {
				m_dbname = dbname;
			}
			void setType(DbProvider t) {
				m_dbtype  = t;
			}

			/**
			 * Created  08/03/2019
			 * @brief   read config from /etc/dataserver.conf. based on cpptoml @see https://github.com/skystrife/cpptoml
			 * @param   
			 * @return  
			 */
			void getConfig(std::string filename ) {
				try
				{
					std::shared_ptr<cpptoml::table> config = cpptoml::parse_file(filename);
					auto db = config->get_table("db");

					auto port = db->get_as<int>("port");
					if (port)
						setPort (  *port)   ;
					else {
						throw DBConnectionParamsException("no sql port");
					//	cout<<"no sql port"<<endl;
					//	return false;
					}
					auto hostaddr = db->get_as<string>("hostaddr");
					if (hostaddr)
						setHosrAddr (  *hostaddr)   ;
					else {
						throw DBConnectionParamsException("no sql hostaddr");
					//	cout<<"no sql hostaddr"<<endl;
					//	return false;
					}

					auto dbname = db->get_as<string>("dbname");
					if (dbname)
						setDbName (  *dbname)   ;
					else {
						throw DBConnectionParamsException("no sql dbname");
						//cout<<"no sql dbname"<<endl;
						//return false;
					}

					auto user = db->get_as<string>("user");
					if (dbname)
						setUser (  *user)   ;
					else {
						throw DBConnectionParamsException("no sql user");
					//	cout<<"no sql user"<<endl;
					//	return false;
					}

					auto password = db->get_as<string>("password");
					if (password)
						setPassword (  *password)   ;
					else {
						throw DBConnectionParamsException("no sql password");
					//	cout<<"no sql password"<<endl;
					//	return false;
					}
					auto type = db->get_as<string>("type");
					if (password)
						setType (  (*type=="psql" ? DbProvider::postgres  : *type=="mssql" ? DbProvider::mssql  : DbProvider::err      ))   ;
					else {
						throw DBConnectionParamsException("no sql type");
					//out<<"no sql type"<<endl;
					//eturn false;
					}


				//	return true;


				}
				catch (const cpptoml::parse_exception& e)
				{
					throw DBConnectionParamsException("data server: cannot read confiurationfile "s +  e.what());
				//	std::cerr << "data server: cannot read confiurationfile " << ": " << e.what() << std::endl;
				//	return false;
				}
			}

		private:
			std::string m_user;
			std::string m_password;
			std::string m_hostaddr ;
			std::string m_dbname ;
			long int m_port;
			DbProvider m_dbtype;
	};
}
#endif
