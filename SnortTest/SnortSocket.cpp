#include "StdAfx.h"
#include "SnortSocket.h"
#pragma comment(lib, "Ws2_32.lib")

CSnortSocket::CSnortSocket(void)
{
	WSADATA ws; 

	WSAStartup(MAKEWORD(2,2),&ws);
}

CSnortSocket::~CSnortSocket(void)
{
	WSACleanup();
}


bool CSnortSocket::SnortConnect(const char* pAddr, const DWORD& dwPort)
{
	Close();

	m_sock = ::socket(AF_INET, SOCK_STREAM, 0);

	if( m_sock == INVALID_SOCKET )
	{
		return false;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr= inet_addr(pAddr);
	addr.sin_port = htons(dwPort);

	if( connect(m_sock, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == SOCKET_ERROR )
	{
		return false;
	}

	int   timeout = 15000;  
	setsockopt(m_sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(timeout));  

	return true;
}

bool CSnortSocket::SendTcp(const char* pAddr, const DWORD& dwPort, const char* pszBuffer, const int& nSize)
{
	if( !SnortConnect(pAddr, dwPort) )
	{
		return false;
	}

	DWORD dwRet = 0;

	dwRet = ::send(m_sock, pszBuffer, nSize, 0);

	return dwRet == nSize;
}

bool CSnortSocket::SendUdp(const char* pAddr, const DWORD& dwPort, const char* pszBuffer, const int& nSize)
{
	Close();

	m_sock = ::socket(AF_INET, SOCK_DGRAM, 0);

	if( m_sock == INVALID_SOCKET )
	{
		return false;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr= inet_addr(pAddr);
	addr.sin_port = htons(dwPort);

	DWORD dwRet = 0;

	dwRet = ::sendto(m_sock, pszBuffer, nSize, 0, (struct sockaddr*)&addr, sizeof(addr));

	return dwRet == nSize;
}

void CSnortSocket::Close()
{
	if (m_sock != INVALID_SOCKET)
	{
		::closesocket(m_sock);
		m_sock = INVALID_SOCKET;		
	}
}
