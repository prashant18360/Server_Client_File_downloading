# Server_Client_File_downloading
# Socket_programming
Socket programming code for server-client networking, in which connection between client and server where client can download file from server.

Write a client server socket program in C. You programm should have the following feature:

a. The client initiates the connection
b. Takes a file name from the user
c. Sends the file name to the server
d. Server searches for the file name in its shared drive (you can assume there is only one shared folder that a client can access file from)
e. Server sends the file to the client
f. Clients stores it in its local drive
g. Client closes the connection


After you have successfully run the program, collect a wireshark trace at the client side and compute the following metrics 

a. How many TCP connections are made
b. What is the port number of the server and what is that of the client
c. How many packets are exchanged between client and server
d. How much time is needed to download the file
