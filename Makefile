client: client.c login.c makeConnection.c oneToOne.c messageHandler.c
	gcc client.c login.c makeConnection.c oneToOne.c messageHandler.c -o client;
	./client
server: server.c
	gcc server.c -o server;
	./server