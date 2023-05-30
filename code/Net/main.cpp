#include "Socket.h"

#include <stdlib.h>
#include <iostream>
#include <thread>
#include <chrono>

#pragma comment(lib, "ws2_32")


void server_run() {
	const size_t buff_size = 1024 * 40;
	char* tmp_buffer = (char*)malloc(buff_size);
	memset(tmp_buffer, 0, buff_size + 1);

	while (1) {
		std::cout << "waite one client ..." << std::endl;
		SOCKET sock = listen_one_client("0.0.0.0", 8888);
		if (sock <= 0) {
			std::cout << "listen faild" << std::endl;
		}
		bool read_rt = true;
		bool write_rt = true;
		const size_t nExit = 3;
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
		SOCKET sock = init_tcp_client("127.0.0.1", 8888);
		const std::string send_str = "hello server!";
		const size_t send_size = send_str.size();
		
		const size_t read_size = 1024 * 40;
		char* read_buffer = (char*)malloc(read_size + 1);

		const size_t nExit = 3;
		size_t nZero = 0;

		while (1) {
			size_t nSend = send_size;
			
			bool send_rt = write_to_socket(sock, send_str.c_str(), nSend);
			if (send_rt) {
				std::cout << "nWrite = " << nSend << ", content = " << std::string(send_str) << std::endl;
			} else {
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
					} else {
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

	start_winsock();
	server_run();
	// client_run();
	stop_winsock();

	return 0;
}

