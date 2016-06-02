// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <fstream>
#include <iostream>
#include <sstream>

#include "Common/FileUtil.h"
#include "Core/MemoryWatcher.h"
#include "Core/HW/Memmap.h"

MemoryWatcher::MemoryWatcher() : m_context(1)
{
	if (!LoadAddresses(File::GetUserPath(F_MEMORYWATCHERLOCATIONS_IDX)))
		return;
	if (!OpenSocket(File::GetUserPath(F_MEMORYWATCHERSOCKET_IDX)))
	  return;
	m_running = true;
}

bool MemoryWatcher::LoadAddresses(const std::string& path)
{
	std::ifstream locations(path);
	if (!locations)
		return false;

	std::string line;
	while (std::getline(locations, line))
		ParseLine(line);

	return m_values.size() > 0;
}

void MemoryWatcher::ParseLine(const std::string& line)
{
	m_values[line] = 0;
	m_addresses[line] = std::vector<u32>();

	std::stringstream offsets(line);
	offsets >> std::hex;
	u32 offset;
	while (offsets >> offset)
		m_addresses[line].push_back(offset);
}

bool MemoryWatcher::OpenSocket(const std::string& path)
{
  std::cout << "Connecting zmq socket to " << path << std::endl;
  
  if (!File::Exists(path))
  {
    std::cout << "Socket does not exist!" << std::endl;
    return false;
  }
  
  m_socket = std::make_unique<zmq::socket_t> (m_context, ZMQ_REQ);
  try
  {
    m_socket->connect(("ipc://" + path).c_str());
  }
  catch (zmq::error_t& e)
  {
    std::cout << "Error connecting socket: " << e.what() << std::endl;
    return false;
  }
  std::cout << "Connected zmq socket to " << path << std::endl;
  return true;
}

u32 MemoryWatcher::ChasePointer(const std::string& line)
{
	u32 value = 0;
	for (u32 offset : m_addresses[line])
		value = Memory::Read_U32(value + offset);
	return value;
}

std::string MemoryWatcher::ComposeMessages()
{
	std::stringstream message_stream;
	message_stream << std::hex;
	
	for (auto& entry : m_values)
	{
		std::string address = entry.first;
		u32& current_value = entry.second;

		u32 new_value = ChasePointer(address);
		if (new_value != current_value)
		{
			// Update the value
			current_value = new_value;
      message_stream << address << '\n' << new_value << '\n';
		}
	}
	
	return message_stream.str();
}

void MemoryWatcher::Poll()
{
  if (!m_running) return;
  
  std::string message = ComposeMessages();
  
  zmq::message_t zmq_msg (message.size());
  memcpy (zmq_msg.data (), message.c_str(), message.size());
  m_socket->send (zmq_msg);
  
  //TODO: do something with this request
  zmq::message_t request;
  m_socket->recv (&request);
}

