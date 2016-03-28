#include "netprogram.h"
#include "head.h"
#include "Pthread.h"

void *service(void *cfd) {
	int connfd = (int)cfd;

	// main thread needn't to wait for each thread it creates
	Pthread_detach(pthread_self()); 

	// here we go
	handle_client_data(connfd);

	// closed the desctiptor shared with the main thread
	Close(connfd);

	return NULL;
}

void server_start() {
	int listenfd, connfd;
	pthread_t tid;
	struct sockaddr server, client;
	socklen_t addrlen = sizeof(client);

    // Create a tcp socket
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	// Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(SERV_PORT);

	// Bind server's well-known port to socket
	Bind(listenfd, (&server_addr), sizeof(server_addr));

	// Convert the socket into a listening socket
	Listen(listenfd, LISTENQ);

	// One thread per client
	while(1) {
		connfd = Accept(listenfd, client, &addrlen);
		Pthread_create(&tid, NULL, &service, (void *)connfd);
	}
}
	
