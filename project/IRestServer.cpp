/*
 * =====================================================================================
 *
 *       Filename:  IRestServer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/23/2022 09:52:00 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */
#include "IRestServer.h"
#include <iostream>
#include <memory>
using namespace std;
using namespace std::string_literals;


using namespace restbed;

void IRestServer::get_handler_callback( std::shared_ptr< Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session)>  callback, const std::vector<std::string> & ids  )
{
	const auto& request = session->get_request( );




	Json::Value root;
	try  {
		bool ok = true;
		if (ids.size()) {
			ok = false;
			for (auto & id : ids) {
				ok =  request->has_query_parameter(id);
				if (ok)
					break;
			}
		}

		if (ok)  {
			root = secondary_get_handler_callback (session, callback, ids);
			//	root = callback (session);
		} else{
			root["error-msg"]="missing command parameters ";
			root["error"] = -2;
		}
	}  catch (const std::invalid_argument e) {
		std::cout << e.what() << "\n";
		root["error-msg"]=e.what();
		root["error"] = -13;
	} catch (const std::out_of_range e) {
		root["error-msg"]=e.what();
		root["error"] = -14;
	} catch (Json::LogicError e) {
		std::cout << e.what() << "\n";
		root["error-msg"]=e.what();
		root["error"] = -19;
	}




	std::ostringstream message;
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = ""; //The JSON document is written in a single line
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root,&message );


	/* find length */
	std::stringstream s; 
	s << strlen (message.str().c_str());
	/* send and close */
	session->close( OK, message.str().c_str(), { { "Content-Length",  s.str().c_str() }, { "Connection", "close" } } );
}

void IRestServer::post_handler_callcback( std::shared_ptr< Session > session ,  std::function<Json::Value(std::shared_ptr< restbed::Session > session, Json::Value root)>  callback, const std::vector<std::string> & ids )
{
	const auto request = session->get_request( );

	int content_length = request->get_header( "Content-Length", 0 );
	session->fetch( content_length, [content_length, &callback]( const std::shared_ptr< Session > session, const Bytes & body ) {
		Json::Value root;
		Json::CharReaderBuilder builder;
		std::string errors;
		Json::CharReader * reader = builder.newCharReader();
		bool parseSuccess = reader->parse( (char*)body.data(), (char*)body.data() + body.size(), &root, &errors);
		delete reader;
		if (!parseSuccess)
		{
			throw IRestServerPostReqException();
		}
		try {
			root = callback (session,  root);

		} catch (const std::invalid_argument  e) {
			std::cout << e.what() << "\n";
			root["error-msg"]=e.what();
			root["error"] = -3;
		} catch (const std::out_of_range  e) {
			root["error-msg"]=e.what();
			root["error"] = -4;
		} catch (Json::LogicError e) {
			std::cout << e.what() << "\n";
			root["error-msg"]=e.what();
			root["error"] = -9;
		}



		std::ostringstream message;
		Json::StreamWriterBuilder wbuilder;
		wbuilder["commentStyle"] = "None";
		wbuilder["indentation"] = ""; //The JSON document is written in a single line
		std::unique_ptr<Json::StreamWriter> writer(wbuilder.newStreamWriter());
		writer->write(root,&message );
		session->close( OK, message.str().c_str(), { { "Content-Length",std::to_string(message.str().length())   }, { "Connection", "close" } } );

	} );

}


