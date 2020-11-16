#pragma once
#include "Header.h"

class CServerManager
{
private:
	SOCKET	m_ControllerSocket;
	SOCKET	m_JetBotSocket;

	bool	m_IsStillActiving;

	std::vector<SOCKADDR_IN>	m_ControllerAddrList;
	std::recursive_mutex					m_ControllerMutex;
	std::vector<SOCKADDR_IN>	m_JetBotAddrList;
	std::recursive_mutex					m_JetBotMutex;

public:
	CServerManager();
	~CServerManager();

public:
	void Initialize();
	void Release();

	void ControllerProcess();
	void JetBotProcess();

	std::vector<std::string>	split(std::string input, char delimiter);
};

