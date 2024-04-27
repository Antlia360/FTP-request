# FTP Server and Client Implementation

This project involves implementing a File Transfer Protocol (FTP) server and client in the C programming language using Transmission Control Protocol (TCP). The FTP system supports basic file transfer operations, including put, get, close, cd (change directory), and ls (list directory contents) from the client to the server.

-  put <file_name>: Upload a file from the client to the server in the current directory.
-  get <file_name>: Download a file from the server to the client in the current directory.
-  close: Close the connection with the client.
-  cd <directory_name>: Change the current directory on the server.
-  ls: List all the contents of the current directory on the server available to the client.

## Usage:
1. Compile the server and client programs separately.
2. Run the server program on a host machine with the specified port.
3. Run the client program on another host machine and provide the server's IP address and port.
4. Use the client command-line interface to interact with the FTP server by entering commands.

for server-
 - Compile : gcc server.c -o server -lpthread\
 - run : ./server
  
for client side-
 - Compile : gcc client.c -o client\
 - run : ./client <IP ADDRESS> <PORT>

## Example Commands:
- To upload a file from the client to the server: put <file_name>
- To download a file from the server to the client: get <file_name>
- To close the connection with the server: close
- To change the current directory on the server: cd <directory_name>
- To list all contents of the current directory on the server: ls
