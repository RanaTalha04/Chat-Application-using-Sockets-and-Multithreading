client: client.c login.c makeConnection.c oneToOne.c
	gcc client.c login.c makeConnection.c oneToOne.c -o client;
	./client
server: server.c
	gcc server.c -o server;
	./server