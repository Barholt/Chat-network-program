//import libraries
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <cstdlib>
#include <time.h>

using namespace std; //without this you'd have to write std::cout <<;

#pragma comment (lib, "Ws2_32.lib")
#pragma warning (disable:4996)

           
#define IP_ADDRESS "172.24.218.249" //change in accordance to your pc
#define DEFAULT_PORT "132" //change in accordance to your pc
#define DEFAULT_BUFLEN 512 	
					 //create socket and id for sending and receiving information
struct client_type {
	SOCKET socket;
	int id;
	char received_message[DEFAULT_BUFLEN];
};

//setting up time
time_t presentTime;
struct tm * timeinfo;

int clientConnector(client_type &new_client);
int main();
//Connects client to the server via sockets
int clientConnector(client_type &new_client) {
	while (1) {
		memset(new_client.received_message, 0, DEFAULT_BUFLEN);
		//make sure the client send messages
		if (new_client.socket != 0) {
			int iResult = recv(new_client.socket, new_client.received_message, DEFAULT_BUFLEN, 0);

			if (iResult != SOCKET_ERROR) {
				//timestamp
				time(&presentTime);
				timeinfo = localtime(&presentTime);
				printf(asctime(timeinfo));
				cout << new_client.received_message << endl;
			}
			//if client loses connection this prints out
			else {
				cout << "Receiving Client information failed: " << WSAGetLastError() << endl;
				break;
			}
		}
	}

	if (WSAGetLastError() == WSAECONNRESET)
		cout << "The server has been disconnected" << endl;

	return 0;
}

int main() {
	WSAData wsa_data; //structure contains information about windows socket implementation
	struct addrinfo *result = NULL, *ptr = NULL, hints; //structure holds host address information
	string sent_message = ""; //these quotationmarks marks messages that are being send
	client_type client = { INVALID_SOCKET, -1, "" }; //calls a previous struct, and sets variables
	int iResult = 0;
	string message;

	cout << "Starting program...\n";


	//initializing the winsocket
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (iResult != 0) {
		cout << "Start-up failed with the following error: " << iResult << endl;
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints)); //fills memory with 0's to create socket
	hints.ai_family = AF_UNSPEC; //returns socket address for any address family
	hints.ai_socktype = SOCK_STREAM; //specifies the socket type
	hints.ai_protocol = IPPROTO_TCP; //determines the connection type

	cout << "Connecting to server...\n";

	//resolving the server address and server port
	iResult = getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) { //if iResult is NOT 0, it's an error
		cout << "Failed to get address with the following error: " << iResult << endl;
		WSACleanup(); //terminates the use of Ws2_32.dll, and frees resources
		system("pause");
		return 1;
	}

	//attempting to connect to an address until it is succeeded
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Creating a SOCKET for connecting to the server
		client.socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (client.socket == INVALID_SOCKET) {
			cout << "The socket failed with following error: " << WSAGetLastError() << endl;
			WSACleanup(); //terminates the use of Ws2_32.dll, and frees resources
			system("pause");
			return 1;
		}

		// Connecting to the server
		iResult = connect(client.socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) { //if socket error occours, it is invalid
			closesocket(client.socket);
			client.socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result); //frees address information

	if (client.socket == INVALID_SOCKET) { //if client.socket is invalid, then no connection to server is made
		cout << "Unable to connect to the server!" << endl;
		WSACleanup(); //terminates the use of Ws2_32.dll, and frees resources
		system("pause");
		return 1;
	}
	//if no errors encountered, then connection to server is successful
	cout << "Successfully connected to the server!\nTo end program type in 'QUIT'\n\nMe:" << endl;


	//obtain id from server for this client;
	recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
	message = client.received_message;

	if (message != "The server is currently full!") { //if server is not full, client can connect
		client.id = atoi(client.received_message); /*not sure this happens in cmd*/

		thread my_thread(clientConnector, client);

		while (1) {
			getline(cin, sent_message); //promts input and saves it
			iResult = send(client.socket, sent_message.c_str(), strlen(sent_message.c_str()), 0); //sends the saved input

			cout << "\nMe: " << endl;

			if (sent_message == "QUIT") {
				exit(1);
			}
		}

		//shutdown the connection since no more data will be sent
		my_thread.detach();
	}
	else
		cout << client.received_message << endl;

	cout << "Shutting down the socket..." << endl; /*not sure this happens in cmd*/
	iResult = shutdown(client.socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		cout << "Shutting down failed with the following error: " << WSAGetLastError() << endl; //returns error status from the last failed windows sockets
		closesocket(client.socket); //closes socket
		WSACleanup(); //terminates the use of Ws2_32.dll, and frees resources
		system("pause");
		return 1;
	}

	closesocket(client.socket);
	WSACleanup(); //terminates the use of Ws2_32.dll, and frees resources
	system("pause");
	return 0;
}
