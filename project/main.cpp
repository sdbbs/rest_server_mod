
#include <thread>
#include <memory>
#include <chrono>
#include <cstdlib>
#include <restbed>

using namespace std;
using namespace restbed;

void get_method_handler( const shared_ptr< Session > session )
{
    session->close( OK, "Hello, World!", { { "Content-Length", "13" }, { "Connection", "close" } } );
}

void service_ready_handler( Service& )
{
    fprintf( stderr, "Hey! The service is up and running." );
}

int main( const int, const char** )
{
    auto resource = make_shared< Resource >( );
    resource->set_path( "/resource" );
    resource->set_method_handler( "GET", get_method_handler );

    auto settings = make_shared< Settings >( );
    settings->set_port( 1984 );

    auto service = make_shared< Service >( );
    service->publish( resource );
    service->set_ready_handler( service_ready_handler );
    service->start( settings );

    return EXIT_SUCCESS;
}


#if OLDCODE0
#include <iostream>
#include <unistd.h>
#include <thread>
#include <memory>
#include <chrono>
#include <cstdlib>
#include <json/json.h>
#include <signal.h>
#include "DataServerResetAPI.h"
#include "git.h"




int main(int argc, char *argv[])
{
	std::vector<std::thread> workers;
	cout<<argv[0]<<",Rest Server Demo,  Yair Gadelov\n  git branch: "<<GIT_BRANCH<<endl<<"  git sha: "<<GIT_DESCRIBE<<endl<<"  build time: "<<GIT_COMMIT_DATE_ISO8601<<endl;

	DataRestServer restserver;
	restserver.start(workers,1984);


	for (auto &t : workers)
		t.join();
	return 0;
}
#endif // OLDCODE0




