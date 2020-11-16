#pragma once
#include "Header.h"

constexpr int BUFFERSIZE{ 1024 };

class CMessage
{
private:
	BYTE	m_Buffer[BUFFERSIZE];
	DWORD	m_Length;

public:
	CMessage();
	~CMessage();


public:
	BOOL	SetBuffer(BYTE* buffer);
	BOOL	SetBuffer();
};