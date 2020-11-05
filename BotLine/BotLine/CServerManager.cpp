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
	// winsock 초기화
	nResult = ::WSAStartup(MAKEWORD(2, 2), &m_WSAData);
	if (nResult != NO_ERROR) {
		std::cout << '[' << nResult << ']' << "Winsock 초기화 실패" << '\n';
		return;
	}

	// IP 주소 출력
	char szLocalHostName[512];
	struct hostent* pLocalHostInformation;
	gethostname(szLocalHostName, sizeof(szLocalHostName));
	pLocalHostInformation = gethostbyname(szLocalHostName);
	for (int i = 0; pLocalHostInformation->h_addr_list[i] != nullptr; i++) {
		std::cout << "Local Address (IPv4) : " << inet_ntoa(*(struct in_addr*)pLocalHostInformation->h_addr_list[i]) << '\n';
	}

	// 소켓 생성
	// IPv4 프로토콜, 데이터그램(UDP)
	m_ListeneSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_ListeneSocket == INVALID_SOCKET)
	{
		std::cout << '[' << WSAGetLastError() << ']' << "소켓 생성 실패 " << '\n';
		WSACleanup();
		return;
	}

	// 서버 정보 설정
	// IPv4 프로토콜, Port(8888)
	// 프로토콜 체계를 표현할 때 PF_INET
	// 주소 체계를 표현할 때 AF_INET
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);


	// 소켓의 로컬 주소를 연결
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
