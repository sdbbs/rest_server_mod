/*
 * =====================================================================================
 *
 *       Filename:  data_server_db.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/24/2022 09:44:42 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef data_server_db
#define data_server_db 
#include "dbmanage.h"
namespace db {
	class data_server: public manage {
		public:
			data_server() = default;
			data_server(const db::data_server&)  {
			}


			Json::Value names(int age) {
				std::stringstream sql;
				if (age == -1)
					sql<< "select * from names";
				else
					sql<< "select * from names where age="<<age<<";";
				return query(sql.str(),{"id","name-tag","age-tag"});
			}





	};
}
#endif
