#include "Header.h"

#include "CServerManager.h"

int __cdecl main()
{
	//std::unique_ptr<CServerManager> cServerManger(new CServerManager());

	CServerManager server;

	server.Initialize();
	
	// 스레드 시작
	std::thread t1{ &CServerManager::ControllerProcess, &server };
	std::thread t2{ &CServerManager::JetBotProcess,		&server };
	
	// 스레드가 종료할 때 까지 대기
	t1.join();
	t2.join();

	server.Release();

	return 0;
}