#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "message.h"


class Subscriber;

class Topic
{
 public:
    // Return the number of threads subscirbed to this topic
    int getNumSubscriberToThisTopic() { return subscribers.size(); }

    // Return a vector with all threads subscribed to this topic
    std::vector<Subscriber*> getSubscribers() { return subscribers; }

    // Get the name of the topic
    std::string getTopicName() { return topic_name; }

    // Set the name to the topic
    void setTopicName(std::string input_topic_name) {
        topic_name = input_topic_name; }

    // Add a thread to the subscribers vector
    void subscribeToThisTopic(Subscriber* s);

    // Use "Erase–remove idiom" to unsubsricbe a thread from this topic
    void unsubscribeFromThisTopic(Subscriber* s);
 private:
    std::string topic_name;
    // A vector contains pointers to the threads subscribing to this topic
    std::vector<Subscriber*> subscribers;
};


class MatchString
{
 public:
    explicit MatchString(std::string s) : s_(s) {}
    bool operator()(Topic& input_topic)
	{
        return input_topic.getTopicName() == s_;
	}
 private:
    std::string s_;
};


// Class MessageSever: maitains a vector of active topics, which in turn maitain
// vectors of subscribers to the according topic
class MessageServer
{
 public:
	MessageServer() {}
	virtual ~MessageServer() {}
    // Check whether the topic already exists in the vector of topics
    std::vector<Topic>::iterator checkTopicStatus(std::string input_topic_name);
    // Add a new topic to the vector of topics, then make s as a subscriber.
    void createTopic(Subscriber* s, std::string input_topic_name);
    // Get the number of topics
    int getNumTopics() { return topics.size(); }
    // Publish a message object to a given topic
    void publishMessage(std::string input_topic_name, Message msg);
    // Subscribe to a given topic for a given thread
    void subscribe(Subscriber* s, std::string input_topic_name);
    // Unsubscribe to a given topic for a given thread
    void unsubscribe(Subscriber* s, std::string input_topic_name);
    // Unsunscribe a given thread from all topics. This method can be used to
    // clean up the vectors of subscribers without pointing to dead threads
    void unsubscribeOneThreadFromAllTopics(Subscriber* s);

 private:
    // A vector of active topics, each of which maintain a vector of subscribers
    std::vector<Topic> topics;
};


// Class Subscriber: provide basic methods for message subscriber
class Subscriber
{
 public:
    Subscriber() { msg_server_handle = NULL; }
    virtual ~Subscriber() {}

    // Fetch the oldest message from message buffer, then remove the message
    Message fetchAndClearMessageBuffer();
    // Get the size of message buffer
    int getMessageBufferSize() { return message_buffer.size(); }
    // Get the pointer to the message server
    MessageServer* getServerHandle() { return msg_server_handle; }
    // Whether the message buffer is empty:
    // --- return ture if it is empty
    // --- return false if it is nonempty
    bool isMssageBufferEmpty() { return message_buffer.empty(); }
    // Message server will use this method to push new msg to the message buffer
    void pushToMessageBuffer(Message input_msg) {
        message_buffer.push_back(input_msg); }
    // Save the pointer of the message server
    void setServerHandle(MessageServer* msg_server) {
        msg_server_handle = msg_server; }

 private:
    // vector of Message objects
    std::vector<Message> message_buffer;
    MessageServer* msg_server_handle;
};

#endif
