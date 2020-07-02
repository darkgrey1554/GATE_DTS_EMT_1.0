#pragma once

#include <iostream>
#include <thread>
#include <list>
#include "logger.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <aio.h> 
#include <errno.h>

enum class TypeData
{
	EMPTY,
	ANALOG,
	DISCRETE,
	BINAR,
	GROUP
};

enum class TypeSignal
{
	EMPTY,
	OUTPUT,
	INPUT
};

std::ostream& operator<<(std::ostream& out, TypeSignal& m);
std::ostream& operator<<(std::ostream& out, TypeData& m);


struct ConfigInfoTCPUnit
{
	TypeData type_data = TypeData::EMPTY;
	TypeSignal type_signal = TypeSignal::EMPTY;
	int size_data = 0;
	std::string ip_address;
	std::string namesharedmemory;
	int port = 0;
	int frequency_time = 0;
	int id_unit = -1;
	int channel = -1;

	void clear();
};


class ConfigReader
{
	std::list<ConfigInfoTCPUnit> listinfo;
	LoggerSpace::Logger* logger = LoggerSpace::Logger::getpointcontact();

public:

	ConfigReader();
	~ConfigReader();
	int ReadConfigFile(const char* namefile);
	ConfigInfoTCPUnit ReadConfigItem(int number);
	int NumberItem();
};

class TCPUnit
{
public:

	ConfigInfoTCPUnit set;
	char* buf;
	pthread_mutex_t mutex;
	int status_mutex = 0;

	void virtual restart_thread() {}
	void virtual close_tcp_unit() {}
	void virtual initpointbuffer(char* inbuf) {};
	void virtual initmutexbuffer(pthread_mutex_t inmutex) {};

	static TCPUnit* create_tcp_unit(ConfigInfoTCPUnit unit);
};

class TCPServer : public TCPUnit
{

	std::thread thread_unit;

	int thread_tcp_server();

public:

	TCPServer(ConfigInfoTCPUnit confgate);
	void restart_thread() override;
	void close_tcp_unit() override;
	void initpointbuffer(char* buf) override;
	void initmutexbuffer(pthread_mutex_t mutex) override;

};

class TCPClient : public TCPUnit
{
	std::thread thread_unit;
	int thread_tcp_client();

public:

	TCPClient(ConfigInfoTCPUnit confgate);
	void restart_thread() override;
	void close_tcp_unit() override;
	void initpointbuffer(char* buf) override;
	void initmutexbuffer(pthread_mutex_t mutex) override;
};

