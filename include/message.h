#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdint.h>
#include <sys/time.h>
#include <string>

class Message
{
public:
	explicit Message(std::string input_msg_content)
	{
        time_stamp = getCurrentTimeInMilliseconds();
        topic_name = "";
        message_content = input_msg_content;
	}
	virtual ~Message() {}

	uint64_t getCurrentTimeInMilliseconds()
	{
		struct timeval current_time;
		uint64_t mtime;
		gettimeofday(&current_time, nullptr);

		mtime = current_time.tv_sec * 1000 + current_time.tv_usec / 1000.0 + 0.5;
		return mtime;
	}

	// Get the message
	std::string getMessageContent() { return message_content; }
	// current time
	uint64_t getTimeStamp() { return time_stamp; }
	// Get topic name
	std::string getTopicName() { return topic_name; }
	// Set content to the message
	inline void setMessageContent(std::string input_msg_content)
	{
		message_content = input_msg_content;
	}

	// Set topic name
	inline void setTopicName(std::string input_topic_name)
	{
		topic_name = input_topic_name;
	}

private:
	std::string message_content;
	uint64_t time_stamp;
	std::string topic_name;
	int data;
};

#endif
