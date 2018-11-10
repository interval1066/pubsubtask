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
#include <algos.h>

std::vector<int> int_store;
std::mutex store_lock;

void
push_int_store(int data)
{
	try {
		std::lock_guard<std::mutex> lock(store_lock);
		int_store.push_back(data);
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
		ModuleB* thread2 = new ModuleB();

		thread1->start();
		thread2->start();

		// Message server keeps lists of subscribers to different
		// topics
		MessageServer* msg_server = new MessageServer();

		// Send the handle of message server to every thread
		thread1->setServerHandle(msg_server);
		thread2->setServerHandle(msg_server);

		usleep(ONE_SECOND);

		// Subscribe these 4 threads to two topics: two readers
		// and two writers
		msg_server->subscribe(thread1, "Topic moduleA");
		msg_server->subscribe(thread2, "Topic moduleB");

		// 1) read a line from the input file
		// 2) create msg object to hold the string
		// 3) add the msg to the msg_server
		// 3) ???
		// 4) PROFIT!
		while(std::getline(inputfile, line)) {
			Message msg(line);
			msg_server->publishMessage("Topic moduleA", msg);

			usleep(ONE_SECOND);
			std::getline(inputfile, line);
			Message msg2(line);
			
			msg_server->publishMessage("Topic moduleB", msg2);
			usleep(ONE_SECOND);
		}
		
		if(thread1) delete thread1;
		if(thread2) delete thread2;
		delete msg_server;

		inputfile.close();
		outputfile.flush();
		outputfile.close();

		return EXIT_SUCCESS;
	}
	// If we get here something is wrong. Don't try to clean up,
	// hopefully RAII is working here. Just report problem to user.
	catch (std::string msg) {
		std::cerr << msg << std::endl;
	}
	return EXIT_FAILURE;
}
