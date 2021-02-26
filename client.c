/* 	Computer Network Assignment 1
	
	Name 	- PRASHANT
	RollNo. - 2018360

	This is the Client Side code,
	The details of a code is described by the commment as 
	followed by the line by line code 

	Here the instruction for Running the code is:-

	This code takes three argument at the time of Running of code 
	1. Executable file name or may be called as output file name
	2. IP address of the server (if we are working on same machine then 
	   enter loopback ip address which is 127.0.0.1 otherwise for different machines 
	   then enter the server's ip address)
	3. PORT NO. (Just to be make sure that PORT NO. is same as the Server's PORT NO.)

	Compiling and Running of an code by example:
	gcc client.c -o cl         		--- Compiling
	./cl 127.0.0.1 9898             --- Running 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>




//function where all the error message is sent and 
//then call the standard error function perror
void error(const char *message) {
	perror(message);
	exit(1);
}


int main(int argc, char const *argv[])
{
	int clientSockfd;
	int port;
	struct sockaddr_in client_addr;


	//check that Client must provide three argument as mentioned above
	if(argc < 3) {
		printf("Less no. of argument\n");
		printf("Format of input : Output_file Server_IPaddress PORTNO.\n");
		return 0;
	}


	//initialize client structure as zero
	bzero((char *) &client_addr, sizeof(client_addr));

	//creating a socket of type TCP
	clientSockfd = socket(AF_INET, SOCK_STREAM, 0);

	//check that clientsockfd value is lesser than zero
	//if it is yes, then there is something wrong for creating the socket
	if(clientSockfd < 0) {
		error("Socket creation Failed !!");
	}

	port = atoi(argv[2]);		//extracting the port no. from string

	//initialize the server_addr structure
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(port);
	int ip = inet_pton(AF_INET, argv[1], &client_addr.sin_addr.s_addr);

	//check that ip address intialization successful or not
	if( ip < 0) {
		error("IP address initializaton Failed !!");
	}

	//connect the client socket to the server socket
	int cont = connect(clientSockfd, (struct sockaddr *) &client_addr, sizeof(client_addr));
	printf("Initiating Connection to the server...........\n");
	if(cont < 0) {
		error("Connection to the server Failed !!");
	}
	printf("Successfully connected to the server\n");

	


	char *filename;
	filename = (char *)malloc(sizeof(char)*100);

	bzero(filename, 100);

	//take input as filename
	printf("Enter the filename which you want to download : ");
	scanf("%[^\n]s", filename);

	//sending filename to the server
	int s = send(clientSockfd, filename, 100, 0);
	if(s < 0) {
		error("Sending filename Failed !!");
	}
	printf("Successfully Send filename\n");





	long fileSize;
	//just receiving the filesize for allocating the memory for content variable
	recv(clientSockfd, &fileSize, sizeof(fileSize), 0);

	char *content = malloc(fileSize + 1);

	//create the file as a input filename and open in as a binary write mode
	FILE *fwriteptr = fopen(filename, "wb");
	
	bool temp = false;
	int i=0;
	//receiving the content of file in binary form and simultaneously write the data in file
	while((recv(clientSockfd, &content[i], sizeof(content[i]), 0)) > 0) {
		fwrite(&content[i], sizeof(content[i]), 1, fwriteptr);
		i++;
		temp = true;
	}

	//check that if file successfully received or not
	if(temp) {
		printf("File Successfully Saved in Client Directory\n");
	}
	else {
		//Here when no such file in the server directory
		//remove the file which we were created as a binary write mode in client directory
		remove(filename);	
		printf("File not found in Server Directory\n");
	}

	printf("Connection Closed from server\n");

	fclose(fwriteptr);
	close(clientSockfd);
	return 0;
}
