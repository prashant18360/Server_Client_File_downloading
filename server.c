/* 	Computer Network Assignment 1
	
	Name 	- PRASHANT
	RollNo. - 2018360

	This is the Server Side code,
	The details of a code is described by the commment as 
	followed by the line by line code 

	Here the instruction for Running the code is:-

	This code takes two argument at the time of Running of code 
	1. Executable file name or may be called as output file name
	2. PORT NO. (always takes greater than 5000)

	Compiling and Running of an code by example:
	gcc server.c -o ser         --- Compiling
	./ser 9898                  --- Running
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	int Sockfd, newSockfd;
	int port;
	struct sockaddr_in server_addr, client_addr;
	socklen_t clientlen;


	//initialize both server and client structure as zero
	bzero((char *) &server_addr, sizeof(server_addr));
	bzero((char *) &client_addr, sizeof(client_addr));

	//check that server must provide two argument as mentioned above
	if (argc < 2)
	{
		printf("Less argument !!");
		printf("Format of argument: filename PORTNO.\n");
		return 0;
	}

	//creating a socket of type TCP
	Sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//check that sockfd value is lesser than zero
	//if it is yes, then there is something wrong for creating the socket
	if(Sockfd < 0) {
		error("Socket creation Failed !!");
	}

	port = atoi(argv[1]);		//extract PORTNO. from string

	//initialize the server_addr structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(port);


	//Bind the current socket (which is also known current file descriptor) with the server_addr
	//here bind function return some value
	//if successful binding, gives positive value
	//if unsuccessful binding, gives negative value 
	int b = bind(Sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)); 
	
	if (b < 0){				//check that if binding is successful or not
		error("Binding Failed !!");
	}

	//setting the server that how many client would have to connect to the server (one by one) 
	//if successful listen return positive value
	//if unsuccessful listen return negative value 
	int lis = listen(Sockfd, 5);

	if(lis < 0) {			//check that the listen is successful or not
		error("listen unsuccessful !!");
	}


	clientlen = sizeof(client_addr);

	printf("Waiting the Client Request for Connection............\n");

	//creating the new socket (new file descriptor) with old socket 
	//by accepting the client request
	newSockfd = accept(Sockfd, (struct sockaddr *) &client_addr, &clientlen);
	printf("Receiving request from Client for Connection..\n");
	if(newSockfd < 0){
		error("Client connection Failed !!");
	}
	printf("Client successfully Connected\n");




	char *filename;
	filename = (char *)malloc(sizeof(char)*100);

	//just to be make sure initially the filename is null
	bzero(filename, 100);

	//receiving the filename from client
	int r = recv(newSockfd, filename, 100, 0);
	if(r < 0) {
		error("Receiving filename Failed !!");
	}
	printf("successfully Received filename\n");



	//searches the file and open in binary read mode
	FILE *freadptr  = fopen(filename, "rb");

	//if file is not there, so the file pointer is null otherwise it pointed to the file 
	if(freadptr == NULL) {
		perror("File not found");
		printf("Client Disconnected\n");	//here file not found, so we need to terminate
		exit(1);
	}
	printf("File successfully Found\n");




	//finding Size of the sending file  
	fseek(freadptr, 0 , SEEK_END);
  	long fileSize = ftell(freadptr);
  	fseek(freadptr, 0 , SEEK_SET);


  	
  	//this is only for sending the filesize to the client
  	send(newSockfd, &fileSize, sizeof(fileSize), 0);



	char *content = malloc(fileSize + 1);

	//read the complete file in the content variable
	fread(content, fileSize, 1, freadptr);

	//send the content to the client
	for(int i=0;i<fileSize;i++) {
		send(newSockfd, &content[i], sizeof(content[i]), 0);
	}

	
	//Now the successful message print
	printf("successfully File Send\n");
	printf("Client Disconnected\n");

	//close file pointer and all socket
	fclose(freadptr);
	close(Sockfd);
	close(newSockfd);

	return 0;
}