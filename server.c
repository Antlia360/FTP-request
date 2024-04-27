#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/file.h>
//include necessary header file

void *client_manager(void *arg) {
    int client_socket = *(int *)arg;
    int n;
    char filename[1024];
 	char command[1024];


        // Clear the command and read the client's command
        bzero(command, 1024);
        bzero(filename, 1024);
        recv(client_socket, command, sizeof(command), 0);
        printf("%s\n",command);
        if (strcmp(command, "put") == 0) {
			bzero(filename, 1024);
			recv(client_socket, filename, sizeof(filename), 0);

			FILE *file = fopen(filename, "wb");
			if (file == NULL) {
				send(client_socket, "Could not open file\n", 19, 0);
				//break;
			}
			int bytes_received;
			bzero(command, 1024);
			while ((bytes_received = recv(client_socket, command, sizeof(command), 0)) > 0) {
				//fwrite(command, 1, sizeof(command), file);bytes_received
				fwrite(command, 1, bytes_received, file);bytes_received
			}

			fclose(file);
			
		} else if (strcmp(command, "get") == 0) {
		  recv(client_socket, filename, sizeof(filename), 0);
			
			// Send a file to the client
			FILE *file = fopen(filename, "rb");
			if (file == NULL) {
				send(client_socket, "Could not open file\n", 19, 0);
				return;
			}

			int bytes_sent;
			while ((bytes_sent = fread(command, 1, sizeof(command), file)) > 0) {
				send(client_socket, command, sizeof(command), 0);
			}

			fclose(file);
			
		} else if (strcmp(command, "close") == 0) {
			close(client_socket);
			printf("Client connection closed\n");
			//break;
		} else if (strcmp(command, "cd") == 0) {
			char new_dir[100];
			recv(client_socket, new_dir, sizeof(new_dir), 0);

			// Change the current directory on the server
			int ret = chdir(new_dir);
			if (ret != 0) {
				send(client_socket, "Could not change directory\n", 25, 0);
				return;
			}

			// success message 
			strcpy(command,"Directory changed successfully");
			send(client_socket, command, sizeof(command), 0);
		} else if (strcmp(command, "ls") == 0) {
			// List files in the server's current directory
			FILE *ls = popen("ls -l", "r");
			int bytes_sent;
			while ((bytes_sent = fread(command, 1, sizeof(command), ls)) > 0) {
				send(client_socket, command, sizeof(command), 0);				
			}
			pclose(ls);
		} else {
			printf("Invalid command\n");
		}
    
    close(client_socket);// close client socket
    pthread_exit(NULL);//exit thread
}


//main function
int main() {

//define the required variables
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    int n;

	//create tcp socket->returns a sock descriptor
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[-]Socket error");
        exit(EXIT_FAILURE);
    }
    printf("[+] TCP server socket created.\n");

	//initialize the server
	//sockaddr_in struct -> server keeps all address information for both the server and the client in it
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(5566);
    server_addr.sin_addr.s_addr = INADDR_ANY;

	//bind the socket descriptorwith server address information
    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("[-] Bind error");
        exit(EXIT_FAILURE);
    }
    printf("[+] Bind to the port number: %d\n", ntohs(server_addr.sin_port));

	//listening the incomming connection from client
    listen(server_sock, 5);
    printf("Listening...\n");

//handels  incoming client connections.
    while (1) {
        addr_size = sizeof(client_addr); //size of client address
        //accept the client conn and assigns to client_sock
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size); 
        printf("[+] Client connected.\n");//connection established

        pthread_t tid;
        //creates thread to handel the new client using client_manager
        //if it fails, close the connection
        //
        if (pthread_create(&tid, NULL, client_manager, &client_sock) != 0) {
            perror("Error creating thread"); 
            close(client_sock);
        } else {
            pthread_detach(tid); //if thread creation is successful the detach and runs independently
        }
    }

    close(server_sock);//closeing the socket
    return 0;
}


