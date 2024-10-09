#ifndef SOCKET_H
#define SOCKET_H

#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
typedef int SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR    (-1)
#endif
#include <string>


bool connectSocket(const char* ip, int port);
 void closeport_Socket();
 void sendCommand_Socket(const char* command);
 int receiveData_Socket(unsigned char* buffer, int size);
#endif // SOCKET_H