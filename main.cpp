#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include<iostream>
#include <string>
#define DEFAULT_PORT "2500"
#define DEF_IP "localhost"
#define DEFAULT_BUFLEN 512
#define ADAT "asd"
#define ADATLENGTH 3

#pragma comment(lib, "Ws2_32.lib")

int main() {


	//Structure for the keyboard event
	INPUT ip;
	//Set up the INPUT structure
	ip.type = INPUT_KEYBOARD;
	ip.ki.time = 0;
	ip.ki.wVk = 0x74; //F5 key
	ip.ki.dwExtraInfo = 0;
	ip.ki.wScan = 0;  //Set 0 because we use wVK

		std::cout << "Listener is starting...";
		WSADATA wsaData;
		SOCKET ConnectSocket = INVALID_SOCKET;
		struct addrinfo* result = NULL,
			* ptr = NULL,
			hints;
		char recvbuf[DEFAULT_BUFLEN];
		int iResult;
		int recvbuflen = DEFAULT_BUFLEN;
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			std::cout << "WSAStartup failed:" << iResult << "\n";
			return 1;
		}
		else {
			std::cout << "\nListener started.";
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		// Resolve the server address and port
		iResult = getaddrinfo(DEF_IP, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return 1;
			}

			// Connect to server.
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}
			break;
		}
		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return 1;
		}

		const char* sendbuf = "this is a test";
		// Send an initial buffer
		iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		printf("Bytes Sent: %ld\n", iResult);

		// shutdown the connection since no more data will be sent
		iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}
		bool check;
	

		// Receive until the peer closes the connection
		do {
			iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0)
			{
				Sleep(2000);
				printf("Bytes received: %d\n", iResult);
				std::cout << "rb" << recvbuf << "rblen" << recvbuflen << "\n";
				if (iResult == ADATLENGTH)
				{
					for (int i = 0; i < ADATLENGTH - 1; i++)
					{
						if (recvbuf[i] == ADAT[i])
							check = true;
						else {
							check = false;
							break;
						}
					}
					if (check == true)
					{
						std::cout << "Press";
						//This let's you do a hardware scan instead of a virtual keypress
						ip.ki.dwFlags = 0;
						//Send the press
						SendInput(1, &ip, sizeof(INPUT));
						//Prepare a keyup event
						ip.ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, &ip, sizeof(INPUT));
					}
					else continue;
				}				
			}
			else if (iResult == 0)
				/*printf("Connection closed\n");*/;
			else
				printf("recv failed: %d\n", WSAGetLastError());
		} while (1);

		// cleanup
		closesocket(ConnectSocket);
		WSACleanup();	
}