#include "netprogram.h"
#include "head.h"
#include "Pthread.h"

/* A Client's state */
struct ClientState_ { 
	player_data player;

	// following are valid when info.station == FIGHTING
	char rival_id[ID_LEN];  
	char stuff[STUFF_LEN];
	uint8_t lifetime;
	uint8_t turn_num;  
};
typedef struct ClientState_ ClientState;
ClientState cstate_table[MAX_PLAERS_NUM]; 

void init_table() {
	int i = 0;
	while(i ++ < MAX_PLAERS_NUM) {
		cstate_table[i].player.station = NOT_ONLINE;
	}
}

/*
 * NULL is returned if doesn't find the client.
 * The pointer point to the client's state is returned if the client is online or playing with someone.
 */
ClientState *get_client_state(char *id) {
	int i = 0;
	for(i = 0; i < MAX_PLAERS_NUM; i ++) {
		ClientState *p = &(cstate_table[i]);

		if( p->player.station != NOT_ONLINE ) 
			if( strcmp(p->player.id, id) == 0 ) 
				return p;
	}
	return NULL;
}

uint8_t get_player_station(char *id) {
	ClientState *c = get_client_state(id);
	if( c == NULL )
		return NOT_ONLINE;
	else 
		return c->player.station;
}

void construct_players_arr(player_data arr[]) {
	int i = 0;
	int j = 0;
	for(i = 0; i < MAX_PLAERS_NUM; i ++) {
		ClientState *p = &(cstate_table[i]);

		if( p->player.station != NOT_ONLINE ) {
			arr[j].station = p->player.station;
			strncpy(arr[j].id, p->player.id, ID_LEN);
		}
	}
}

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
	
