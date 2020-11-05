#include "Header.h"

#include "CServerManager.h"

int __cdecl main()
{
	std::unique_ptr<CServerManager> cServerManger(new CServerManager());

	cServerManger->Initialize();

	cServerManger->Release();

	return 0;
}