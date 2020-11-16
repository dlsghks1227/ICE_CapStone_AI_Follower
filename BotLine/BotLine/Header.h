#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif


#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <algorithm>

#include <thread>
#include <mutex>

#pragma comment (lib, "ws2_32.lib")