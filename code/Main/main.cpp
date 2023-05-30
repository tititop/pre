#include <iostream>
#include <thread>
#include <chrono>

#include <WinSock2.h>
#include <Windows.h>

#include "Socket.h"

#pragma comment(lib, "Net.lib")
#pragma comment(lib, "ws2_32")

void server_run() {
	const size_t nExit = 3;
	const size_t buff_size = 1024 * 40;

	char* tmp_buffer = (char*)malloc(buff_size);
	memset(tmp_buffer, 0, buff_size + 1);

	while (1) {
		std::cout << "waite one client ..." << std::endl;
		SOCKET sock = listen_one_client("0.0.0.0", 8888);
		if (sock == 0 || sock == -1) {
			std::cout << "listen faild" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			continue;
		}
		bool read_rt = true;
		bool write_rt = true;
		size_t nZero = 0;

		while (1) {
			size_t nRead = buff_size;
			read_rt = read_from_socket(sock, tmp_buffer, nRead);
			if (!read_rt) {
				std::cout << "read faild" << std::endl;
				closesocket(sock);
				break;
			}
			if (nRead == 0) {
				nZero++;
				if (nZero < nExit) {
				}
				else {
					std::cout << "readExit" << std::endl;
					closesocket(sock);
					break;
				}
			}
			tmp_buffer[nRead] = '\0';
			std::cout << "nRead  = " << nRead << ", content = " << std::string(tmp_buffer, nRead) << std::endl;
			write_rt = write_to_socket(sock, tmp_buffer, nRead);
			if (!write_rt) {
				std::cout << "write faild" << std::endl;
				closesocket(sock);
				break;
			}
			else {
				std::cout << "nWrite = " << nRead << ", content = " << std::string(tmp_buffer, nRead) << std::endl;
			}
		}
	}

}

void client_run() {

	while (1) {
		const std::string remote_ip = "127.0.0.1";
		const uint16_t remote_port = 8888;

		std::string local_ip = "127.0.0.1";
		uint16_t local_port = 8888;

		const std::string send_str = "hello server!";
		const size_t send_size = send_str.size();

		const size_t read_size = 1024 * 40;
		char* read_buffer = (char*)malloc(read_size + 1);

		const size_t nExit = 3;
		size_t nZero = 0;

		SOCKET sock = init_tcp_client(remote_ip.c_str(), remote_port);
		if (sock == -1) {
			continue;
		}
		else {
			sockaddr_in sin;
			int sin_len = sizeof(sin);
			getpeername(sock, (sockaddr*)&sin, &sin_len);
			local_ip = inet_ntoa(sin.sin_addr);
			local_port = sin.sin_port;
			printf("connected: remote_ip = %s, remote_port = %d, local_ip = %s, local_port = %d\n"
				, remote_ip.c_str(), remote_port
				, local_ip.c_str(), local_port);
		}
		while (1) {
			size_t nSend = send_size;

			bool send_rt = write_to_socket(sock, send_str.c_str(), nSend);
			if (send_rt) {
				std::cout << "nWrite = " << nSend << ", content = " << std::string(send_str) << std::endl;
			}
			else {
				std::cout << "Write error, err code = " << WSAGetLastError() << std::endl;
				closesocket(sock);
				break;
			}

			size_t nRead = read_size;
			memset(read_buffer, 0, read_size + 1);
			bool recv_rt = read_from_socket(sock, read_buffer, nRead);
			if (recv_rt) {
				if (nRead == 0) {
					nZero++;
					if (nZero < nExit) {
					}
					else {
						std::cout << "readExit" << std::endl;
						closesocket(sock);
						break;
					}
				}
			}
			else {
				std::cout << "Write error, err code = " << WSAGetLastError() << std::endl;
				closesocket(sock);
				break;
			}
			read_buffer[read_size] = '\0';
			std::cout << "nRead  = " << nRead << ", content = " << std::string(read_buffer, nRead) << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}

}

int main() {
	// dll 放在 exe 同文件夹下无问题, 调试 -> 环境设置runtime路径会有问题, 10106
	// 怀疑破坏了运行环境
	start_winsock();
	server_run();
	// client_run();
	stop_winsock();

	return 0;
}