#include "Socket.h"
#include <iostream>
#include <winspool.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")
bool sendData_Socket(const unsigned char* data);
bool sendDataSz_Socket(const unsigned char* data, int size);

SOCKET sock = INVALID_SOCKET;

// connect , send 
#pragma region SDK_IO
 void sendCommand_Socket(const char* command)
{
	unsigned char buffer[1024];
	snprintf(reinterpret_cast<char*>(buffer), sizeof(buffer), "%s\r\n", command);
	sendData_Socket(buffer);
}

bool sendData_Socket(const unsigned char* data)
{
	int result = ::send(sock, reinterpret_cast<const char*>(data), std::strlen(reinterpret_cast<const char*>(data)), 0);
	if (result == SOCKET_ERROR) {
		std::cerr << "Send failed." << std::endl;
		return false;
	}
	return true;
}

bool sendDataSz_Socket(const unsigned char* data, int size)
{
	int result = ::send(sock, reinterpret_cast<const char*>(data), size, 0);
	if (result == SOCKET_ERROR) {
		std::cerr << "Send failed." << std::endl;
		return false;
	}
	return true;
}

bool setReceiveTimeout(SOCKET sock, int timeout) {
	timeval tv;
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = (timeout % 1000) * 1000;
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&tv), sizeof(tv)) == SOCKET_ERROR) {
		std::cerr << "setsockopt failed. Error: " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

int receiveData_Socket(unsigned char* buffer, int size)
{
	//setReceiveTimeout(sock, 2000);

	int bytesReceived = ::recv(sock, reinterpret_cast<char*>(buffer), size, 0);
	if (bytesReceived == SOCKET_ERROR) {
		int error = WSAGetLastError();
		if (error == WSAETIMEDOUT) {
			std::cerr << "Receive operation timed out." << std::endl;
		}
		else {
			std::cerr << "Receive failed. Error: " << error << std::endl;
		}
		return -1;
	}

	if (bytesReceived > 0 && bytesReceived < size) {
		buffer[bytesReceived] = '\0';  // Null-terminate the received data if space allows
	}

	return bytesReceived;
}

 bool connectSocket(const char* ip, int port)
{
#ifdef _WIN32
	 WSADATA wsaData;
	 if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		 std::cerr << "WSAStartup failed." << std::endl;
		 return false;
	 }
#endif

	 sock = socket(AF_INET, SOCK_STREAM, 0);
	 if (sock == INVALID_SOCKET) {
		 std::cerr << "Socket creation failed." << std::endl;
#ifdef _WIN32
		 WSACleanup();
#endif
		 return false;
	 }

	 // 设置为非阻塞模式
#ifdef _WIN32
	 u_long mode = 1;
	 ioctlsocket(sock, FIONBIO, &mode);
#else
	 int flags = fcntl(sock, F_GETFL, 0);
	 fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#endif

	 sockaddr_in serverAddr;
	 serverAddr.sin_family = AF_INET;
	 serverAddr.sin_port = htons(port);
	 inet_pton(AF_INET, ip, &serverAddr.sin_addr);

	 int result = ::connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	 if (result == SOCKET_ERROR) {
#ifdef _WIN32
		 if (WSAGetLastError() != WSAEWOULDBLOCK) {
#else
		 if (errno != EINPROGRESS) {
#endif
			 std::cerr << "Connection failed." << std::endl;
#ifdef _WIN32
			 closesocket(sock);
			 WSACleanup();
#else
			 ::close(sock);
#endif
			 return false;
		 }
		 }

	 fd_set writefds;
	 FD_ZERO(&writefds);
	 FD_SET(sock, &writefds);

	 timeval tv;
	 tv.tv_sec = 2000 / 1000;
	 tv.tv_usec = 0;

	 result = select(sock + 1, NULL, &writefds, NULL, &tv);
	 if (result <= 0) {
		 std::cerr << "Connection timed out or failed." << std::endl;
#ifdef _WIN32
		 closesocket(sock);
		 WSACleanup();
#else
		 ::close(sock);
#endif
		 return false;
	 }

	 // 检查连接是否成功
	 int sock_error;
	 socklen_t len = sizeof(sock_error);
	 getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&sock_error, &len);
	 if (sock_error != 0) {
		 std::cerr << "Connection failed. Error: " << sock_error << std::endl;
#ifdef _WIN32
		 closesocket(sock);
		 WSACleanup();
#else
		 ::close(sock);
#endif
		 return false;
	 }

	 // 设置回阻塞模式
#ifdef _WIN32
	 mode = 0;
	 ioctlsocket(sock, FIONBIO, &mode);
#else
	 fcntl(sock, F_SETFL, flags);
#endif

	 return true;
}

 void closeport_Socket()
{
#ifdef _WIN32
	closesocket(sock);
	WSACleanup();
#else
	::close(sock);
#endif
}
#pragma endregion