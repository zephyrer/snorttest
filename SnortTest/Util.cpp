#include "StdAfx.h"
#include "Util.h"



Util::Util(void)
{
}

Util::~Util(void)
{
}

string Util::Int2Ip( DWORD dwIp)
{
	in_addr addr;
	//addr.S_un.S_addr=ntohl(dwIp);
	addr.S_un.S_addr = dwIp;
	return inet_ntoa(addr);
}

bool Util::ConvertHex(char* rule, char* out, u_int& dummy_size)
{
	char tmp_buf[MAX_PATTERN_SIZE];
	bool pattern_buf = false;
    /* got enough ptrs for you? */
    char *start_ptr;
    char *end_ptr;
    char *idx;
    char *dummy_idx;
    char *dummy_end;
    char *tmp;
    char hex_buf[3];
    int size;
    int hexmode = 0;
    int hexsize = 0;
    int pending = 0;
    int cnt = 0;
    int literal = 0;
    int exception_flag = 0;
	dummy_size = 0;

    /* clear out the temp buffer */
    bzero(tmp_buf, MAX_PATTERN_SIZE);

    if (rule == NULL)
        return pattern_buf;

    while(isspace((int)*rule))
        rule++;

    if(*rule == '!')
    {
        exception_flag = 1;
        while(isspace((int)*++rule));
    }

	start_ptr = rule;
	end_ptr = strrchr(rule, '\0');

	if (end_ptr == NULL)
	    return pattern_buf;


    /* how big is it?? */
    size = end_ptr - start_ptr;

    /* uh, this shouldn't happen */
    if (size <= 0)
         return pattern_buf;

    /* set all the pointers to the appropriate places... */
    idx = start_ptr;

    /* set the indexes into the temp buffer */
    dummy_idx = tmp_buf;
    dummy_end = (dummy_idx + size);

    /* why is this buffer so small? */
    bzero(hex_buf, 3);
    memset(hex_buf, '0', 2);

    /* BEGIN BAD JUJU..... */
    while(idx < end_ptr)
    {
        if (dummy_size >= MAX_PATTERN_SIZE-1)
        {
            /* Have more data to parse and pattern is about to go beyond end of buffer */
             return pattern_buf;
        }

        switch(*idx)
        {
            case '|':
                if(!literal)
                {
                    if(!hexmode)
                    {
                        hexmode = 1;
                    }
                    else
                    {

                        /*
                        **  Hexmode is not even.
                        */
                        if(!hexsize || hexsize % 2)
                        {
                             return NULL;
                        }

                        hexmode = 0;
                        pending = 0;
                    }

                    if(hexmode)
                        hexsize = 0;
                }
                else
                {
                    literal = 0;
                    tmp_buf[dummy_size] = start_ptr[cnt];
                    dummy_size++;
                }

                break;

            case '\\':

                if(!literal)
                {
                    /* Make sure the next char makes this a valid
                     * escape sequence.
                     */
                    if (idx [1] != '\0' && strchr ("\\\":;", idx [1]) == NULL)
                    {
                         return pattern_buf;
                    }

                    literal = 1;
                }
                else
                {
                    tmp_buf[dummy_size] = start_ptr[cnt];
                    literal = 0;
                    dummy_size++;
                }

                break;
            case '"':
                if (!literal)
                    return pattern_buf;
                /* otherwise process the character as default */
            default:
                if(hexmode)
                {
                    if(isxdigit((int) *idx))
                    {
                        hexsize++;

                        if(!pending)
                        {
                            hex_buf[0] = *idx;
                            pending++;
                        }
                        else
                        {
                            hex_buf[1] = *idx;
                            pending--;

                            if(dummy_idx < dummy_end)
                            {
                                tmp_buf[dummy_size] = (u_char)
                                    strtol(hex_buf, (char **) NULL, 16)&0xFF;

                                dummy_size++;
                                bzero(hex_buf, 3);
                                memset(hex_buf, '0', 2);
                            }
                            else
                            {
                                 return NULL;
                            }
                        }
                    }
                    else
                    {
                        if(*idx != ' ')
                        {
                             return NULL;
                        }
                    }
                }
                else
                {
                    if(*idx >= 0x1F && *idx <= 0x7e)
                    {
                        if(dummy_idx < dummy_end)
                        {
                            tmp_buf[dummy_size] = start_ptr[cnt];
                            dummy_size++;
                        }
                        else
                        {
                             return NULL;
                        }

                        if(literal)
                        {
                            literal = 0;
                        }
                    }
                    else
                    {
                        if(literal)
                        {
                            tmp_buf[dummy_size] = start_ptr[cnt];
                            dummy_size++;
                             return NULL;
                            literal = 0;
                        }
                        else
                        {
                             return NULL;
                        }
                    }
                }

                break;
        }

        dummy_idx++;
        idx++;
        cnt++;
    }
    /* ...END BAD JUJU */

    /* error prunning */

    if (literal)
         return NULL;

    if (hexmode)
         return NULL;

	pattern_buf = true;


    memcpy(out, tmp_buf, dummy_size);

	return dummy_size;

}