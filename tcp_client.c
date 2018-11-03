#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
#include<arpa/inet.h>
#include <errno.h>

#define MAX  1000
#define PORT_NUM 7891



void delay();
void run_client();



int main(int argc, char **argv) {
	if(argc != 2) {
		printf("Usage: /{filename} {server_ip}\n");
		return -1;
	} 
	char * ip_addr = argv[1];

	run_client(ip_addr);
	return 0;
}



void delay(int number_of_seconds){
	// Converting time into milli_seconds
	int milli_seconds = 1000 * number_of_seconds;

	// Stroing start time
	clock_t start_time = clock();

	// looping till required time is not acheived
	while (clock() < start_time + milli_seconds);
}



void run_client(const char * ip_addr){
	char received[MAX], to_send[MAX], username[MAX];
	int net_socket, conn_stat, valread;	
	struct sockaddr_in server_address;
	socklen_t  address_size;

	if((net_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("Socket Failed");
		exit(EXIT_FAILURE);
	}

	//Server address is IPv4 and port is PORT_NUM
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT_NUM);	
	server_address.sin_addr.s_addr = inet_addr(ip_addr);	

	// address_size = sizeof server_address;
	if(connect(net_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1){
		printf("There was an error connecting to the socket\n");
		printf("ERRNO %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	// Receiving welcome message
	recv(net_socket, received, sizeof(received),0);
	printf("%s", received);
	memset(received, 0, sizeof(received));
	
	// Input message
	printf("%s: ", username);
	memset(to_send, 0, sizeof(to_send));
	fgets(to_send, MAX, stdin);
	printf("\n");
	
	//Sending message to server
	if(send(net_socket, to_send, strlen(to_send), 0) < 0) {
		printf("Error: send() failed\n");
		exit(EXIT_FAILURE);
	}

	// Receive reply and terminate
	if(recv(net_socket, received, sizeof(received), 0) < 0) {
		printf("Error: recv() failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Server: %s\n", received);

	return;
}
