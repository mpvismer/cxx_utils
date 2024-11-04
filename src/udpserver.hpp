#include "logger.hpp"


#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")

#define UDPSERVER_MAX_FRAME_SIZE (65536u)



class udpserver
{
  private:
    WSADATA _wsa_data;
    SOCKET _socket = INVALID_SOCKET;
    SOCKADDR_IN _send_addr = { 0 };
    SOCKADDR_IN _recv_addr = { 0 };
    uint8_t _buffer[UDPSERVER_MAX_FRAME_SIZE];

public:
    udpserver(const char * bind_addr="", uint16_t bind_port = 0)
    {        
        _send_addr.sin_family = AF_INET;
		_send_addr.sin_port = htons(0);
		_send_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

		_recv_addr.sin_family = AF_INET;
		_recv_addr.sin_port = htons(bind_port);
        if (bind_addr[0] == '\0')
            _recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        else
		    _recv_addr.sin_addr.s_addr = inet_addr(bind_addr);

        if (WSAStartup(MAKEWORD(2, 2), &_wsa_data) != 0)
        {
            LOG_ERROR("WSAStartup failed with error {}", WSAGetLastError());
            fprintf(stderr, "winsock version not supported.");
            exit(1);
        }
    }

    void connect(const char* addr, uint16_t port)
    {
        _send_addr.sin_family = AF_INET;
		_send_addr.sin_port = htons(port);
		_send_addr.sin_addr.s_addr = inet_addr(addr);
        _safe_open();
        ::connect(_socket, (SOCKADDR *)&_send_addr, sizeof(_send_addr));
    }

    void send(uint8_t *buf, size_t bufsize)
    {
        send((const char *)buf, bufsize);
    }

    void send(const char*buf, size_t bufsize)
    {
        ::send(_socket, buf, bufsize, 0);
    }

    bool run()
    {
        bool success = _safe_open();

        if ( (success) && (SOCKET_ERROR == bind(_socket, (SOCKADDR *)&_recv_addr, sizeof(_recv_addr))) )
        {
            LOG_ERROR("Error! bind() failed! Error code: {}", WSAGetLastError());
            success = false;
        }

        if (success)
        {
            int len = sizeof(_recv_addr);
            getsockname(_socket, (SOCKADDR *)&_recv_addr, &len);
            LOG_INFO("UDP socket bound,  {}:{}", inet_ntoa(_recv_addr.sin_addr), htons(_recv_addr.sin_port));
        }

        while (success)
        {
            int addr_size = sizeof(_send_addr);
            int received = recvfrom(_socket, (char*)_buffer, sizeof(_buffer), 0, (SOCKADDR *)&_send_addr, &addr_size);
            if (received >= 0)
            {
                if (received==0)
                {
                    LOG_DEBUG("recvfrom() failed with error code: {}", WSAGetLastError());
                }
                else
                {
                    LOG_TRACE("recvfrom() - {} bytes received {}:{}", received, inet_ntoa(_send_addr.sin_addr), htons(_send_addr.sin_port));
                    int sent = sendto(_socket, (const char *)_buffer, received, 0, (SOCKADDR *)&_send_addr, sizeof(_send_addr));
                    if (sent !=received)
                    {
                        LOG_WARN("sendto() only sent {}, attempted {}", sent, received);
                    }
                }
            }
            else 
            { 
                LOG_ERROR("Connection closed with error code: {}", WSAGetLastError());
            }
        }
        _safe_close();
        return success;
    }

    // Signal the run() to stop and exit
    void stop_running()
    { 
        if (INVALID_SOCKET != _socket)
        {
            if (0 != shutdown(_socket, SD_RECEIVE))
            {
                LOG_ERROR("shutdown() failed with error {}.", WSAGetLastError());
            }
        }
    }

    ~udpserver()
    {
        _safe_close();
        if( WSACleanup() != 0)
        {
            LOG_WARNING("WSACleanup() failed! Error code: {}", WSAGetLastError());
        }
    }

 private:

    bool _safe_open()
    {
        bool success = true;
        if (_socket == INVALID_SOCKET)
        {
            _socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (_socket == INVALID_SOCKET)
            {
                LOG_ERROR("Failed to open socket, error: {}", WSAGetLastError());
                success = false;
            }
        }
        return success;
    }

    void _safe_close()
    {
        if (_socket!=INVALID_SOCKET)
        {
            if (closesocket(_socket) != 0)
            {
                LOG_WARNING("closesocket() failed! Error code: {}", WSAGetLastError());
            }
            _socket = INVALID_SOCKET;
        }
    }
};


