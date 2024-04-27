#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/socket.h>
//include the necessary libraries

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <Server IP> <Port> \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    //parsing
   // char *ip = argv[1];  //ip address
    //int num_req = atoi(argv[2]); // no of ping req
    //int interval = atoi(argv[3]); // no of intervals
   // int port = atoi(argv[2]);  // port number
    //char *command = argv[3];    //int port = 5566;


	//variables declaration 
	int client_socket;
    

    while (1) {
    
        struct sockaddr_in server_addr;
    char buffer[1024];
    int n;
    

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("[-] Socket error");
        exit(EXIT_FAILURE);
    }
    
    printf("[+]TCP server socket created.\n");
 
	memset(&server_addr, '\0', sizeof(server_addr));

	//ip address and port number
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

//connection to the server
		//connection establishment with the IP provided by user and port.
		if (connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		    perror("[-] Connection failed"); 
		    exit(EXIT_FAILURE);  // if not established
		}
		printf("[+] Connected to the server.\n");

    // Send command to the server
   // char command[100];
   // strcpy(command, argv[3]);

	//char response[1024];
   int bytes_received;
 char filename[1024];
 char command[1024];
    
    	bzero(command, 1024);
    	//char *command;
        printf("ftp_client> ");
        //scanf("%s ", command);
        //char *command = argv[3]
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';


        if (send(client_socket, command, sizeof(command), 0) < 0) {
            perror("Error sending command");
            close(client_socket);
            exit(EXIT_FAILURE);
        }
        
        if (strcmp(command, "put") == 0) {
            printf("Enter the file name to upload: ");
            bzero(filename, 1024);
            fgets(filename, 1024, stdin);
            filename[strcspn(filename, "\n")] = '\0';

            send(client_socket, filename, strlen(filename), 0);

            FILE *file = fopen(filename, "rb");
            if (file == NULL) {
                printf("File not found\n");
                continue;
            }
		int bytes_read;

            bzero(buffer, 1024);
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0){
                //size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);
                send(client_socket, buffer, sizeof(buffer), 0);
            }

            fclose(file);
            printf("Upload sucessful\n");
        } else if (strcmp(command, "get") == 0) {
            printf("Enter the file name to download: ");
            fgets(filename, 1024, stdin);
            filename[strcspn(filename, "\n")] = '\0';
            send(client_socket, filename, strlen(filename), 0);

            FILE *file = fopen(filename, "wb");
            if (file == NULL) {
                printf("Error creating the file\n");
                continue;
            }

            while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
                fwrite(buffer, 1, bytes_received, file);
            }

            fclose(file);
            printf("Download done sucessfully\n");
        } else if (strcmp(command, "cd") == 0) {
             printf("Enter the file location: ");
            fgets(filename, 256, stdin);
            filename[strcspn(filename, "\n")] = '\0';
            send(client_socket, filename, strlen(filename), 0);
            recv(client_socket, buffer, sizeof(buffer), 0);
            printf("%s\n",buffer);
        } else if(strcmp(command, "ls")==0){
        	int bytes_sent;
        	
        	while ((bytes_sent = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
                printf("%s", buffer);
            }

        }
        else if (strcmp(command, "close") == 0) {
            printf("Connection closed by client\n");
            break;
        } else {
            printf("Invalid command\n");
        }
    }
    

    close(client_socket);
    return 0;
}


