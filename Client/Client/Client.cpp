//Includes
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <thread>
#include <cstdlib>
#include <time.h>

//Namespaces
using namespace std;

//Pragmas
#pragma comment (lib, "Ws2_32.lib")
#pragma warning (disable:4996)

//Defines
#define DEFAULT_IP "172.24.218.249" 	//Change in accordance to your PC
#define DEFAULT_PORT "132"		//Change in accordance to your PC
#define DEFAULT_BUFLEN 512

struct client_type {
	SOCKET socket;
	int id;
	char received_message[DEFAULT_BUFLEN];
};









int main() {
	WSAData wsa_data; //Structure contains information about windows socket implementation
	struct addrinfo *result = NULL, *ptr = NULL, hints; //Structure holds host address information
	string = sent_message = ""; //These quotationmarks marks the messages that are being sent
	client_type client = { INVALID_SOCKET, -1, ""}; //Calls a previous struct, and sets variables 
	int iResult = 0;
	string message;
	
	cout << "Starting program...\n";
	
	//Initializing the Winsocket
	iResult = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (iResult != 0) {
		cout << "Start-up failed with the following error: " << iRestult << endl;
	}
	
	ZeroMemory(&hints, sizeof(hints)); //Fills memory with 0's to create the socket
	hints.ai_family = AF_UNSPEC; //Returns socket address for any address family
	hints.ai_socktype = SOCK_STREAM; //Specifies the socket type
	hints.ai_protocol = IPPROTO_TCP; //Determines the connection type
	
	cout << "Connecting to server... \n";
	
	//Resolving the server address and server port
	iResult == getaddrinfo(static_cast<LPCTSTR>(IP_ADDRESS), DEFAULT_PORT, &hints, &result);
	if (iResult != 0) { //If iResult is not 0, it is an error
		cout << "Failed to get address with the following error: " << iResult << endl;
		WSACleanup(); //Terminates the use of Ws2_32.dll, and frees resources
		system("pause");
		return 1;
	}
	
	//Attempting to connect to an address until succeeded
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		//Creating a socket for connecting to the server
		client.socket = socket(ptr->ai_family, ptr->ai:socktype, ptr->ai_protocol);
		if (client.socket == INVALID SOCKET) {
			cout << "The socket failed with the following error: " << WSAGetLastError() << endl;
			WSACleanup(); //Terminates the use of Ws2_32.dll, and frees resources
			system("pause");
			return 1;
		}
	
		//Connecting to server
		iResult = connect(client.socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) { //If socket error occours, it is invalid
			closesocket(client.socket);
			client.socket = INVALID_SOCKET;
			continue;
		}
	break;
	}
freeaddrinfo(result); //Frees address information

if (client.socket == INVALID_SOCKET) { //If client.socket is invalid, then no connection to the server is made
	cout << "Unable to connect to server!" << endl;
	WSACleanup(); //Terminates the use of Ws2_32.dll, and frees resources
	system("pause");
	return 1;
}
//If no errors are encountered, then the connection to the server is successful
	cout << "Successfully connected to server!\nTo close the program, type 'QUIT'\n\nMe:" << endl;
	
//Obtaining the ID from the server for the client
recv(client.socket, client.received_message, DEFAULT_BUFLEN, 0);
message = client.received_message;

		
		
		
		
		
		
		
		
		
		
		
		
		
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// Validate the parameters
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}
