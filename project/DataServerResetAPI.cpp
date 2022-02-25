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

#include "dataServer/dbmanage.h"
#include "dataServer/DBConnectionParams.h"
#include "dataServer/data_server_db.h"

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
	DBConnectionParams.getConfig (DATASERVER_CONGIH_FILE_PATH );


	auto  psql = std::make_unique<db::psql>(DBConnectionParams.getConnectionString());
	if (psql != nullptr) {
		data_server.set_db(std::move(psql));
	} else {
		throw db::psqlException();
	}

	return data_server;
}

void DataRestServer::get_broker_info_handler( const shared_ptr< Session > session )
{

	auto func = [this](shared_ptr< restbed::Session > session) ->Json::Value {
		const auto& request = session->get_request( );
		std::string acountID = request->get_query_parameter("id");
		auto data_server =  create_db_instance();
		auto root = data_server.brokers_info(std::stoi(acountID));
		return root;

	};

	// validate "id" parameter
	get_handler_callback(session ,  func, {"id"});
}

void DataRestServer::gethandler_getAccountList( const shared_ptr< Session > session )
{
	auto func = [this](shared_ptr< restbed::Session > session) ->Json::Value {
		auto data_server =  create_db_instance();		
		
		auto root = data_server.accouns_list();
		return root;

	};

	// validate "id" parameter
	get_handler_callback(session ,  func);
}



//curl -w'\n' -v -X GET -d "value":"30","type":"Tip 3" 'http://localhost:1984/fxcm-account-info/1'
void DataRestServer::gethandler_getAccountInfo( const shared_ptr< Session > session )
{

	auto func = [this](shared_ptr< restbed::Session > session) ->Json::Value {
		const auto& request = session->get_request( );
		std::string acountID = request->get_query_parameter("id");
		auto data_server =  create_db_instance();		
		
		auto root = data_server.account_info(std::stoi(acountID));
		return root;

	};

	// validate "id" parameter
	get_handler_callback(session ,  func, {"id"});
}


//curl -w'\n' -v -X GET -d "value":"30","type":"Tip 3" 'http://localhost:1984/contracts/1'
void DataRestServer::gethandler_getExtractContractsData( const shared_ptr< Session > session )
{

	auto func = [this](shared_ptr< restbed::Session > session) ->Json::Value {
		auto data_server =  create_db_instance();				
		auto root = data_server.instrument_table();
		return root;

	};

	get_handler_callback(session ,  func);

}



void DataRestServer::gethandler_getRegisteredSymbols( const shared_ptr< Session > session )
{

	auto func = [this](shared_ptr< restbed::Session > session) ->Json::Value {
		const auto& request = session->get_request( );
		std::string acountID = request->get_query_parameter("id");
		auto data_server =  create_db_instance();				
		auto root = data_server.instruments(std::stoi(acountID));
		return root;

	};

	// validate "id" parameter
	get_handler_callback(session ,  func, {"id"});
}

void DataRestServer::gethandler_getLastUpdateTime( const shared_ptr< Session > session )
{

	auto func = [this](shared_ptr< restbed::Session > session) ->Json::Value {
		auto data_server =  create_db_instance();		
		auto root = data_server.last_update_time();
		return root;

	};

	get_handler_callback(session ,  func);

}



void DataRestServer::gethandler_getOpenTrades( const shared_ptr< Session > session )
{

	auto func = [this](shared_ptr< restbed::Session > session) ->Json::Value {
		const auto& request = session->get_request( );
		std::string acountID = request->get_query_parameter("id");
		auto data_server =  create_db_instance();				
		auto root = data_server.open_trades(std::stoi(acountID));
		return root;

	};

	// validate "id" parameter
	get_handler_callback(session ,  func, {"id"});
}




void DataRestServer::service_ready_handler( Service& )
{
	fprintf( stderr, "Hey! The service is up and running. \n" );
}


// curl -w'\n' -v -X POST -d '{"account":"1","symbol_id":"1","number_of_contructs":"3"}' 'http://localhost:1985/buy'
void DataRestServer::posthandler_AddTrade( const std::shared_ptr< Session > session )
{
	auto func = [this](shared_ptr< restbed::Session > session,Json::Value root) ->Json::Value {
		Json::Value root_out;
		if ( root["error"].asInt64()==0) {
			auto data_server =  create_db_instance();		
			auto instrument= root["instrument-id"].asInt();
			//				auto offer_id =  "deprecated";//root["offer-id"].asString();
			auto open_orders_id = root["order-id"].asString();
			int open_positions = root["open-positions"].asInt();
			int lot_size = root["lot-size"].asInt();
			auto account= root["account-id"].asString();
			auto tradeid= root["trade-id"].asString();
			auto rate= root["rate"].asDouble();

			std::stringstream query; 
			query<<"insert into account_"<<account<<".trades(OpenOrderID,InstrumentId,lotSize,openContracts,tradeId,rate) values('"<<open_orders_id<<"','"<<instrument<<"',"<<lot_size<<","<<open_positions<<","<<tradeid<<","<<rate<<""<<");";

			Json::Value root_out = data_server.execute(query.str());				
		}
		return root_out;


	};

	post_handler_callcback(session ,  func);
}

// curl -w'\n' -v -X POST -d '{"account":"1","symbol_id":"1","number_of_contructs":"3"}' 'http://localhost:1985/buy'
void DataRestServer::posthandler_postAddTickData( const std::shared_ptr< Session > session )
{
	auto func = [this](shared_ptr< restbed::Session > session,Json::Value root) ->Json::Value {
		
		auto bid = root["bid"].asDouble();
		auto ask = root["ask"].asDouble();
		auto bidsize = root["bid-size"].asInt();
		auto asksize = root["ask-size"].asInt();
		auto time = root["time"].asDouble();
		auto tablename = root["table-name"].asString();
		auto accout = root["account"].asInt();
		auto symbol_id = root["symbol-id"].asInt();

		auto data_server =  create_db_instance();		
		
		std::stringstream query; 
		query<<" insert into account_"<<accout<<"."<<tablename<<"(createdate,bid,ask) values ("
			<<"to_timestamp("<<std::setprecision(6)<<std::fixed<<(double)(time/1000000.0)<<")" 
			<<","<< std::setprecision(6)<<std::fixed<<bid
			<<","<< std::setprecision(6)<<std::fixed<<ask<<")"; 

		Json::Value root_out = data_server.execute(query.str());

		// update the last time where a specific symbol was record
		data_server.update_last_tick_time(symbol_id);

		return root_out;

	};

	post_handler_callcback(session ,  func);

}






int DataRestServer::start(std::vector<std::thread> &workers,int port )
{
	auto resource = make_shared< Resource >( );
	resource->set_path( "/broker-info/" );
	resource->set_method_handler( "GET", std::bind(&DataRestServer::get_broker_info_handler,this,std::placeholders::_1) );


		


	// @see sql->getAccountList
	auto getAccountList = make_shared< Resource >( );
	getAccountList->set_path( "/account-list/" );
	getAccountList->set_method_handler( "GET", std::bind(&DataRestServer::gethandler_getAccountList,this,std::placeholders::_1) );


	// @see sql->getAccountList
	auto getAccountInfo = make_shared< Resource >( );
	getAccountInfo->set_path( "/account-info/" );
	getAccountInfo->set_method_handler( "GET",std::bind(&DataRestServer::gethandler_getAccountInfo,this,std::placeholders::_1)  );







	// @see sql->getAccountList
	auto getExtractContractsData = make_shared< Resource >( );
	getExtractContractsData->set_path( "/contracts/" );
	getExtractContractsData->set_method_handler( "GET", std::bind(&DataRestServer::gethandler_getExtractContractsData,this,std::placeholders::_1) );


	// @see sql->gethandler_getRegisteredSymbols
	auto getRegisteredSymbols = make_shared< Resource >( );
	getRegisteredSymbols->set_path( "/registered-symbols/" );
	getRegisteredSymbols->set_method_handler( "GET", std::bind(&DataRestServer::gethandler_getRegisteredSymbols,this,std::placeholders::_1));
	// @see sql->gethandler_getLastUpdateTime
	auto getLastUpdateTime = make_shared< Resource >( );
	getLastUpdateTime->set_path( "/last-update-time/" );
	getLastUpdateTime->set_method_handler( "GET",std::bind(&DataRestServer::gethandler_getLastUpdateTime,this,std::placeholders::_1)  );








	// @see sql->getAccountList
	auto getOpenTrades = make_shared< Resource >( );
	getOpenTrades->set_path( "/open-trades/" );
	getOpenTrades->set_method_handler( "GET", std::bind(&DataRestServer::gethandler_getOpenTrades,this,std::placeholders::_1) );

	

#if 0
	// @see sql->getAccountList
	auto getFxcmTrades = make_shared< Resource >( );
	getFxcmTrades->set_path( "/fxcm/trades/" );
	getFxcmTrades->set_method_handler( "GET", gethandler_getFxcmTrades );
#endif

	//	auto resource1 = make_shared< Resource >( );
	//	resource1->set_path( "/resource1" );
	//	resource1->set_method_handler( "POST", post_method_handler );


	auto postAddFxcmTrade = std::make_shared< Resource >( );
	postAddFxcmTrade->set_path( "/add-trade/" );
	postAddFxcmTrade->set_method_handler( "POST", std::bind(&DataRestServer::posthandler_AddTrade,this,std::placeholders::_1));


	auto postAddTickData = std::make_shared< Resource >( );
	postAddTickData->set_path( "/add-tick-data/" );
	postAddTickData->set_method_handler( "POST", std::bind(&DataRestServer::posthandler_postAddTickData,this,std::placeholders::_1));






	auto settings = make_shared< Settings >( );
	settings->set_port( port );

	auto service = make_shared< Service >( );
	//service->publish( resource1 );
	service->publish( getAccountList );
	service->publish( getAccountInfo );
	//	service->publish( getBitstampLoginPrm );
	service->publish( getExtractContractsData );
	//	service->publish( getActiveStrategiesInfo );
	//service->publish( getActiveStrategy );
	//	service->publish( getFxcmTrades );
	service->publish( postAddFxcmTrade );
	service->publish( getRegisteredSymbols );
	service->publish( postAddTickData );
	service->publish( getOpenTrades );
	service->publish( getLastUpdateTime );




	service->publish( resource );
	service->set_ready_handler( std::bind(&DataRestServer::service_ready_handler,this,std::placeholders::_1) );

	workers.push_back(std::thread([service,settings]() 
				      {
					      std::cout << "log:  initilize restserver thread"<<std::endl;;
					      service->start( settings );
				      }));
	return EXIT_SUCCESS;
}


