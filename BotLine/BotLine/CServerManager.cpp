#include "CServerManager.h"


CServerManager::CServerManager()
{
	m_ListeneSocket = INVALID_SOCKET;
}

CServerManager::~CServerManager()
{
}

void CServerManager::Initialize()
{
	WSADATA m_WSAData;
	int nResult;
	// winsock �ʱ�ȭ
	nResult = ::WSAStartup(MAKEWORD(2, 2), &m_WSAData);
	if (nResult != NO_ERROR) {
		std::cout << '[' << nResult << ']' << "Winsock �ʱ�ȭ ����" << '\n';
		return;
	}

	// IP �ּ� ���
	char szLocalHostName[512];
	struct hostent* pLocalHostInformation;
	gethostname(szLocalHostName, sizeof(szLocalHostName));
	pLocalHostInformation = gethostbyname(szLocalHostName);
	for (int i = 0; pLocalHostInformation->h_addr_list[i] != nullptr; i++) {
		std::cout << "Local Address (IPv4) : " << inet_ntoa(*(struct in_addr*)pLocalHostInformation->h_addr_list[i]) << '\n';
	}

	// ���� ����
	// IPv4 ��������, �����ͱ׷�(UDP)
	m_ListeneSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_ListeneSocket == INVALID_SOCKET)
	{
		std::cout << '[' << WSAGetLastError() << ']' << "���� ���� ���� " << '\n';
		WSACleanup();
		return;
	}

	// ���� ���� ����
	// IPv4 ��������, Port(8888)
	// �������� ü�踦 ǥ���� �� PF_INET
	// �ּ� ü�踦 ǥ���� �� AF_INET
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);


	// ������ ���� �ּҸ� ����
	nResult = ::bind(m_ListeneSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if (nResult == SOCKET_ERROR) {
		closesocket(m_ListeneSocket);
		WSACleanup();
		return;
	}

	char msg[20];
	ZeroMemory(&msg, sizeof(msg));

	SOCKADDR_IN clientAddr;
	ZeroMemory(&clientAddr, sizeof(clientAddr));

	int clientAddrSize = sizeof(clientAddr);
	::recvfrom(m_ListeneSocket, msg, sizeof(msg), 0, (struct sockaddr*)&clientAddr, &clientAddrSize);
	std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << ']' << msg << '\n';
	
	return;
}

void CServerManager::Release()
{
	closesocket(m_ListeneSocket);
	::WSACleanup();
	return;
}
