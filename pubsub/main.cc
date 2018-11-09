#include "module.h"
#include "subscriber.h"
#include <cstdlib>
#include <memory>

int
main(int argc, char** argv)
{
    // Create 4 threads:
    ModuleA* thread1 = new ModuleA();
    ModuleA* thread2 = new ModuleA();
    ModuleB* thread3 = new ModuleB();
    ModuleB* thread4 = new ModuleB();

    thread1->start();
    thread2->start();
    thread3->start();
    thread4->start();

    // Create message server, keeps lists of subscribers to different
    // topics
    MessageServer* msg_server = new MessageServer();

    // Send the handle of message server to every thread
    thread1->setServerHandle(msg_server);
    thread2->setServerHandle(msg_server);
    thread3->setServerHandle(msg_server);
    thread4->setServerHandle(msg_server);

    // Test: unsubscribe thread1 from all topics. The method of
    // unsubscribeOneThreadFromAllTopics() can be invoked before destroying a
    // thread, to clean up subscriber lists without pointing to dead threads
    msg_server->unsubscribeOneThreadFromAllTopics(thread1);

    // Waiting for 2 seconds
    usleep(1000000);

    // Subscribe these 4 threads to two topics:
    msg_server->subscribe(thread1, "Topic moduleA");
    msg_server->subscribe(thread2, "Topic moduleA");
    msg_server->subscribe(thread3, "Topic moduleB");
    msg_server->subscribe(thread4, "Topic moduleB");

    Message msg1("ModuleB");
    msg_server->publishMessage("Topic moduleA", msg1);

    // Waiting for 10 seconds
    usleep(10000000);
    // Unsubscribe thread4 from topic "Topic moduleB"
    msg_server->unsubscribe(thread4, "Topic moduleB");
    std::cout << "-------------------------------------------------------------"
    << std::endl;
    std::cout << "-----------unsubscribe(thread4, Topic_for_moduleB)-----------"
    << std::endl;
    std::cout << "-------------------------------------------------------------"
    << std::endl;

    // Waitting for 1 seconds
    usleep(1000000);
    // Unsubscribe thread2 from all topics
   msg_server->unsubscribeOneThreadFromAllTopics(thread2);

    while (true) {}
        std::cout << "done" << std::endl;

    delete thread1;
    delete thread2;
    delete thread3;
    delete msg_server;

    EXIT_SUCCESS;
}
