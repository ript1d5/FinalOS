// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080
#define MAX 1000
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char hello[] = "Hello from server";

    // Creating socket file descriptor
	//Socket is using IPv4 and TCP. Checks if NULL
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

	//Address is a struct of sockaddr_in and uses IPv4 and the localhost for IP
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
        perror("accept");
        exit(EXIT_FAILURE);
    }

	//New Stuff
	char message[MAX];

	//If user enters "exit" terminate
	while(1){
		valread = read(new_socket, buffer, 1024);
		printf("Other user: %s", buffer);

		printf(">> ");
		fgets(message,MAX, stdin);
		printf("\n");
		if(strcmp(message, "exit") == 0){
			break;
		}
		send(new_socket, message, strlen(message), 0);
		memset(message, 0, sizeof message);
		memset(buffer, 0, 1024);
	}

    return 0;
}
