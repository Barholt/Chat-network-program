#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

//setting up clients
struct client_type {
    int id;
    SOCKET socket;
};

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

//setting up some more client variables
const char OPTION_VALUE = 1;
const int maxClients = 5;

//setting up time
time_t presentTime;
struct tm * timeinfo;

//starting up the client message construction
int clientConnector(client_type &new_client, std::vector<client_type> &client_array, std::thread &thread) {
	std::string msg = "";
    	char tm[DEFAULT_BUFLEN] = ""; //tm is short for temporary message
	
	//sending and receiving messages
	while (1) {
		memset(tm, 0, DEFAULT_BUFLEN); //memset: set bytes in memory
		if (new_client.socket != 0)	{
			int iResult = recv(new_client.socket, tm, DEFAULT_BUFLEN, 0);

			//if there's no apparent errors, start showing messages
			if (iResult != SOCKET_ERROR) {
				if (strcmp("", tm)) {
					msg = "Client #" + std::to_string(new_client.id) + ": " + tm + "\n";
				}

				//timestamp
				time(&presentTime);
				timeinfo = localtime(&presentTime);
				printf(asctime(timeinfo));

				//print message
				std::cout << msg.c_str() << std::endl;

				//broadcast that message to the other clients
				for (int i = 0; i < maxClients; i++) {
					if (client_array[i].socket != INVALID_SOCKET) {
						if (new_client.id != i) {
							iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
						}
					}
				}
			}
	
	thread.detach(); //make sure the client gets detached /*review this*/
 
    return 0;
}

int __cdecl main(void)
{
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	Zeromory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
 //setup Server
    std::cout << "Setting up server..." << std::endl;
	
//create a listening socket for connecting to server
	std::cout << "Creating server socket..." << std::endl;
	server_socket = socket(server->ai_family, server->ai_socktype, server->ai_protocol);

	//setup socket options
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &OPTION_VALUE, sizeof(int)); //Make it possible to re-bind to a port that was used within the last 2 minutes
	setsockopt(server_socket, IPPROTO_TCP, TCP_NODELAY, &OPTION_VALUE, sizeof(int)); //Used for interactive programs

	//assign an address to the server socket.
	std::cout << "Binding socket..." << std::endl;
	bind(server_socket, server->ai_addr, (int)server->ai_addrlen);
	
	//listen for incoming connections.
   	std::cout << "Listening...\n" << std::endl;
    	listen(server_socket, SOMAXCONN);
 
   	//initialize client list
    	for (int i = 0; i < maxClients; i++) {
        client[i] = { -1, INVALID_SOCKET };
    }
	
    getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &server);
	
	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	  while (1) {
        SOCKET incoming = INVALID_SOCKET;
        incoming = accept(server_socket, NULL, NULL); //accepts client
		  
	//reset the number of clients
		sum_clients = -1;

		//assign ID to client
		tI = -1;
		for (int i = 0; i < maxClients; i++) {
			if (client[i].socket == INVALID_SOCKET && tI == -1) {
				client[i].socket = incoming;
				client[i].id = i;
				tI = i;
			}

			if (client[i].socket != INVALID_SOCKET) {
				sum_clients++; //number of clients raised by 1
			}
		  
		  else {
			  msg = "The server is currently full!";
			  send(incoming, msg.c_str(), strlen(msg.c_str()), 0);
			  std::cout << msg << std::endl; //prints in server that the server is full
		  }
		  
	  }
	// No longer need server socket
	closesocket(ListenSocket);

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ClientSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else  {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return 1;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();

	return 0;
}

