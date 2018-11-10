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
    ModuleA() { server_exists = false; }
    void* run();

private:
    bool server_exists;
};

class ModuleB : public Module, public Subscriber
{
public:
    ModuleB() { server_exists = false; }
    void* run();

private:
    bool server_exists;
};

#endif
