#pragma once

#define bzero(addr, count) memset((addr), 0, (count))
#define MAX_PATTERN_SIZE 2048

class Util
{
public:
	Util(void);
	~Util(void);
	static string Int2Ip( DWORD dwIp);

	static bool ConvertHex(char* rule, char* out, u_int& dummy_size);

};
