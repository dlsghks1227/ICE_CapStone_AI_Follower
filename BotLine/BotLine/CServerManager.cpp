#include "CServerManager.h"


CServerManager::CServerManager()
{
	m_ControllerSocket = INVALID_SOCKET;
	m_JetBotSocket = INVALID_SOCKET;
	m_IsStillActiving = false;
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
	m_ControllerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_ControllerSocket == INVALID_SOCKET)
	{
		std::cout << '[' << WSAGetLastError() << ']' << "소켓 생성 실패 " << '\n';
		WSACleanup();
		return;
	}

	m_JetBotSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_JetBotSocket == INVALID_SOCKET)
	{
		std::cout << '[' << WSAGetLastError() << ']' << "소켓 생성 실패 " << '\n';
		WSACleanup();
		return;
	}

	// 서버 정보 설정
	// IPv4 프로토콜, Port(8888)
	// 프로토콜 체계를 표현할 때 PF_INET
	// 주소 체계를 표현할 때 AF_INET
	SOCKADDR_IN ControllerServerAddr;
	ZeroMemory(&ControllerServerAddr, sizeof(ControllerServerAddr));
	ControllerServerAddr.sin_family = PF_INET;
	ControllerServerAddr.sin_port = htons(7070);
	ControllerServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	SOCKADDR_IN JetBotServerAddr;
	ZeroMemory(&JetBotServerAddr, sizeof(JetBotServerAddr));
	JetBotServerAddr.sin_family = PF_INET;
	JetBotServerAddr.sin_port = htons(8080);
	JetBotServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;


	// 소켓의 로컬 주소를 연결
	nResult = ::bind(m_ControllerSocket, (struct sockaddr*)&ControllerServerAddr, sizeof(ControllerServerAddr));
	if (nResult == SOCKET_ERROR) {
		this->Release();
		return;
	}

	nResult = ::bind(m_JetBotSocket, (struct sockaddr*)&JetBotServerAddr, sizeof(JetBotServerAddr));
	if (nResult == SOCKET_ERROR) {
		this->Release();
		return;
	}

	//SOCKADDR_IN jetbotAddr;
	//ZeroMemory(&jetbotAddr, sizeof(jetbotAddr));
	//jetbotAddr.sin_family = PF_INET;
	//jetbotAddr.sin_port = htons(7070);
	//jetbotAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.63");

	//SOCKADDR_IN jetbot2Addr;
	//ZeroMemory(&jetbot2Addr, sizeof(jetbot2Addr));
	//jetbot2Addr.sin_family = PF_INET;
	//jetbot2Addr.sin_port = htons(7070);
	//jetbot2Addr.sin_addr.S_un.S_addr = inet_addr("192.168.0.4");

	//while (true)
	//{
	//	if (packet[0].compare("c") == 0) {
	//		std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << ']' << msg << '\n';
	//		::sendto(m_ListeneSocket, msg, sizeof(msg), 0, (struct sockaddr*)&jetbotAddr, sizeof(jetbotAddr));
	//	}
	//	if (packet[0].compare("j") == 0) {
	//		std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << ']' << msg << '\n';
	//	}
	//	else
	//	{

	//	}
	//	//::sendto(m_ListeneSocket, msg, sizeof(msg), 0, (struct sockaddr*)&jetbotAddr, sizeof(jetbotAddr));
	//	//::sendto(m_ListeneSocket, msg, sizeof(msg), 0, (struct sockaddr*)&jetbot2Addr, sizeof(jetbot2Addr));
	//	// std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << ']' << packet[0] << '\n';
	//	ZeroMemory(&msg, sizeof(msg));
	//}

	m_IsStillActiving = true;

	return;
}

void CServerManager::Release()
{
	m_IsStillActiving = false;

	closesocket(m_ControllerSocket);
	closesocket(m_JetBotSocket);
	::WSACleanup();
	return;
}

void CServerManager::ControllerProcess()
{
	SOCKADDR_IN clientAddr;
	ZeroMemory(&clientAddr, sizeof(clientAddr));
	int clientAddrSize = sizeof(clientAddr);

	char msg[200];
	ZeroMemory(&msg, sizeof(msg));

	std::cout << "Controller Processing.." << '\n';
	while (m_IsStillActiving) {
		::recvfrom(m_ControllerSocket, msg, sizeof(msg), 0, (struct sockaddr*)&clientAddr, &clientAddrSize);
		std::vector<std::string> message = split(msg, '|');
		if (message.size() > 0) {
			if (message.at(0).compare("connect") == 0) {
				{
					std::lock_guard<std::recursive_mutex>	lock(m_ControllerMutex);
					if (std::none_of(m_ControllerAddrList.begin(), m_ControllerAddrList.end(),
						[clientAddr](auto& element) {
							return (clientAddr.sin_addr.S_un.S_addr == element.sin_addr.S_un.S_addr);
						}))
					{
						SOCKADDR_IN conAddr;
						ZeroMemory(&conAddr, sizeof(conAddr));
						conAddr.sin_family = PF_INET;
						conAddr.sin_port = htons(6060);
						conAddr.sin_addr = clientAddr.sin_addr;
						m_ControllerAddrList.push_back(conAddr);
						std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << ' ' << ntohs(conAddr.sin_port) << "] " << msg << '\n';
					}
				}
			}
			else if (message.at(0).compare("disconnect") == 0) {
				{
					std::lock_guard<std::recursive_mutex>	lock(m_ControllerMutex);
					std::vector<SOCKADDR_IN>::iterator iter = std::find_if(m_ControllerAddrList.begin(), m_ControllerAddrList.end(),
						[clientAddr](auto& element) {
							return (clientAddr.sin_addr.S_un.S_addr == element.sin_addr.S_un.S_addr);
						});
					if (iter != m_ControllerAddrList.end()) {
						m_ControllerAddrList.erase(iter);

						std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << "] " << msg << '\n';
					}
				}
			}
			else if (message.at(0).compare("move") == 0) {
				int controllNumber = std::stoi(message.at(1));
				if (m_JetBotAddrList.size() > controllNumber) {
					std::lock_guard<std::recursive_mutex>	lock(m_JetBotMutex);
					SOCKADDR_IN jetbotAddr = m_JetBotAddrList.at(controllNumber);
					::sendto(m_ControllerSocket, msg, sizeof(msg), 0, (struct sockaddr*)&jetbotAddr, sizeof(jetbotAddr));
					std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << "] " << msg << '\n';
				}
			}
		}
		ZeroMemory(&msg, sizeof(msg));
	}
}

void CServerManager::JetBotProcess()
{
	SOCKADDR_IN clientAddr;
	ZeroMemory(&clientAddr, sizeof(clientAddr));
	int clientAddrSize = sizeof(clientAddr);

	char msg[200];
	ZeroMemory(&msg, sizeof(msg));

	while (m_IsStillActiving) {
		::recvfrom(m_JetBotSocket, msg, sizeof(msg), 0, (struct sockaddr*)&clientAddr, &clientAddrSize);
		std::vector<std::string> message = split(msg, '|');
		if (message.size() > 0) {
			if (message.at(0).compare("connect") == 0) {
				{
					std::lock_guard<std::recursive_mutex>	lock(m_JetBotMutex);

					if (std::none_of(m_JetBotAddrList.begin(), m_JetBotAddrList.end(),
						[clientAddr](auto& element) {
							return (clientAddr.sin_addr.S_un.S_addr == element.sin_addr.S_un.S_addr);
						}))
					{
						clientAddr.sin_port = htons(8080);
						m_JetBotAddrList.push_back(clientAddr);

						std::stringstream ss;
						ss << "jconnect|" << m_JetBotAddrList.size() - 1 << '|' << inet_ntoa(clientAddr.sin_addr) << '|';
						const std::string& connectMsg(ss.str());

						// 잘못하면 교착상태가 발생할 수 있다.
						std::lock_guard<std::recursive_mutex>	lock(m_ControllerMutex);

						for (int i = 0; i < m_ControllerAddrList.size(); i++)
						{
							::sendto(m_JetBotSocket, connectMsg.c_str(), connectMsg.length(), 0, (struct sockaddr*)&m_ControllerAddrList[i], sizeof(m_ControllerAddrList[i]));
							std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << "] ";
							std::cout << inet_ntoa(m_ControllerAddrList[i].sin_addr) << '\n';
						}

						std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << ' ' << ntohs(clientAddr.sin_port) << "] JetBot_" << m_JetBotAddrList.size() - 1 << " Connected" << '\n';
					}
				}
			}
			else if (message.at(0).compare("disconnect") == 0) {
				{
					std::lock_guard<std::recursive_mutex>	lock(m_JetBotMutex);
					std::vector<SOCKADDR_IN>::iterator iter = std::find_if(m_JetBotAddrList.begin(), m_JetBotAddrList.end(),
						[clientAddr](auto& element) {
							return (clientAddr.sin_addr.S_un.S_addr == element.sin_addr.S_un.S_addr);
						});
					if (iter != m_JetBotAddrList.end()) {
						m_JetBotAddrList.erase(iter);

						// 잘못하면 교착상태가 발생할 수 있다.
						std::lock_guard<std::recursive_mutex>	lock(m_ControllerMutex);

						std::stringstream ss;
						ss << "jdisconnect|" << inet_ntoa(clientAddr.sin_addr) << '|';
						const std::string& connectMsg(ss.str());

						for (auto& addr : m_ControllerAddrList) {
							::sendto(m_JetBotSocket, connectMsg.c_str(), connectMsg.length(), 0, (struct sockaddr*)&addr, sizeof(addr));
							std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << "] " << connectMsg << '\n';
						}

						std::cout << '[' << inet_ntoa(clientAddr.sin_addr) << "] " << msg << '\n';
					}

					//std::stringstream ss;

					//const std::string& connectMsg(ss.str());
				}
			}
		}
		ZeroMemory(&msg, sizeof(msg));
	}
}

std::vector<std::string> CServerManager::split(std::string input, char delimiter)
{
	std::vector<std::string> answer;
	std::stringstream ss(input);
	std::string temp;

	while (std::getline(ss, temp, delimiter)) {
		answer.push_back(temp);
	}

	return answer;
}
