#include <iostream>
//#include "fxcm/ResponseListener.h"
//#include "fxcm/SessionStatusListener.h"
//#include "InstrumentsTable.h"
#include <unistd.h>
//#include "CmdServer.h"
//#include "AccountMng.h"
//#include "fxcm/FxcmConnect.h"
//#include "BrokerLoginPrm.h"
#include "common/version.h"
#include "config_vars.h"
#include "common/CurlFetch.h"
#include "json/json.h"
#include "common/log4algo.h"
#include "DBConnectionParams.h"

using namespace std;

#include "DataServerResetAPI.h"
#include "common/log4algo.h"



#include <thread>
#include <memory>
#include <chrono>
#include <cstdlib>



#include <json/json.h>
#include <signal.h>
#include "git.h"



void intHandler(int dummy) {
	cout<<"sig hadnler"<<endl;
	log4algo_print (LOG_INFO,"ctrl+c exit");

}

//psql -d production  -U yair -h www.appsite.co.il
//sudo nc -q0  127.0.0.1 514 -u -k -l -vv
int main(int argc, char *argv[])
{
	std::vector<std::thread> workers;
	cout<<argv[0]<<",AllRights to Yair Gadelov\n  git branch: "<<GIT_BRANCH<<endl<<"  git sha: "<<GIT_DESCRIBE<<endl<<"  build time: "<<GIT_COMMIT_DATE_ISO8601<<endl;

	log4algo_init(0,LOG_LOCAL0);
	setvbuf ( stdout , 0 , _IONBF , 0 );
	log4algo_print (LOG_NOTICE,"fxcm server was started - git sha %s, build time %s",GIT_DESCRIBE,GIT_COMMIT_DATE_ISO8601);

	log4algo_print (LOG_CRIT,"fxcm server was started - git sha %s, build time %s",GIT_DESCRIBE,GIT_COMMIT_DATE_ISO8601);

	log4algo_print (LOG_INFO,"fxcm server was started - git sha %s, build time %s",GIT_DESCRIBE,GIT_COMMIT_DATE_ISO8601);

	signal(SIGPIPE, intHandler);



	DataRestServer restserver;
	restserver.start(workers,1984);


	for (auto &t : workers)
		t.join();
	return 0;
}





