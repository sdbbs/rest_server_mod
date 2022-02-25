/*
 * =====================================================================================
 *
 *       Filename:  IRestServer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/23/2022 09:52:14 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#ifndef IRestServer_H
#define IRestServer_H
#include <restbed>
#include <memory>
#include <vector>
#include <string>
#include <exception>
#include <json/json.h>

//#include "brokers/connection.h"
//#include "brokers/SystemTop.h"


using namespace restbed;
struct IRestServerPostReqException : public std::exception
{
	const char * what () const throw ()
	{
		return "invalid json data for post request";
	}
};
struct InvalidAccountIDException : public std::exception
	{
		const char * what () const throw ()
		{
			return "invalid account id";
		}
	};
class IRestServer {
	public:

		void get_handler_callback( std::shared_ptr< restbed::Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session)>  callback, const std::vector<std::string> & ids = {} );

		void post_handler_callcback( std::shared_ptr< restbed::Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session, Json::Value root)>  callback, const std::vector<std::string> & ids = {} );
		virtual void service_ready_handler( Service& ) = 0;
		virtual int start(std::vector<std::thread> &workers,int port ) = 0;
		/**
		 * Created  02/23/2022
		 * @brief   this function make sure to wrap the call back with all necessary try - catch blocks
		 * @param   
		 * @return  
		 */
		virtual Json::Value secondary_get_handler_callback( std::shared_ptr< restbed::Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session)>  callback, const std::vector<std::string> & ids = {} ) = 0;

		virtual Json::Value secondary_post_handler_callback( std::shared_ptr< restbed::Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session,  Json::Value root)>  callback, const std::vector<std::string> & ids = {} ) = 0;




};
#endif

