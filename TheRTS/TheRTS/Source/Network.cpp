#include "Network.h"
#include <iostream>

HRESULT Network::Initialize()
{
	WORD wVer = MAKEWORD( 2, 2 );
	WSADATA wsaData;

	int nRet = WSAStartup( wVer, &wsaData );

	if( nRet == SOCKET_ERROR )
	{
		std::cout << "Failed to init WinSock library\n";
		return E_FAIL;
	}

	return S_OK;
}

void Network::Release()
{

}

Network::Network()
{

}

Network::~Network()
{

}

HRESULT Network::StartHostSession()
{
	if( !isHosting )
	{
		std::cout << "Starting server\n";
		isHosting = true;

		SOCKET hSock;

		hSock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

		if( hSock == INVALID_SOCKET )
		{
			std::cout << "Invalid socket, failed creation\n";
			return E_FAIL;
		}

		sockaddr_in saListen = { 0 };
		saListen.sin_addr.s_addr	= htonl( INADDR_ANY );
		saListen.sin_family			= PF_INET;
		saListen.sin_port			= htons( NE_DEFAULT_PORT );

		int nRet = bind( hSock, (SOCKADDR*)&saListen, sizeof(SOCKADDR) );
		if( nRet == SOCKET_ERROR )
		{
			std::cout << "Failed to bind socket\n";
			closesocket( hSock );
			return E_FAIL;
		}

		std::cout << "Listening for connections\n";

		while( numConnections < 1 )
		{
			nRet = listen( hSock, 0 );
			if( nRet == SOCKET_ERROR )
			{
				int nErr = WSAGetLastError();
				if( nErr == WSAECONNREFUSED )
					std::cout << "Failed to listen, connection refused\n";
				else
					std::cout << "Call to listen failed\n";
				closesocket( hSock );
				return E_FAIL;
			}

			sockaddr_in saClient			= { 0 };
			int nSALen						= sizeof( SOCKADDR );
			clientSockets[numConnections]	= accept( hSock, (SOCKADDR*)&saClient, &nSALen );

			if( clientSockets[1] == INVALID_SOCKET )
			{
				std::cout << "Invalid client socket, connection failed\n";
				closesocket( hSock );
				return E_FAIL;
			}

			std::cout << "Connection established\n";
			numConnections++;
			closesocket( hSock );
		}
	}
	return S_OK;
}

HRESULT Network::AddConnection()
{
	std::string IP = "127.0.0.1";

	std::cout << "Trying to join IP " << IP.c_str() << '\n';

	clientSockets[numConnections] = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

	if( clientSockets[numConnections] == INVALID_SOCKET )
	{
		std::cout << "Invalid socket, failed creation\n";
		return E_FAIL;
	}

	sockaddr_in saServer = { 0 };
	saServer.sin_addr.s_addr	= inet_addr( IP.c_str() );
	saServer.sin_family			= PF_INET;
	saServer.sin_port			= htons( NE_DEFAULT_PORT );

	int nRet = connect( clientSockets[numConnections], (SOCKADDR*)&saServer, sizeof( SOCKADDR ) );

	if( nRet == SOCKET_ERROR )
	{
		std::cout << "Connection to server failed\n";
		closesocket( clientSockets[numConnections] );
		return E_FAIL;
	}
	std::cout << "Connection established\n";
	numConnections++;

	return S_OK;
}

HRESULT Network::ReceiveData()
{
	for( UINT i  = 0; i < numConnections; i++ )
	{
		char	wzRec[NE_DEFAULT_BUFLEN];
		int		nLeft	= NE_DEFAULT_BUFLEN;
		int		iPos	= 0;
		int		nData	= 0;

		do
		{
			nData = recv( clientSockets[i], &wzRec[iPos], nLeft, 0 );
			if( nData == SOCKET_ERROR )
			{
				std::cout << "Error receiving data\n";
				closesocket( clientSockets[i] );
				numConnections--;
				break;
			}
			nLeft	-= nData;
			iPos	+= nData;
		} while( nLeft > 0 );

		std::string message = wzRec;
		ZeroMemory( wzRec, sizeof( wzRec ) );

		std::cout << "Message: " << message.c_str() << '\n';
	}
	return S_OK;
}

HRESULT Network::SendData()
{
	for( UINT i  = 0; i < numConnections; i++ )
	{
		std::cout << "IMMA SEND!\n";
		char	wzRec[NE_DEFAULT_BUFLEN] = "";
		int		nLeft	= NE_DEFAULT_BUFLEN;
		int		iPos	= 0;
		int		nData	= 0;

		strcpy( wzRec, "Dis be valuez" );

		do
		{
			nData = send( clientSockets[i], &wzRec[iPos], nLeft, 0 );
			if( nData == SOCKET_ERROR )
			{
				std::cout << "Error receiving data\n";
				closesocket( clientSockets[i] );
				numConnections--;
				break;
			}
			nLeft	-= nData;
			iPos	+= nData;
		} while( nLeft > 0 );

		ZeroMemory( wzRec, sizeof( wzRec ) );
	}

	return S_OK;
}