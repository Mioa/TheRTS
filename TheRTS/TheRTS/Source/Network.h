#ifndef NETWORK_H
#define NETWORK_H

#define WIN32_LEAN_AND_MEAN

#include <thread>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment( lib, "Ws2_32.lib" )

#define NE_MAX_SOCKETS		8
#define NE_DEFAULT_BUFLEN	512
#define NE_DEFAULT_PORT		27015

#define NE_MSG_WELCOME	1
#define NE_MSG_INPUT	2

class Network
{
	public:
		UINT		numConnections			= 0;
		int			mySocketIndex			= -1;

		bool		receiveThreadRunning	= false;
		bool		sendThreadRunning		= false;
		std::string sendMsgBuffer			= "";
		std::string	receiveMsgBuffer		= "";

	private:
		bool	isHosting = false;
		SOCKET	clientSockets[NE_MAX_SOCKETS];
	
	public:
		HRESULT Initialize();
		void	Release();

				Network();
				~Network();
	public:
		HRESULT	StartHostSession();
		HRESULT	AddConnection();

		HRESULT	ReceiveData();
		HRESULT SendData();

		HRESULT SendWelcomeMessage( UINT socketIndex_ );

	private:
};
#endif