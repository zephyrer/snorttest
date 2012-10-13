#pragma once
#include <WinSock2.h>

class CSnortSocket
{
public:
	CSnortSocket(void);

	~CSnortSocket(void);

	bool SendTcp(const char* pAddr, const DWORD& dwPort, const char* pszBuffer, const int& nSize);

	bool SendUdp(const char* PAddr, const DWORD& dwPort, const char* pszBuffer, const int& nSize);

	void Close();

private:

	bool SnortConnect(const char* pAddr, const DWORD& dwPort);

	SOCKET m_sock;
};
