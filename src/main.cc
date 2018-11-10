#include <module.h>
#include <subscriber.h>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <fstream>
#include <unistd.h>
#include <mutex>
#include <thread>
#include <mer.h>
#include <bub.h>

std::vector<int> int_store;
std::mutex store_lock;

void
push_int_store(int data)
{
	try {
		store_lock.lock();
		int_store.push_back(data);
		store_lock.unlock();
	}
	catch (std::exception e) {
		std::cerr << e.what() << std::endl;
	}
}

int
main(int argc, char** argv)
{
	const useconds_t ONE_SECOND = 10000000;
	std::string meth, line;
	std::vector<std::string> feeder;

	try {
		if(argc != 4)
			throw std::string("Usage: pusubsort <infile> <outfile> <mer|bub>");

		std::string meth(argv[3]);
		if((meth.find("bub") == std::string::npos) && meth.find("mer") == std::string::npos)
			throw std::string("Bad sort method, options are \'bub\' or \'mer\'");

		std::ifstream inputfile(argv[1]);
		std::ofstream outputfile(argv[2]);

		ModuleA* thread1 = new ModuleA();
		ModuleA* thread2 = new ModuleA();
		ModuleB* thread3 = new ModuleB();
		ModuleB* thread4 = new ModuleB();

		thread1->start();
		thread2->start();
		thread3->start();
		thread4->start();

		// Message server keeps lists of subscribers to different
		// topics
		MessageServer* msg_server = new MessageServer();

		// Send the handle of message server to every thread
		thread1->setServerHandle(msg_server);
		thread2->setServerHandle(msg_server);
		thread3->setServerHandle(msg_server);
		thread4->setServerHandle(msg_server);

		usleep(ONE_SECOND);

	    // Subscribe these 4 threads to two topics:
		msg_server->subscribe(thread1, "Topic moduleA");
		msg_server->subscribe(thread2, "Topic moduleA");
		msg_server->subscribe(thread3, "Topic moduleB");
		msg_server->subscribe(thread4, "Topic moduleB");

		// 1) read a line from the input file
		// 2) create msg object to hold the string
		// 3) ???
		// 4) PROFIT!
		while(std::getline(inputfile, line)) {

			std::istringstream iss(line);

			Message msg1("string to process");
		
			msg_server->publishMessage("Topic moduleA", msg1);

			usleep(ONE_SECOND);
		}
		
		if(thread1) delete thread1;
		if(thread2) delete thread2;
		if(thread3) delete thread3;

		if(thread4) delete thread4;
		delete msg_server;
		inputfile.close();

		outputfile.flush();
		outputfile.close();

		return EXIT_SUCCESS;
	}
	catch (std::string msg) {
		std::cerr << msg << std::endl;
	}		
	return EXIT_FAILURE;
}
