#include "udp.h"
#include "log.h"

#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")


static WSADATA udp_wsa_data;

static SOCKET udp_socket = INVALID_SOCKET;

SOCKADDR_IN send_addr = { 0 };

SOCKADDR_IN recv_addr = { 0 };


bool udp_init()
{
    bool success = true;
    send_addr.sin_family = AF_INET;
    send_addr.sin_port = htons(51000);
    send_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    recv_addr.sin_family = AF_INET;
    recv_addr.sin_port = htons(0);
    recv_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
     
    if( WSAStartup(MAKEWORD(2,2), &udp_wsa_data) != 0)
    {
        LOG_ERROR("WSAStartup failed with error %ld\n", WSAGetLastError());
        success = false;
    }

    if (success)
    {
        udp_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

        if (udp_socket == INVALID_SOCKET){
            LOG_ERROR("Failed to open socket, error: %ld\n", WSAGetLastError());
            success = false;
        }
    }

    if ( (success) && (bind(udp_socket, (SOCKADDR *)&recv_addr, sizeof(recv_addr)) == SOCKET_ERROR) )
    {
        LOG_ERROR("Error! bind() failed!");
        closesocket(udp_socket);
        success = false;
    }

    if (success)
    {
        getsockname(udp_socket, (SOCKADDR *)&recv_addr, (int *)sizeof(recv_addr));
        LOG_INFO("UDP socket open,  %s:%d", inet_ntoa(recv_addr.sin_addr), htons(recv_addr.sin_port));
    }

    return success;
}

void udp_send(const uint8_t *const buf, const size_t len)
{ 
    int sent = sendto(udp_socket, (const char*)buf, len, 0, (SOCKADDR *)&send_addr, sizeof(send_addr));
    if (sent !=len)
    {
        LOG_WARN("sendto() only sent %d, attempted %d", buf, len);
    }
}

size_t udp_recv(uint8_t *const buf, const size_t max_len)
{
    int addr_size = sizeof(send_addr);
    int received = recvfrom(udp_socket, (char*)buf, max_len, 0, (SOCKADDR *)&send_addr, &addr_size);
    if (received >= 0)
    {
        if (received==0)
        {
            LOG_DEBUG("recvfrom() failed with error code: %d\n", WSAGetLastError());
        }
        else
        {
            LOG_TRACE("recvfrom() received %s:%d", inet_ntoa(send_addr.sin_addr), htons(send_addr.sin_port));
        }
        return received;
    }
    else 
    { 
        LOG_ERROR("Connection closed with error code: %ld\n", WSAGetLastError());
    }
    return 0;
}

void udp_deinit(void)
{
    if (udp_socket!=INVALID_SOCKET)
    {
        if (closesocket(udp_socket) != 0)
        {
            LOG_WARNING("closesocket() failed! Error code: %ld\n", WSAGetLastError());

        }
    }
    if(WSACleanup() != 0)
    {
        LOG_WARNING("WSACleanup() failed! Error code: %ld\n", WSAGetLastError());
    }
}
