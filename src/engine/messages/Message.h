#pragma once

#include <string>

enum MessageType {
	String,
};

class Message {
public:
	Message(MessageType type, std::string msg);
	virtual ~Message();

	MessageType type;
	std::string msg;
};