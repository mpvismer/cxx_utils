#include "log_helper.hpp"


#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#include ""support.h"

class UdpServer
{
  private:
    WSADATA _wsa_data;
    SOCKET _socket = INVALID_SOCKET;
    SOCKADDR_IN _send_addr = { 0 };
    SOCKADDR_IN _recv_addr = { 0 };

public:
    UdpServer()
    {
        if (WSAStartup(MAKEWORD(2, 2), &_wsa_data) != 0)
        {
            LOG_ERROR("WSAStartup failed with error %ld\n", WSAGetLastError());
            exit(-1);
        }
    }

    bool Run()
    {
        bool success = true;

        _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (_socket == INVALID_SOCKET){
            LOG_ERROR("Failed to open socket, error: %ld\n", WSAGetLastError());
            success = false;
        }

        if ( (success) && (SOCKET_ERROR == bind(_socket, (SOCKADDR *)&_recv_addr, sizeof(_recv_addr))) )
        {
            LOG_ERROR("Error! bind() failed!");
            closesocket(_socket);
            success = false;
        }

        if (success)
        {
            getsockname(_socket, (SOCKADDR *)&_recv_addr, (int *)sizeof(_recv_addr));
            LOG_INFO("UDP socket open,  %s:%d", inet_ntoa(_recv_addr.sin_addr), htons(_recv_addr.sin_port));
        }

        while (success)
        {
            int addr_size = sizeof(_send_addr);
            int received = recvfrom(_socket, (char*)buf, max_len, 0, (SOCKADDR *)&_send_addr, &addr_size);
            if (received >= 0)
            {
                if (received==0)
                {
                    LOG_DEBUG("recvfrom() failed with error code: %d\n", WSAGetLastError());
                }
                else
                {
                    LOG_TRACE("recvfrom() received %s:%d", inet_ntoa(_send_addr.sin_addr), htons(_send_addr.sin_port));
                    int sent = sendto(_socket, (const char*)buf, len, 0, (SOCKADDR *)&_send_addr, sizeof(_send_addr));
                    if (sent !=len)
                    {
                        LOG_WARN("sendto() only sent %d, attempted %d", buf, len);
                    }
                }
                return received;
            }
            else 
            { 
                LOG_ERROR("Connection closed with error code: %ld\n", WSAGetLastError());
            }
        }
        
        if (_socket!=INVALID_SOCKET)
        {
            if (closesocket(_socket) != 0)
            {
                LOG_WARNING("closesocket() failed! Error code: %ld\n", WSAGetLastError());

            }
        }
    }

    ~UdpServer()
    {
        if( WSACleanup() != 0)
        {
            LOG_WARNING("WSACleanup() failed! Error code: %ld\n", WSAGetLastError());
        }
    }

};


