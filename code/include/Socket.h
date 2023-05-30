#pragma once

#ifndef __MXT_SOCKET_H__
#define __MXT_SOCKET_H__


#ifndef __BUILD_DLL__ 
#define __DLL_DECL__ __declspec(dllimport)
#else
#define __DLL_DECL__ __declspec(dllexport)
#endif // __DLL_DECL__ 


#include <stdint.h>

#ifndef __DEF_SOCKET__

#define __DEF_SOCKET__

#ifdef _WIN32
#include <WinSock2.h>
#else
#define SOCKET int
#endif // _WIN32

#endif // __DEF_SOCKET__

extern "C"  {

	bool __DLL_DECL__ start_winsock();
	bool __DLL_DECL__ stop_winsock();
	
	SOCKET __DLL_DECL__ init_tcp_client(const char* ip, uint16_t port);
	SOCKET __DLL_DECL__ init_tcp_server(const char* ip, uint16_t port);
	
	SOCKET __DLL_DECL__ listen_one_client(const char* ip, uint16_t port);
	long __DLL_DECL__ listen_clients(const char* ip, uint16_t port, void (*cb)(SOCKET));
	
	bool __DLL_DECL__ read_from_socket(SOCKET sock, char* buffer, size_t& size);
	bool __DLL_DECL__ write_to_socket(SOCKET sock, const char* buffer, size_t size);
}


#endif // __MXT_SOCKET_H__