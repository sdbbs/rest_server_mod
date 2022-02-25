/*
 * =====================================================================================
 *
 *       Filename:  dbmanage.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/23/19 07:22:36
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "dbmanage.h"
//#include "config.h"

#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include <iomanip>      // std::stringstream
//#include "cpptoml.h"
//#include "Order2Book.h"
#include "common/log4algo.h"

using namespace std;
#if 0
bool PSQLManage::login()
{
	if (m_postgressConnection.is_open()) {
		std::cout << "Opened database successfully: " << m_postgressConnection.dbname() << std::endl;
		return true;
	} else {
		std::cout << "Can't open database" << std::endl;
		return false;
	}
}

#endif
#ifdef DEPRECTED_THIS
/**
 * Created  09/18/2019
 * @brief   fills ContractInfo class which placed in instrumenttable class
 * @param   
 * @return  
 */
bool PSQLManage::ExtractContractsData(std::map <int,std::tuple<ContractInfo , Order2Book>> &Contracts,int accountID )
{
	/* Reads symbols table */

	std::stringstream sql;
	sql<< "select symbols.id,symbol,sectype,currency,exchange  from symbols  join brokers on brokers.id=symbols.brokerID where accountid='"<<accountID<<"'";
	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( sql.str() ));

		/* List down all the records */
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			ContractInfo contract(c[0].as<int>()  , c[1].as<string>().c_str(),  c[2].as<string>().c_str(), c[3].as<string>().c_str(), c[4].as<string>().c_str()  );
			std::get <ContractInfo> (Contracts[c[0].as<int>()]) = contract;
		}

		cout << "Operation done successfully" << endl;

		return true;
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return false;
	}

}
#endif

#if 0
/**
 * Created  10/21/2021
 * @brief   returns json accoring to query
 * @param   
 * @return  
 */
std::string PSQLManage::selectQuery(std::string sql, const std::vector<string> & f={}) 

{
	Json::Value root;
	
	/* Reads symbols table */

	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R(N.exec(sql));

		/* List down all the records */
		int idx = 0;
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			for (int j = 0; j < c.size(); j++) {
				 std:string name;
				if (j < f.size() )
					name = f[j];
				else
					name = R.column_name(j);	
				root[idx][name] = c[j].as<string>();
			}
			idx++;
		}


	} catch (const std::exception &e) {
		root["error"] = e.what();
		cerr << e.what() << std::endl;

		
	}

	std::ostringstream message;
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = ""; //The JSON document is written in a single line
	std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	writer->write(root,&message );


	return message.str();
}
#endif



#if 0

/**
 * Created  09/17/2019
 * @brief   reads broker name by accoutn id run the this query: 
 *              "select brokerid,name from accounts join brokers on accounts.brokerID = brokers.id where accountid=1;"
 *              this  is the returned table: "brokerid | name"
 * @param   
 * @return  
 */
bool PSQLManage::ReadBrokerID(int accountID, std::string &brokerName,int &brokerID ) 
{
	//int brokerID=-1;
	/* Reads symbols table */
	std::stringstream sql;
	sql << "select brokerid,name from accounts join brokers on accounts.brokerID = brokers.id where accountid='"<<accountID<<"'";
	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* List down all the records */
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			brokerID = c[0].as<int>();
			brokerName = c[1].as<string>();

		}
		//  cout << "Operation done successfully" << endl;

	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return false;
	}
	return true;

}
#endif
#ifdef DERECATED_THIS
bool PSQLManage::InitTickDataTables(int accountID)  
{
	std::stringstream sql;
	sql<<"select create_tick_tables('"<< accountID <<"')";
	cout<<sql.str()<<endl;
	try {
		/* Create a transactional object. */
		work W(m_postgressConnection);

		/* Execute SQL query */
		W.exec( sql );
		W.commit();

	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return false;
	}
	return true;

}
#endif

#if 0
//bool PSQLManage::InsertBidAsk(struct tickdata &tickdata , std::string brokerName,std::string exchange , std::string symbol ,std::string currency,int account_id )
bool PSQLManage::InsertBidAsk(struct tickdata &tickdata , std::string symbol, int account_id )

{
	std::stringstream sql;       
	std::string s;
	s.append("accout_");
	s.append(std::to_string(account_id));
	s.append(".");
	s.append(symbol);



#if 0
	s.append(brokerName);
	s.append("_");
	s.append(exchange);
	s.append("_");
	s.append(symbol);
//	s.append("_");
//	s.append(currency);
	s.append("_");
	s.append(std::to_string(account_id));
#endif
	sql<<" insert into "<<s<<"(createdate,bid,ask) values ("
		<<"to_timestamp("<<std::setprecision(6)<<std::fixed<<(double)(tickdata.time/1000000.0)<<")" 
		<<","<< std::setprecision(6)<<std::fixed<<tickdata.bids[0].price
		<<","<< std::setprecision(6)<<std::fixed<<tickdata.asks[0].price<<")"; 
	//	std::cout<<sql.str()<<endl;

	//printf ("%s\n",sql.str().c_str());

	//	log4algo_print (LOG_INFO,"%s",sql.str().c_str());

	log4algo_to_udp (134,(char *)sql.str().c_str());
//	printf ("%s\n",ss);
//	return true;
	try {

		/* Create a transactional object. */
		work W(m_postgressConnection);

		/* Execute SQL query */
		W.exec( sql );
		W.commit();
		return true;
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return false;
	}

}
#endif
#if 0
bool PSQLManage::InsertQuery(std::string sql) {
	work W(m_postgressConnection);
	try {

		W.exec( sql );
		W.commit();
	} catch (const std::exception &e) {
		W.abort();
		cerr << e.what() << std::endl;
		return false;
	}
	return true;
}
#endif
#if 0
/**
 * Created  08/12/2019
 * @brief   adds new strategy  to table strategy. The just created strategyID is retured in strategy object.
 * @param   strategy: conatins info about the strategy. the function add new column to  strategy_instances 
 * @return  
 */
bool PSQLManage::createStrategy(Strategy &strategy )
{
	work W(m_postgressConnection);
	try {
		//	create new instace in database
		//sql.str("");
		//sql.clear();
		std::stringstream sql;
		sql<<"insert into strategies (name,filename,description) values ("<<strategy.name.c_str()<<","<<strategy.filename.c_str()<<","<<strategy.description.c_str()<<")";
		W.exec( sql );
		W.commit();
	} catch (const std::exception &e) {
		W.abort();

		cerr << e.what() << std::endl;
		return false;
	}

	/* find the newest ID which was just created and place it in strategy.strategyID*/  
	try {
		std::stringstream sql ;
		sql <<"select  id from strategies ORDER BY ID DESC LIMIT 1";
		nontransaction N( m_postgressConnection);
		result R( N.exec( sql ));
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			strategy.strategyid = c[0].as<int>();
		}
		N.commit();
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return false;
	}
	return true;
}

#endif

#if 0

/**
 * Created  08/12/2019
 * @brief   insert aram value + name to strategy_params. @se table strategy_params
 * @param   
 * @return  
 */
bool PSQLManage::UpdateStrategyParams(int strategyid,int instanceID,double value,char*name)
{
	work W1(m_postgressConnection);
	try {
		/* add parameter to insance */ 
		std::stringstream  sql;
		// query call example: select update_strategy_params(1,100,1234,'long_ma');
		sql<<"select update_strategy_params("<<strategyid<<","<< instanceID<<","<< value <<"," << "\'"<<name<<"\');";
		W1.exec( sql );
		W1.commit();
	} catch (const std::exception &e) {
		W1.abort();
		cerr << e.what() << std::endl;
		return false;
	}
	return true;
}

bool PSQLManage::ReadParamValueFromDB(int strategyid,int instanceID,std::map<string,double> &params)
{
	try {
		/* find the newest ID which was just created*/
		std::stringstream sql ;
		sql<< "select name,value from strategy_params where instanceid="<<instanceID<<" and strategyid="<<strategyid;
		nontransaction N( m_postgressConnection);
		result R( N.exec( sql ));
		for (auto const & c : R ) {
//		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			params[c[0].as<string>()] = c[1].as<double>();
		}
		N.commit();
		return false;
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return true;
	}

}
#endif
#if 0
bool PSQLRead::RunQuery(	std::stringstream &sql){
	try {

		/* Create a non-transactional object. */
		N =  std::make_unique <nontransaction> ( m_postgressConnection);
		/* Execute SQL query */
		R =std::make_unique< result> ( N->exec( sql ));

		/* get first row */
		c = R->begin();

		if (c == R->end() )
			return false;
		return true;
	}catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return false;
	}
	return true;
}

bool PSQLRead::NextRow(	){
	++c;  
	if ( c == R->end() )
		return false;
	return true;
}
bool PSQLRead::EndQuery(	){
	N->commit();
	return true;
}
#endif
#if 0 

/**
 * Created  08/12/2019
 * @brief   call sql function "read_tick_data" to get tickdata array
 * @param   
 * @return  
 */
struct tickdata *  PSQLManage::ReadTickData(std::vector<int>  symbol_id, int *len)
{
	struct tickdata *tickdata  = 0 ;
	int tick_data_size = 0;
	try {
		/* get size of table */ 
		std::stringstream  sql;
		sql <<"select count(symbolid) from read_tick_data(ARRAY [";
		for (auto  & sym : symbol_id )  {
			if (&sym != &symbol_id.back())
				sql<<sym<<",";
			else
				sql<<sym;
		}
		sql<<"])";
		nontransaction N( m_postgressConnection);
		result R( N.exec( sql ));
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			tick_data_size = c[0].as<int>();
		}
		// this is not C++!!! - this array uses as input to pure c function that runs in the dll 
		tickdata = (struct tickdata *) malloc(sizeof( struct tickdata ) * tick_data_size);

		N.commit();


	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return 0;
	}



	try {
		int i=0;
		/* get size of table */ 
		std::stringstream  sql;
		sql <<"select * from read_tick_data(ARRAY [";
		for (auto  & sym : symbol_id )  {
			if (&sym != &symbol_id.back())
				sql<<sym<<",";
			else
				sql<<sym;
		}
		sql<<"])";
		nontransaction N( m_postgressConnection);
		result R( N.exec( sql ));
		memset (tickdata,0,sizeof (struct tickdata)*tick_data_size);
		for ( result::const_iterator c = R.begin(); c != R.end(); ++i, ++c) {

		//	if (static_cast<bool>(c[0].size()) )
		//		tickdata[i].brokerID = c[0].as<int >();
			if (static_cast<bool>(c[1].size()) )
				tickdata[i].symbolID = c[1].as<int >();
			if (static_cast<bool>(c[2].size()) )     
				tickdata[i].bids[0].price = c[2].as<double>();
			if (static_cast<bool>(c[3].size()) )     
				tickdata[i].asks[0].price = c[3].as<double>();
			if (static_cast<bool>(c[4].size()) )     
				tickdata[i].bids[0].size = c[4].as<int >();
			if (static_cast<bool>(c[5].size()) )     
				tickdata[i].asks[0].size = c[5].as<int >();
			if (static_cast<bool>(c[6].size()) )     
				tickdata[i].time = c[6].as<double >()*1000;
		}

		N.commit();
		*len = i;

	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return 0;
	}




	return tickdata;
}
#endif




#if 0
/**
 * Created  09/19/2019
 * @brief   this function if a factory  to Stratgey class.  The class is build on based parameters which taken from database
 * @param   
 * @return  
 */
unique_ptr<StrategyInfo>  PSQLManage::getStrategy(int strategyID,int instanceID) 
{	
	std::stringstream sql,sofilename ;
	std::string  path;

	StrategyInfo strategy;
	//int ok = 0;
	sql <<"select  id,name,filename, description from strategies  where id="<<strategyID;

	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* should be only one record according to stragegy ID*/
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
			strategy.strategyid =  c[0].as<int>();
			strategy.instanceID = instanceID;
			strategy.name = c[1].as<string>();
			strategy.filename = c[2].as<string>();
			strategy.description = c[3].as<string>();
		//	sofilename<<path<<"/"<<c[2].as<string>();
		//	ok =  strategy.Load(sofilename.str() ) ;

		}
		N.commit();

//		if (ok<0)
//			goto err_here;

#if 0
		/* get parameter from startegy instance parameters  and copy it to to stratgegy*/
		std::map<string,double> params;
		ReadParamValueFromDB( strategyID, instanceID,params);
		for( auto const& [key, val] : params )
		{
			strategy.set_param_by_name (key.c_str(),val);
		}
#endif
//		strategy.Init();

		//  cout << "Operation done successfully" << endl;
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		goto err_here;

	}


	return make_unique< StrategyInfo> (strategy);
err_here:
	return  unique_ptr< StrategyInfo> (nullptr);

}
#endif


#if 0
/**
 * Created  09/09/19
 * @brief   returns active strategies list on a specfifc strategy server
 * @param   serverID: id if strategy server
 * @return  
 */
bool PSQLManage::getActiveStrategies(int accountid,std::vector<std::pair<int,int>> &strategy_info) 
{
	std::stringstream sql,sofilename ;
	sql <<"select  strategyid,instanceID from strategies join  active_strategies  on strategies.id =  active_strategies.strategyID where accountid="<<accountid;


	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* should be only one record according to stragegy ID*/
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) 
			strategy_info.push_back (std::pair<int,int> (c[0].as<int>(), c[1].as<int>() ) );

		N.commit();
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		return false;
	}
	return true;

}
#endif


#if 0
/**
 * Created  09/18/2019
 * @brief   returns vecort of tupples. ecah tupple holds pair of (accountID,brokerID). The runnuing quiery is:
 *            production=> select brokerid,accountid,baseurl,name from accounts join brokers on brokers.id= accounts.brokerid;
 *		 brokerid | accountid |             baseurl              |   name   
 *		----------+-----------+----------------------------------+----------
 *			1 |         1 |                                  | IB
 *			2 |         2 |                                  | FXCM
 *			3 |         3 | https://www.bitstamp.net/api/v2/ | BITSTAMP           
 * @param   filename nae of dataserver.conf file
 * @return  
 */
accountTypeDef PSQLManage::getAccountList(std::string filename  ) 
{
	std::stringstream sql,sofilename ;
	accountTypeDef accounts ;
	sql <<"select brokerid,accountid,baseurl,name from accounts join brokers on brokers.id= accounts.brokerid where dataserverID=1 and  ";
	auto config = cpptoml::parse_file(filename);
	auto tarr = config->get_table_array("active-accounts");
	if (tarr) {
		for (const auto& table : *tarr)
		{
			// *table is a cpptoml::table
			auto id = table->get_as<int>("id");
			if (id)
				sql<<" brokers.id="<<*id<<" or";
		}
	} else {
		std::cout<<"warning - not any account was defined at /etc/dataserver.conf"<<std::endl;
		return accounts;
	}

	string s = sql.str();
	s=s.substr (0,s.size()-2);


	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( s ));

		/* should be only one record according to stragegy ID*/
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) 
			accounts.push_back (std::make_tuple<int,int,string,string> (  static_cast<bool>(c[0].size()) ?    c[0].as<int>() : -1 ,
						static_cast<bool>(c[1].size()) ?    c[1].as<int>() : -1 ,
						static_cast<bool>(c[2].size()) ?    c[2].as<string>() : "" ,
						static_cast<bool>(c[3].size()) ?    c[3].as<string>() : "" ) );

		N.commit();
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		//  return false;
	}
	return accounts;


}
#endif
#if 1
#if 0
//THIS_IS_THE_RIGHT_FORM_OF_THE_FUNTION getAccountList
//select brokerid,accountid,baseurl,name from accounts join brokers on brokers.id= accounts.brokerid where dataserverID=1;
//
accountTypeDef PSQLManage::getAccountList() //int dataserverID  ) 
{

	std::stringstream sql ;
//	sql <<"select brokerid,accountid,baseurl,name from accounts join brokers on brokers.id= accounts.brokerid where dataserverID="<<dataserverID;
	sql <<"select brokerid,accountid,baseurl,name from accounts join brokers on brokers.id= accounts.brokerid";
	accountTypeDef accounts ;

	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* should be only one record according to stragegy ID*/
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) 
			accounts.push_back (std::make_tuple<int,int,string,string> (  static_cast<bool>(c[0].size()) ?    c[0].as<int>() : -1 ,
						static_cast<bool>(c[1].size()) ?    c[1].as<int>() : -1 ,
						static_cast<bool>(c[2].size()) ?    c[2].as<string>() : "" ,
						static_cast<bool>(c[3].size()) ?    c[3].as<string>() : "" ) );

		N.commit();
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		//  return false;
	}
	return accounts;


}
#endif
#endif



//using fxcmTradeDef =  std::tuple<std::string,std::string,std::string,int,int> ; 

	


#if 0
fxcmTradesListDef PSQLManage::getFxcmTrades(std::string accountid,int & err) {
	std::stringstream sql ;
	fxcmTradesListDef trades ;

//	sql <<"select OpenOrderID,InstrumentName,OfferID,lotSize,openContracts,tradeId,extract(epoch from created_at),created_at,rate from account_"<<accountid<<".fxcm_trades where openContracts <> 0 ";

	sql <<"select openorderid,instrumentname,offerid,lotsize,opencontracts,tradeid,extract(epoch from created_at),created_at,rate from (select *, row_number() over (partition by tradeId order by abs(openContracts)) as rn from account_"<<accountid<<".fxcm_trades) t where rn = 1 and opencontracts<>0;";

	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* should be only one record according to stragegy ID*/
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) 
			trades.push_back (std::make_tuple<std::string,std::string,std::string,int,int,std::string,double,std::string,double> (  
						static_cast<bool>(c[0].size()) ?    c[0].as<string>() : "" ,
						static_cast<bool>(c[1].size()) ?    c[1].as<string>() : "" ,
						static_cast<bool>(c[2].size()) ?    c[2].as<string>() : "" ,
						static_cast<bool>(c[3].size()) ?    c[3].as<int>() : -1,
						static_cast<bool>(c[4].size()) ?    c[4].as<int>() : -1,
						static_cast<bool>(c[5].size()) ?    c[5].as<string>() : "", 
						static_cast<bool>(c[6].size()) ?    c[6].as<double>() : -1,
						static_cast<bool>(c[7].size()) ?    c[7].as<string>() : "", 
						static_cast<bool>(c[8].size()) ?    c[8].as<double>() : -1
						)
					);

		N.commit();
		err = 0;
	} catch (const std::exception &e) {
		err = -1;
		cerr << e.what() << std::endl;
		//  return false;
	}
	return trades;
}
#endif

#if 0
/**
 * Created  09/18/2019
 * @brief   returns fxcmLoginPrm with fxcm account details
 *       id |  username  | password |                 host                 | type | accountid 
 *       ----+------------+----------+--------------------------------------+------+-----------
 *        1 | D161437087 | 4400     | http://www.fxcorporate.com/Hosts.jsp | Demo |         2
 * 
 * @param   accountID - system internal accountID (not refer to FXCM or any other broker)
 * @return  
 */
unique_ptr<FxcmLoginPrm>  PSQLManage::getFxcmLoginPrm( int accountID) 
{
	std::stringstream sql,sofilename ;
	// select * from accounts join accounts_fxcm on accounts.accountid = accounts_fxcm.accountid  join brokers on brokers.id = accounts.brokerid
	sql <<"select username,password,host,type from accounts_fxcm where accountid="<<accountID;
	unique_ptr<FxcmLoginPrm>  fxcmLoginPrm  (nullptr);
	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* should be only one record according to stragegy ID*/
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) 
			fxcmLoginPrm = make_unique<FxcmLoginPrm>(c[0].as<string>(), c[1].as<string>(),c[2].as<string>(),c[3].as<string>());

		N.commit();
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		//  return false;
	}
	return fxcmLoginPrm;


}
#endif

#if 0
/**
 * Created  01/13/2021
 * @brief   retrive accout psrametrs of bitstamp
 * production=> select * from accounts_bitstamp;
 *   id | username |               key                |              secret              | accountid 
 *   ----+----------+----------------------------------+----------------------------------+-----------
 *    1 | 34387    | hqI0xFMWEvfDZPaUK2wWi2H8IW3VAi0x | gNCmlXT5BE6Bm2S1X6H8Nd7R1Fwn3T1U |         3
 *  (1 row)
 *
 * @param   
 * @return  
 */
unique_ptr<BitstampLoginPrm>  PSQLManage::getBitstampLoginPrm( int accountID) 
{
	std::stringstream sql,sofilename ;
	sql <<"select * from accounts_bitstamp where accountid="<<accountID;
	unique_ptr<BitstampLoginPrm>  bitstampLoginPrm  (nullptr);
	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* should be only one record according to stragegy ID*/
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) 
			bitstampLoginPrm = make_unique<BitstampLoginPrm>(c[0].as<string>(), c[1].as<string>(),c[2].as<string>(),c[3].as<string>());

		N.commit();
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		//  return false;
	}
	return bitstampLoginPrm;


}


#endif

#if 0
/**
 * Created  09/18/2019
 * @brief   returns bitstampLoginPrm  with bitstamp account details
 *	production=> select * from accounts_bitstamp;
 *	 id | username |               key                |              secret              | accountid 
 *	----+----------+----------------------------------+----------------------------------+-----------
 *	  1 | 34387    | hqI0xFMWEvfDZPaUK2wWi2H8IW3VAi0x | gNCmlXT5BE6Bm2S1X6H8Nd7R1Fwn3T1U |         3
 *
 * 
 * @param   accountID - system internal accountID (not refer to FXCM or any other broker)
 * @return  
 */
unique_ptr<BitstampLoginPrm>  PSQLManage::getBitstampLoginPrm( int accountID) 
{
	std::stringstream sql,sofilename ;
	sql <<"select username,key,secret from accounts_bitstamp where accountID ="<<accountID;
	unique_ptr<BitstampLoginPrm>  bitstampLoginPrm  (nullptr);
	try {
		/* Create a non-transactional object. */
		nontransaction N( m_postgressConnection);

		/* Execute SQL query */
		result R( N.exec( sql ));

		/* should be only one record according to stragegy ID*/
		for (result::const_iterator c = R.begin(); c != R.end(); ++c) 
			bitstampLoginPrm = make_unique<BitstampLoginPrm>(c[0].as<string>(), c[1].as<string>(),c[2].as<string>() );

		N.commit();
	} catch (const std::exception &e) {
		cerr << e.what() << std::endl;
		//  return false;
	}
	return bitstampLoginPrm;


}
#endif
#if 0

/**
 * Created  10/28/2019
 * @brief   update insertion time of symbol at table: last_ticktime. The following query is done:
 * 					select  update_last_ticktime(2);
 *					 
 * 				  this one will use to read the update time in other part of the system:
 * 				  select brokerid,symbol,currency,exchange, updated_at ,created_at from last_ticktime join symbols on  last_ticktime.symbolid = symbols.id where brokerid=2;
 * @param   symbolID the id of the last inserted symbol
 * @return  
 */
bool PSQLManage::InsertLastUpdateTime(int symbolID)
{
	work W1(m_postgressConnection);
	try {
		/* add parameter to insance */ 
		std::stringstream  sql;
		// query call example: select update_strategy_params(1,100,1234,'long_ma');
		sql <<"select  update_last_ticktime(" <<symbolID<<")";
		W1.exec( sql );
		W1.commit();
	} catch (const std::exception &e) {
		W1.abort();
		cerr << e.what() << std::endl;
		return false;
	}
	return true;

}
#endif
