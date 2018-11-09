#ifndef MODULE_H_
#define MODULE_H_

#include <pthread.h>
#include <unistd.h>
#include <subscriber.h>

class Module
{
public:
	Module();
	virtual ~Module();
	int detach();

	int join();
	virtual void* run() = 0;
	pthread_t self();

	int start();

private:
	int m_detached;
	int m_running;
	pthread_t m_tid;
};

class ModuleA : public Module, public Subscriber
{
public:
    ModuleA() { is_message_server_exist = false; }
    void* run();

private:
    bool is_message_server_exist;
};

class ModuleB : public Module, public Subscriber
{
public:
    ModuleB() { is_message_server_exist = false; }
    void* run();

private:
    bool is_message_server_exist;
};

#endif
