#include <subscriber.h>

void
Topic::subscribeToThisTopic(Subscriber* s)
{
	if (getNumSubscriberToThisTopic() == 0) {
		subscribers.push_back(s);
		std::cout << "Add new subscriber, now we have "
			<< getNumSubscriberToThisTopic() << " subscribers to topic "
			<< getTopicName() << std::endl;
	}
	else {
		std::vector<Subscriber*>::iterator it = std::find(subscribers.begin(),
			subscribers.end(), s);
		if (*it != s) {
			subscribers.push_back(s);

			std::cout << "Add new subscriber, now we have "
				<< getNumSubscriberToThisTopic() << " subscribers to topic "
				<< getTopicName() << std::endl;
		}
		else
			std::cout << "The thread already subscribed to topic: "
				<< getTopicName() << std::endl;
	}
}

void
Topic::unsubscribeFromThisTopic(Subscriber* s)
{
	subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), s),
		subscribers.end());
	std::cout << "Unsubscribe from topic: " << getTopicName() << std::endl;
}

std::vector<Topic>::iterator
MessageServer::checkTopicStatus(std::string input_topic_name)
{
	std::vector<Topic>::iterator it = find_if(topics.begin(),
		topics.end(), MatchString(input_topic_name));

	return it;
}


void
MessageServer::createTopic(Subscriber* s, std::string input_topic_name)
{
	Topic topic;
	topic.setTopicName(input_topic_name);
	topic.subscribeToThisTopic(s);

	topics.push_back(topic);
	std::cout << "Create new topic: " << input_topic_name << std::endl;
}


void
MessageServer::publishMessage(std::string input_topic_name, Message msg)
{
	std::vector<Topic>::iterator it = checkTopicStatus(input_topic_name);
	if (it != topics.end()) {
		std::vector<Subscriber*> subscribers = it->getSubscribers();

		std::vector<Subscriber*>::iterator p;
		for (p = subscribers.begin(); p != subscribers.end(); p++)
			(*p)->pushToMessageBuffer(msg);
	} 
	else
		std::cout << "Publish message failed: No such topic --- " <<
			input_topic_name << std::endl;
}


void
MessageServer::subscribe(Subscriber* s, std::string input_topic_name)
{
	std::vector<Topic>::iterator it = checkTopicStatus(input_topic_name);
	if (it != topics.end()) {
		std::cout << "subscribeToThisTopic: " << input_topic_name << std::endl;
		it->subscribeToThisTopic(s);
	}
	else
		createTopic(s, input_topic_name);
}

void
MessageServer::unsubscribe(Subscriber* s, std::string input_topic_name)
{
	std::vector<Topic>::iterator it = checkTopicStatus(input_topic_name);
	if (it != topics.end()) {
		std::cout << "Unsubscribing topic..." << std::endl;

		it->unsubscribeFromThisTopic(s);

		if (it->getNumSubscriberToThisTopic() == 0) {
			topics.erase(it);
				std::cout << "Unsubscribe: delete empty topic: " <<
				input_topic_name << std::endl;
		}
	}
	else
		std::cout << "Unsubscribe: No such topic called " << input_topic_name
			<< std::endl;
}


void
MessageServer::unsubscribeOneThreadFromAllTopics(Subscriber* s)
{
	for (std::vector<Topic>::iterator it = topics.begin(); it != topics.end(); ++it)
		it->unsubscribeFromThisTopic(s);
}

Message
Subscriber::fetchAndClearMessageBuffer()
{
	Message temp_msg = message_buffer.front();
	message_buffer.erase(message_buffer.begin());

	return temp_msg;
}
