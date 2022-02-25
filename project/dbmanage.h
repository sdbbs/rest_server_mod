/*
 * =====================================================================================
 *
 *       Filename:  dbmanage.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/23/19 07:23:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef dbmanage_h1
#define dbmanage_h1
#include <string>       // std::string
#include <iostream>     // std::cout
#include <json/value.h>
#include <sstream>      // std::stringstream
#include <string>
#include <iostream>
#include <iomanip>      
#include <exception>

#include <pqxx/pqxx>



namespace db {
	//https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
	//http://peterforgacs.github.io/2017/06/25/Custom-C-Exceptions-For-Beginners/

	struct psqlException : public std::exception
	{
		const char * what () const throw ()
		{
			return "cannot access to psql database";
		}
	};

	class idb {
		public:
			virtual Json::Value query(const std::string & sql, const std::vector<std::string> & f = {} ) = 0 ;
			virtual bool execute(std::string sql)  = 0;
			virtual void close()  = 0;

	};

	class psql:public idb {
		public:
			psql(std::string connection_str):
				m_postgressConnection (connection_str) {};


			void close() override{
				m_postgressConnection.close();
			}
			/**
			 * Created  10/21/2021
			 * @brief   returns json accoring to query
			 * @param   
			 * @return  
			 */
			Json::Value query(const std::string &  sql, const std::vector<std::string> & f={}) override 
			{
				Json::Value root;

				/* Reads symbols table */

				try {
					/* Create a non-transactional object. */
					pqxx::nontransaction N( m_postgressConnection);

					/* Execute SQL query */
					pqxx::result R(N.exec(sql));

					/* List down all the records */
					int idx = 0;
					for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
						for (int j = 0; j < c.size(); j++) {
							std::string name;
							if (j < f.size() )
								name = f[j];
							else
								name = R.column_name(j);	
							if (!c[j].is_null())
								root[idx][name] = c[j].as<std::string>();
							else
								root[idx][name] = "empty";


						}
						idx++;
					}
				} catch (const std::exception &e)  {
					root["error"] = -1;
					root["msg"] = e.what();
					std::cerr << e.what() << std::endl;
				}
				return root;
			}

			bool execute(std::string sql) override 
			{
				pqxx::work W(m_postgressConnection);
				try {
					W.exec( sql );
					W.commit();
				} catch (const std::exception &e) {
					W.abort();
					std::cerr << e.what() << std::endl;
					return false;
				}
				return true;
			}


		private:
			pqxx::connection  m_postgressConnection;


	};
	class manage {
		public:
			manage() = default;
			~manage() {
				if (m_db != nullptr)
					m_db->close();
			}
			void set_db(std::unique_ptr<idb> db ) {
				m_db = std::move(db);
			}
			Json::Value query(std::string q, const std::vector<std::string> & f = {} ) {
				return m_db->query(q,  f);
			}
			void close () {
				m_db->close();
			}
			bool execute(std::string sql) {
				return m_db->execute(sql);
			}
		private:
			std::unique_ptr<idb> m_db;

	};
};


#endif
