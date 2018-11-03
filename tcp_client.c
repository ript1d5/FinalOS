#pragma comment(lib, "Ws2_32.lib")

#include<stdio.h>
#include<stdlib.h>
//#include<sys/types.h>
//#include<unistd.h>
#include <Ws2tcpip.h>
#include<time.h>
#include<Winsock2.h>

//Needs to be converted to linux

#define MAX 1000

void delay(int number_of_seconds)
{
	// Converting time into milli_seconds
	int milli_seconds = 1000 * number_of_seconds;

	// Stroing start time
	clock_t start_time = clock();

	// looping till required time is not acheived
	while (clock() < start_time + milli_seconds);
}


int main() {
	WSADATA mywsadata;
	WSAStartup(0x0202, &mywsadata);

	char recieved[MAX], to_send[MAX], username[MAX];

	int net_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9005);
	InetPton(AF_INET, "192.168.1.209", &server_address.sin_addr.s_addr);

	int conn_stat = connect(net_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	if (conn_stat == -1) {
		printf("There was an error connecting to the socket\n");
	}

	//Gets initial messages from the server
		recv(net_socket, recieved, sizeof(recieved), 0);
		printf("%s", recieved);
		memset(recieved, 0, sizeof(recieved));

		recv(net_socket, recieved, sizeof(recieved), 0);
		printf("%s", recieved);
		memset(recieved, 0, sizeof(recieved));

		fgets(username, MAX, stdin);
		strtok(username, "\n");
		send(net_socket, username, sizeof(username), 0);

	while (1) {
		recv(net_socket, recieved, sizeof(recieved), 0);
		printf("Server: ");
		printf("%s\n", recieved);
		if (strcmp(recieved, "Server has ended") == 0) {
			delay(5);
			break;
		}
		memset(recieved, 0, sizeof(recieved));

		//Sending message to server
		printf("%s: ", username);
		memset(to_send, 0, sizeof(to_send));
		fgets(to_send, MAX, stdin);
		printf("\n");
		if (strcmp(to_send, "exit\n") == 0) {
			printf("Exiting...");
			send(net_socket, "Client has left", sizeof(username), 0);
			break;
		}
		send(net_socket, to_send, strlen(to_send), 0);
	}

	closesocket(net_socket);
	return 0;

}
