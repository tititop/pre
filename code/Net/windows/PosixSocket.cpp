
#define __BUILD_DLL__
#include "Socket.h"

#ifdef _WIN32


#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>

#define SOCK_ERROR_RETURN(sock, err, rt)                                        \
    if((sock) == (err)) {                                                       \
        std::cout << "sock error, error_code = " << WSAGetLastError()           \
                << " " << __FILE__ << __LINE__ << "\n";                         \
        return (rt);                                                            \
    }                                                                           \
//


bool start_winsock()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error_code: %d\n", WSAGetLastError());
        return false;
    }
    return true;
}

bool stop_winsock()
{
    return WSACleanup();
}

SOCKET init_tcp_server(const char* ip, uint16_t port)
{
    SOCKET ans = 0;
    int ret = 0;
    ans = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCK_ERROR_RETURN(ans, INVALID_SOCKET, -1);
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.S_un.S_addr = inet_addr(ip);
    sin.sin_port = htons(port);

    ret = bind(ans, (sockaddr*)&sin, sizeof(sin));
    SOCK_ERROR_RETURN(ret, INVALID_SOCKET, -1);

    ret = ::listen(ans, 100);
    SOCK_ERROR_RETURN(ans, SOCKET_ERROR, -1);

    return ans;
}

SOCKET listen_one_client(const char* ip, uint16_t port)
{
    SOCKET server = init_tcp_server(ip, port);
    SOCK_ERROR_RETURN(server, SOCKET_ERROR, -1);
    SOCKET client = 0;
    sockaddr_in sin;
    int len = sizeof(sin);

    while (1) {
        client = accept(server, (sockaddr*)&sin, &len);
        SOCK_ERROR_RETURN(client, SOCKET_ERROR, -1);
        if (client != INVALID_SOCKET) {
            printf("accepp client: ip = %s, port = %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
            closesocket(server);
            return client;
        }
    }
}

long listen_clients(const char* ip, uint16_t port, void(*cb)(SOCKET))
{
    long ans = 0;

    SOCKET new_client = 0;
    SOCKET server = init_tcp_server(ip, port);
    SOCK_ERROR_RETURN(server, SOCKET_ERROR, -1);
    sockaddr_in sin;
    int len = sizeof(sin);

    while (1) {
        new_client = accept(server, (sockaddr*)&sin, &len);
        if (new_client > 0) {
            cb(new_client);
            printf("accepp client: ip = %s, port = %s\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
        } else if (new_client == SOCKET_ERROR) {
            printf("accept failed with error_code: %d\n", WSAGetLastError());
            break;
        }
    }

    closesocket(server);
    return ans;
}

SOCKET init_tcp_client(const char* ip, uint16_t port)
{
    SOCKET ans = 0;
    int ret = 0;
    ans = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCK_ERROR_RETURN(ans, INVALID_SOCKET, -1);
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.S_un.S_addr = inet_addr(ip);
    sin.sin_port = htons(port);

    ret = connect(ans, (sockaddr*)&sin, sizeof(sin));
    SOCK_ERROR_RETURN(ret, INVALID_SOCKET, -1);
    return ans;
}

bool read_from_socket(SOCKET sock, char* buffer, size_t & size)
{
    if (sock == SOCKET_ERROR || sock == INVALID_SOCKET) {
        return false;
    }
    size = (size_t)(::recv(sock, buffer, size, 0));
    SOCK_ERROR_RETURN(size, SOCKET_ERROR, false);
    return true;
}

bool write_to_socket(SOCKET sock, const char* buffer, size_t size)
{
    if (sock == SOCKET_ERROR || sock == INVALID_SOCKET) {
        return false;
    }
    size = (size_t)(::send(sock, buffer, size, 0));
    SOCK_ERROR_RETURN(size, SOCKET_ERROR, false);
    return true;
}

#endif // _WIIN32
