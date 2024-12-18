#pragma once

#include "Message.h"

class MeshMessage : public Message {
public:
	MeshMessage(std::string meshName) 
		: Message(MessageType::String, "load_mesh"), 
		meshName(meshName) { }

	std::string get_mesh_name() {
		return meshName;
	}

private:
	std::string meshName;
};