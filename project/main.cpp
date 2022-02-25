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





