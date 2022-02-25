/*
 * =====================================================================================
 *
 *       Filename:  DatServerResetAPI.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/02/2021 08:03:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef DatAServerResetAPI
#define DatAServerResetAPI

#include <restbed>
#include <thread>
#include <vector>
#include "IRestServer.h"
#include "DBConnectionParams.h"

#include "data_server_db.h"

class DataRestServer: public IRestServer {
	public:
		int start(std::vector<std::thread> &workers,int port ) override;
		DataRestServer( ) = default ;
	
	
	private:
		void gethandler_getNames( const std::shared_ptr< restbed::Session > session );


		void posthandler_postAddTickData( const std::shared_ptr< restbed::Session > session );
		
		void service_ready_handler( Service& ) override;
		db::data_server create_db_instance() ;
		
		Json::Value secondary_post_handler_callback( std::shared_ptr< restbed::Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session,  Json::Value root)>  callback, const std::vector<std::string> & ids = {} ) override;
		Json::Value secondary_get_handler_callback( std::shared_ptr< restbed::Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session)>  callback, const std::vector<std::string> & ids = {} ) override;

};


#endif
