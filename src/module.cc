#include <string>
#include <module.h>


static void*
runModule(void* arg)
{
    return (reinterpret_cast<Module*>(arg))->run();
}

Module::Module() : m_tid(0), m_running(0), m_detached(0) {}


Module::~Module()
{
    if (m_running == 1 && m_detached == 0) { pthread_detach(m_tid); }
    if (m_running == 1) { pthread_cancel(m_tid); }
}

int
Module::detach()
{
    int result = -1;
    if (m_running == 1 && m_detached == 0) {
        result = pthread_detach(m_tid);
        if (result == 0) { m_detached = 1; }
    }
    return result;
}

int
Module::join()
{
    int result = -1;

    if (m_running == 1) {
        // pthread_join: wait for thread termination
        result = pthread_join(m_tid, NULL);
        if (result == 0) { m_detached = 0; }
    }
    return result;
}


// Return ID
pthread_t
Module::self() { return m_tid; }

// Start the thread
int Module::start() {
    // Create a thread and get a thread ID
    int result = pthread_create(&m_tid, NULL, runModule, this);
    if (result == 0) { m_running = 1; }
    return result;
}


// Every thread instance of Module-A will run the for-loop continuesly
void*
ModuleA::run()
{
    for (int i = 0;; i++) {
        std::cout << "thread " << self() << ", loop " << i << std::endl;

        // Waitting for 1 second
        usleep(1000000);
        // Get the handle of message server
        MessageServer* temp_msg_server_handle = getServerHandle();

        if (!isMssageBufferEmpty()) {
            // Fetch a msg from msg buffer, then remove the msg from the buffer
            Message temp_msg = fetchAndClearMessageBuffer();
            // Get the message content
            std::string temp_msg_content = temp_msg.getMessageContent();

            std::cout << "thread " << self() << ", reciever msg --- "
            << temp_msg_content << std::endl;

            // Create a message
            Message msg_to_ModuleB("A_welcome_from_ModuleA");
            // Publish the message to topic "Topic_for_moduleB"
            temp_msg_server_handle->publishMessage("Topic_for_moduleB",
            msg_to_ModuleB);
        }
		else
            // Waiting message
            std::cout << "thread " << self() << " with empty MessageBuffer"
            << std::endl;
    }
    return nullptr;
}

void*
ModuleB::run()
{
    for (int i = 0;; i++) {
        std::cout << "thread " << self() << ", loop " << i << std::endl;

        // Waitting for 2 seconds
        usleep(2000000);
        // Get the handle of message server
        MessageServer* temp_msg_server_handle = getServerHandle();

        if (!isMssageBufferEmpty()) {
            // Fetch a msg from msg buffer, then remove
            Message temp_msg = fetchAndClearMessageBuffer();
            // Get the message
            std::string temp_msg_content = temp_msg.getMessageContent();

            std::cout << "thread " << self() << ", reciever msg --- "
            << temp_msg_content << std::endl;

            // Create a message
            Message msg_to_ModuleA("A_welcome_from_ModuleB");
            // Publish Message to topic "Topic_for_moduleA"
            temp_msg_server_handle->publishMessage("Topic_for_moduleA",
            msg_to_ModuleA);
        }
		else
            // Waiting for new message
            std::cout << "thread " << self() << " with empty MessageBuffer" << std::endl;
    }
    return nullptr;
}
