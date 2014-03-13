Network-protocol-development-NJU
================================

This is the project of Network Protocol Development in the Department of Computer Science and Technology at  Nanjing University, Spring, 2014. 

*************************************************************
* This is lab02 the project of Network Protocol Development * 
* in the Department of Computer Science and Technology at   *
* Nanjing University, Spring, 2014.                         *
*************************************************************

This is a simplifed munipulation of the tcp/ip client server model using C socket programming.
The client can query the server about the weather forecast the next given days of given city,
and server provides all the information to be returned to the client.
Concurrent connections is supported by the server. For the sake of simplicity , all information 
provided by the server is fake.

Compilation:
	In Debian/Ubuntu, go to the project folder
	> cd lab02

	Compile the client
	> make client

	Compile the server
	> make server

	If you want to clean all the executable files
	> make clean

Usage: 
  To connect to remote server in 114.212.191.33
	Make sure that you have a connection to 114.212.191.33.
	Start the client
	> ./client

  Or, to hava server and clients running on the local host, 
	> cd lab02/Client/
	> vim client.h 
	> change the SERV_IP macro from "114.212.191.33" to "127.0.0.1"

	Start the server First
	> ./server
	
	Start the client
	> ./client
