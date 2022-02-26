/*
 * =====================================================================================
 *
 *       Filename:  DatServerResetAPI.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/02/2021 08:03:34 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "DataServerResetAPI.h"
#include <functional>
#include <iostream>
#include <sstream>
#include <cstring>
#include <thread>
#include <string>
#include <vector>

#include <json/json.h>

#include "dbmanage.h"
#include "DBConnectionParams.h"
#include "data_server_db.h"

using namespace restbed;



/**
 * Created  02/23/2022
 * @brief   this function make sure to wrap the call back with all necessary try - catch blocks
 * @param   
 * @return  
 */
Json::Value DataRestServer::secondary_get_handler_callback( std::shared_ptr< restbed::Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session)>  callback, const std::vector<std::string> & ids )  {
	Json::Value root;

	try {
		root = callback (session);

	} catch (InvalidAccountIDException e) {
		root["error-msg"]=e.what();
		root["error"] = -1;

	} /*catch (const  std::exception &e) {
		root["error-msg"]=e.what();
		root["error"] = -4;
	}*/
	catch (const db::DBConnectionParamsException  e) {
		root["error-msg"]=e.what();
		root["error"] = -4;
	} catch (const db::psqlException  e) {
		root["error-msg"]=e.what();
		root["error"] = -5;
	} 
	return root;

}
/**
 * Created  02/23/2022
 * @brief   this function make sure to wrap the call back with all necessary try - catch blocks
 * @param   
 * @return  
 */
Json::Value DataRestServer::secondary_post_handler_callback( std::shared_ptr< restbed::Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session, Json::Value root)>  callback, const std::vector<std::string> & ids )  {
	Json::Value root;

	try {
		root = callback (session,root);

	} catch (InvalidAccountIDException &e) {
		root["error-msg"]=e.what();
		root["error"] = -1;

	} catch (const db::DBConnectionParamsException & e) {
		root["error-msg"]=e.what();
		root["error"] = -4;
	} catch (const db::psqlException & e) {
		root["error-msg"]=e.what();
		root["error"] = -5;
	} 
	return root;

}


db::data_server DataRestServer::create_db_instance() {
	db::data_server  data_server;

	db::DBConnectionParams DBConnectionParams;
	DBConnectionParams.getConfig ("/etc/appserver.conf"   );


	auto  psql = std::make_unique<db::psql>(DBConnectionParams.getConnectionString());
	if (psql != nullptr) {
		data_server.set_db(std::move(psql));
	} else {
		throw db::psqlException();
	}

	return data_server;
}


void DataRestServer::gethandler_getNames( const shared_ptr< Session > session )
{

	auto func = [this](shared_ptr< restbed::Session > session) ->Json::Value {
		Json::Value root;
		const auto& request = session->get_request( );
		auto data_server =  create_db_instance();				
		
		std::string age = request->get_query_parameter("age");
		if (age == "") 
			 root = data_server.names(-1);
		else
			 root = data_server.names(std::stoi(age));
		return root;

	};

	// validate "id" parameter
	get_handler_callback(session ,  func);
}



void DataRestServer::service_ready_handler( Service& )
{
	fprintf( stderr, "Hey! The service is up and running. \n" );
}





int DataRestServer::start(std::vector<std::thread> &workers,int port )
{


	auto getNames = make_shared< Resource >( );
	getNames->set_path( "/names/" );
	getNames->set_method_handler( "GET", std::bind(&DataRestServer::gethandler_getNames,this,std::placeholders::_1) );



	auto settings = make_shared< Settings >( );
	settings->set_port( port );
	auto service = make_shared< Service >( );
	service->publish( getNames );




	service->set_ready_handler( std::bind(&DataRestServer::service_ready_handler,this,std::placeholders::_1) );

	workers.push_back(std::thread([service,settings]() 
				      {
					      std::cout << "log:  initilize restserver thread"<<std::endl;;
					      service->start( settings );
				      }));
	return EXIT_SUCCESS;
}


