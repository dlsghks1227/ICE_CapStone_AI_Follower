#pragma once
#include "Header.h"

class CServerManager
{
private:
	SOCKET		m_ListeneSocket;

public:
	CServerManager();
	~CServerManager();

public:
	void Initialize();
	void Release();
};

