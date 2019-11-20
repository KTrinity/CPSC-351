#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <WinSock2.h>
#include <ws2tcpip.h>
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::thread;

#pragma comment(lib, "Ws2_32.lib")

#define SEND_PORT	3514
#define RECEIVE_PORT    3515
#define BUFFER	256
#define IP      "127.0.0.1"

void sockSend() {
	while (true) {
		sockaddr_in destAddr, srcAddr;
		int start;
		SOCKET sockSend;

		WSADATA wsaData;

		string input;
		char message[BUFFER];
		getline(cin, input);
		strcpy_s(message, input.c_str());

		if (input.length() <= BUFFER) {

			start = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (start != 0) {
				cout << "WSAStartup Failed With Error: " << start << endl;
				exit(1);
			}

			if ((sockSend = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
				perror("socket");
				exit(1);
			}

			destAddr.sin_family = AF_INET;
			inet_pton(AF_INET, IP, &destAddr.sin_addr.s_addr);
			destAddr.sin_port = htons(SEND_PORT);

			srcAddr.sin_family = AF_INET;
			inet_pton(AF_INET, IP, &srcAddr.sin_addr.s_addr);
			srcAddr.sin_port = htons(RECEIVE_PORT);

			if (bind(sockSend, (struct sockaddr*) & srcAddr, sizeof(srcAddr)) < 0) {
				perror("send bind");
				exit(1);
			}

			if (sendto(sockSend, message, strlen(message) + 1, 0, (struct sockaddr*) & destAddr, sizeof(destAddr)) < 0) {
				perror("sendto");
				exit(1);
			}

			start = closesocket(sockSend);
		}
		else {
			cout << "ERROR: MESSAGE IS TOO LONG\n";
		}
	}
}

void sockReceive() {
	while (true) {
		sockaddr_in destAddr, srcAddr;
		socklen_t srcLen = sizeof(srcAddr);
		int start;
		SOCKET sockReceive;
		char received[BUFFER];
		WSADATA wsaData;

		start = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (start != 0) {
			cout << "WSAStartup Failed With Error: " << start << endl;
			exit(1);
		}

		if ((sockReceive = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("socket");
			exit(1);
		}

		memset(&srcAddr, 0, sizeof(srcAddr));
		srcAddr.sin_family = AF_INET;
		inet_pton(AF_INET, IP, &destAddr.sin_addr.s_addr);
		srcAddr.sin_port = htons(RECEIVE_PORT);

		if (bind(sockReceive, (struct sockaddr*) & srcAddr, sizeof(srcAddr)) < 0) {
			perror("receive bind");
			exit(1);
		}

		if (recvfrom(sockReceive, received, sizeof(received), 0, (struct sockaddr*) & srcAddr, &srcLen) < 0) {
			perror("recvfrom");
			exit(1);
		}

		cout << "Received a message: " << received << endl;

		start = closesocket(sockReceive);
	}
}

int main(int argc, char* argv[]) {
		thread receive(sockReceive);
		thread send(sockSend);
	
		send.join();
		receive.join();	

	return 0;
}