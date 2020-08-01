#include "Socket.h"



sock::sock() {
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	sock::winsockInit();
};

int sock::winsockInit() {
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cerr << "WSAStartup failed: " << iResult << "\n";
		return 1100;
	}
	else {
		std::cout << "\nWSA started.\n";
		return 0;
	}
};
int sock::resolveAddr(char port[4]) {
	iResult = getaddrinfo(NULL, port, &hints, &result);
	if (iResult != 0) {
		std::cerr << "getaddrinfo failed: " << iResult << "\n";
		WSACleanup();
		return 1200;
	}
	else return 0;
}
int sock::createSocket() {
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		std::cerr << "Error at socket(): " << WSAGetLastError() << "\n";;
		freeaddrinfo(result);
		WSACleanup();
		return 1300;
	}
	return 0;
}
int sock::tcpListen() {
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "bind failed with error: " << WSAGetLastError() << "\n";
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1400;
	}
	freeaddrinfo(result);
	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cerr << "Listen failed with error: " <<  WSAGetLastError() << "\n";
		closesocket(ListenSocket);
		WSACleanup();
		return 1500;
	}
	return 0;
}

int sock::tcpAccept() {
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		std::cerr << "accept failed: "<< WSAGetLastError() << "\n";
		closesocket(ListenSocket);
		WSACleanup();
		return 1600;
	}
	else std::cout << "Client " << ClientSocket << " connected. \n";
	return 0;
}
int sock::receiveData(std::string adat, INPUT ip) {
	do {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			/*std::cout << "Bytes received: "<< iResult << "\n";*/
			std::cout << "Data received. \n";
			if (iResult == adat.size())	
				if (std::string(recvbuf, 0, adat.size()) == adat)
				{
					std::cout << " Button pressed. \n";
					//This let's you do a hardware scan instead of a virtual keypress
					ip.ki.dwFlags = 0;
					//Send the press
					SendInput(1, &ip, sizeof(INPUT));
					//Prepare a keyup event
					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &ip, sizeof(INPUT));
				}
			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				std::cerr << "Send failed: " << WSAGetLastError() << "\n";
				closesocket(ClientSocket);
				WSACleanup();
				return 1700;
			}
			/*printf("Bytes sent: %d\n", iSendResult);*/
		}
		else if (iResult == 0)
		{
			std::cout << "Client " << ClientSocket << " disconnected. \n";
		}
		else {
			std::cerr << "Receive failed: " << WSAGetLastError() << "\n";
			closesocket(ClientSocket);
			WSACleanup();
			return 1800;
		}

	} while (iResult > 0);
}
int sock::close() {
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		std::cerr << "Shutdown failed: " << WSAGetLastError() << "\n";
		closesocket(ClientSocket);
		WSACleanup();
		return 1900;
	}
	closesocket(ClientSocket);
	WSACleanup();
}



