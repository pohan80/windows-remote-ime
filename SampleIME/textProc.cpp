#include "textProc.h"
#include <ws2tcpip.h>
#include <fstream>

#pragma comment (lib, "Ws2_32.lib")



textProc::textProc(CSampleIME *pTextService){
	_pTextService = pTextService;
	_processThread = std::thread(&textProc::startConnectionProc,this);
}

textProc::~textProc(){
	_processThread.join();
}

void textProc::startConnectionProc(){
	WSADATA wsaData;
	WCHAR wcstring[10];
	char *connectHint = "ok";
	int iResult, len;
	int errorCode = 0;
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	ListenSocket = INVALID_SOCKET;
	ClientSocket = INVALID_SOCKET;

	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		errorCode = iResult;
		goto exitthread;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if ( iResult != 0 ) {
		errorCode = WSAGetLastError();
		WSACleanup();
		goto exitthread;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		errorCode = WSAGetLastError();
		freeaddrinfo(result);
		WSACleanup();
		goto exitthread;
	}

	// Setup the TCP listening socket
	iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		errorCode = WSAGetLastError();
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		goto exitthread;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		errorCode = WSAGetLastError();
		closesocket(ListenSocket);
		WSACleanup();
		goto exitthread;
	}

	while(true){

		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			errorCode = WSAGetLastError();
			closesocket(ListenSocket);
			WSACleanup();
			goto exitthread;
		}

		iResult = send(ClientSocket, connectHint, strlen(connectHint),0);
		if (iResult == SOCKET_ERROR) {
			errorCode = WSAGetLastError();
			closesocket(ClientSocket);
			WSACleanup();
			goto exitthread;
		}

		while(true){
			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
			if(iResult > 0){
				recvbuf[iResult] = '\0';
				len = MultiByteToWideChar(CP_ACP, 0, recvbuf, -1, NULL,0);
				if(len > 0){
					MultiByteToWideChar(CP_ACP,0,recvbuf, -1,wcstring,len);
				}
				_pTextService->setIsRemoteUse(true);
				_pTextService->setWch(wcstring);
				keybd_event(0x31,0,0,0);
			}else{
				closesocket(ClientSocket);
				break;
			}

		}
	}

exitthread:
	recordLog(errorCode);
}

void textProc::recordLog(int errorCode){
	std::fstream file;
	file.open("Error.log",std::ios::out|std::ios::app);
	file << "[Error] : " << errorCode << std::endl;
	file.close();
}

void textProc::stopSocket(){
	closesocket(ListenSocket);
	closesocket(ClientSocket);
}