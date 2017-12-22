//import libraries
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <vector>
#include <time.h>

//setting win library
#pragma comment (lib, "Ws2_32.lib")
//ignore errors about time stuff
#pragma warning (disable:4996)

#define IP_ADDRESS "172.24.221.117" //ip-address - change in correspondance to your pc
#define DEFAULT_PORT "132" //port - change in correspondance to your pc
#define DEFAULT_BUFLEN 512

//setting up clients
struct client_type {
	int id;
	SOCKET socket;
};

//setting up some more client variables
const char OPTION_VALUE = 1; /*change name*/
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

    std::string msg = "";
    char tm[DEFAULT_BUFLEN] = ""; //tm is short for temporary message
	
	//if the client closes their window it saves who disconnected
	else {
		msg = "Client #" + std::to_string(new_client.id) + " Disconnected\n";
 
                //timestamp
                time(&presentTime);
                timeinfo = localtime(&presentTime);
                printf(asctime(timeinfo));
 
                //print message
                std::cout << msg << std::endl;
 
                closesocket(new_client.socket);
                closesocket(client_array[new_client.id].socket);
                client_array[new_client.id].socket = INVALID_SOCKET;
 
                //broadcast the disconnection message to the other clients
                for (int i = 0; i < maxClients; i++) {
                    if (client_array[i].socket != INVALID_SOCKET)
                        iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
                }
                break;
            }
        }
    } //end while
 
    thread.detach(); //make sure the client gets detached /*review this*/
 
    return 0;
}

int __cdecl main(void)
{
	WSADATA wsaData;
	int iResult;


			//if the client closes their window it saves who disconnected
			else {
				msg = "Client #" + std::to_string(new_client.id) + " Disconnected\n";

				//timestamp
				time(&presentTime);
				timeinfo = localtime(&presentTime);
				printf(asctime(timeinfo));

				//print message
				std::cout << msg << std::endl;

				closesocket(new_client.socket);
				closesocket(client_array[new_client.id].socket);
				client_array[new_client.id].socket = INVALID_SOCKET;

				//broadcast the disconnection message to the other clients
				for (int i = 0; i < maxClients; i++) {
					if (client_array[i].socket != INVALID_SOCKET)
						iResult = send(client_array[i].socket, msg.c_str(), strlen(msg.c_str()), 0);
				}
				break;
			}
		}
	} //end while

	thread.detach(); //make sure the client gets detached /*review this*/

	return 0;
}

int main() {
	WSADATA wsaData; //structure contains information about windows socket implementation
	struct addrinfo hints; //hold host address information
	struct addrinfo *server = NULL;
	SOCKET server_socket = INVALID_SOCKET;
	std::string msg = "";
	std::vector<client_type> client(maxClients);
	int sum_clients = 0;
	int tI = -1; //tI is short for temporary IP
	std::thread my_thread[maxClients];

	//initialize Winsock
	std::cout << "Intializing Winsock..." << std::endl;
	WSAStartup(MAKEWORD(2, 2), &wsaData); //creates WORD value by concatenating specified value

	//setup hints
	ZeroMemory(&hints, sizeof(hints)); //fills memory with 0's to create socket
	hints.ai_family = AF_INET; //determines connection type
	hints.ai_socktype = SOCK_STREAM; //specifies the socket type
	hints.ai_protocol = IPPROTO_TCP; //determines the connection type
	hints.ai_flags = AI_PASSIVE; //for wildcard IP address

	//setup Server
	std::cout << "Setting up server..." << std::endl;
	getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &server);

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

	while (1) {
		SOCKET incoming = INVALID_SOCKET;
		incoming = accept(server_socket, NULL, NULL); //accepts client

		if (incoming == INVALID_SOCKET) continue;

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
		}

		if (tI != -1) {
			//send the id to that client
			time(&presentTime);
			timeinfo = localtime(&presentTime);
			printf(asctime(timeinfo)); //timestamp printed
			std::cout << "Client #" << client[tI].id << " Accepted\n" << std::endl; //prints that the client is accepted
			msg = std::to_string(client[tI].id);
			send(client[tI].socket, msg.c_str(), strlen(msg.c_str()), 0);

			//create a thread process for that client
			my_thread[tI] = std::thread(clientConnector, std::ref(client[tI]), std::ref(client), std::ref(my_thread[tI]));
		}

		else {
			msg = "The server is currently full!";
			send(incoming, msg.c_str(), strlen(msg.c_str()), 0);
			std::cout << msg << std::endl; //prints in server that the server is full
		}
	} //end while

	//close listening socket
	closesocket(server_socket);

	//close client socket
	for (int i = 0; i < maxClients; i++){
		my_thread[i].detach();
		closesocket(client[i].socket);
	}

	system("pause");
	return 0;
}
