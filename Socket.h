#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

class sock
{
	WSADATA wsaData;
	int iResult;
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;
	char recvbuf[512];
	int recvbuflen = 512;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	int iSendResult;
public:
	sock();
	int winsockInit();
	int resolveAddr(char port[4]);
	int createSocket();
	int tcpListen();
	int tcpAccept();
	int receiveData(std::string adat, INPUT ip);
	int close();
};

