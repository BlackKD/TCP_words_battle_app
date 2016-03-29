#include "netprogram.h"
#include "head.h"
#include "Pthread.h"

/* A Client's state */
struct ClientState_ { 
	player_data player;

	// connfd is available when player.station != NOT_ONLINE
	int connfd;

	// following are valid when player.station == FIGHTING
	char rival_id[ID_LEN];  
	char stuff[STUFF_LEN];
	uint8_t lifetime;
	uint8_t turn_num;  
};
typedef struct ClientState_ ClientState;

ClientState cstate_table[MAX_PLAERS_NUM]; 
pthread_rwlock_t cstable_rwlock;

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
ClientState *get_cstate_byid(char *id) {
	if(id == NULL) 	return NULL;

	int i = 0;
	ClientState *r = NULL;
	
	for(i = 0; i < MAX_PLAERS_NUM; i ++) {
		ClientState *p = &(cstate_table[i]);

		if( p->player.station != NOT_ONLINE ) 
			if( strcmp(p->player.id, id) == 0 ) { 
				r = p;
				break;
			}
	}

	return r;
}

ClientState *get_cstate_bycfd(int connfd) {
	int i = 0;
	ClientState *r = NULL;
	
	for(i = 0; i < MAX_PLAERS_NUM; i ++) {
		ClientState *p = &(cstate_table[i]);

		if( p->player.station != NOT_ONLINE ) 
			if( p->connfd == connfd ) { 
				r = p;
				break;
			}
	}

	return r;
}

ClientState *get_rival_state(char *myid) {
	ClientState *rival = NULL;
	ClientState *me = get_cstate_bycfd(myid);
	if(me != NULL)
	   rival = get_cstate_byid(me->rival_id);

	return rival;
}

int get_rival_cfd(char* myid) {
	ClientState *rival = get_rival_state(myid);
	if(rival != NULL)
		return rival->connfd;
	else 
		return -1;
}

uint8_t get_player_station(char *id) {
	ClientState *c = get_client_state(id);
	if( c == NULL )
		return NOT_ONLINE;
	else 
		return c->player.station;
}

ClientState *get_empty_entry() {
	int i = 0;
	ClientState *r = NULL;
	
	for(i = 0; i < MAX_PLAERS_NUM; i ++) {
		ClientState *p = &(cstate_table[i]);

		if( p->player.station == NOT_ONLINE ) {
			r = p;
			break;
		}
	}

	return r;
}

inline void set_player(player_data *p, uint8_t s, char *id) {
	if(p == NULL) return;

	p->station = s;
	if(id = NULL)
		p->id[0] = '\0';
	else
		strncpy(p->id, id, ID_LEN);
}

void change_cstation(char *id, uint8_t s) {
	if(id == NULL) return;

	ClientState *c = get_cstate_byid(id);

	if(c == NULL) { 
		if( c = get_empty_entry() ) { // c must not be NULL
			set_player(&(c->player), s, id);
		}
	}
	else {
		c->player.station = s;
	}
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

/*
 * Set the station field in a server_data
 */
inline void set_sd_station(server_data *p, uint8_t station) {
	if(p == NULL) return;

	p->station = station;
}

/*
 * Set the another_id field in a server_data
 */
inline void set_sd_rid(server_data *p, char *rival_id) {
	if(p == NULL) return;

    if(rival_id == NULL)
		p->another_id[0] = '\0';
	else
	    strncpy(p->another_id, rival_id, ID_LEN);
}

/*
 * Set the returndata field in a server_data
 */
inline void set_sd_rd(server_data *p, uint8_t lifetime, char *stuff, uint8_t win) {
	if(p == NULL) return;

	return_data *t = &(p->returndata);

	t->lifetime = life_time;
	if( stuff == NULL )
		t->pk_stuff[0] = '\0';
	else
		strncpy(t->pk_stuff, stuff, STUFF_LEN);
}

/*
 * Set the game_over field in a server_data
 */
inline void set_sd_over(server_data *p, char *prompt) {
	if( p == NULL ) return;

	if(prompt == NULL)
		p->gameover[0] = '\0';
	else
		strncpy(t->gameover, prompt, PROMPT_LEN);
}

/*
 * Set the game_station field in a server_data
 */
inline void set_sd_station(server_data *p, uint8_t s) {
	if( p == NULL ) return;

	p->game_station = s;
}

/*
 * Set the player field in a server_data
 */
inline void set_sd_player(server_data *p) {
	if( p == NULL ) return;

	construct_player_arr(p->player);
}

/*
 * Set the saying field in a server_data
 */
inline void set_sd_saying(server_data *p, char *say) {
	if(p == NULL) return;

	if( say == NULL )
		p->say[0] = '\0';
	else 
		strncpy(p->saying, say, SAYING_LEN);
}

void Clogin(int connfd, client_data *buf) {
	server_data sdata;
	server_data *p = &sdata;

	Pthread_rwlock_wrlock(&cstable_rwlock);

	if( get_cstate_byid(buf->id) == NULL ) { // NOT_ONLINE
		set_sd_station(p, SRLINK);
		change_cstation(buf->id, ONLINE);
	}
	else 
		set_sd_station(p, SREFUSE);

	Pthread_rwlock_unlock(&cstable_rwlock);

	Writen(connfd, p, sizeof(sdata));
}

void Cneed_table(int connfd) {
	server_data sdata;
	server_data *p = &sdata;

	Pthread_rwlock_rdlock(&cstable_rwlock);

	set_sd_station(p, SGIVE_TABLE);
	set_sd_player(p);

	Pthread_rwlock_unlock(&cstable_rwlock);

	Writen(connfd, p, sizeof(sdata));
}

void Cchoose_player(int connfd, client_data *buf) {
	server_data sdata;
	server_data *p = &sdata;

	Pthread_rwlock_rdlock(&cstable_rwlock);

	char *rival_id = buf->pkid;
	ClientState *rival = get_cstate_byid(rival_id);
	if( rival != NULL) {
		set_sd_station(p, SASK_PLAYER);
		set_sd_rid(p, buf->id); 
		Writen(rival->connfd, p, sizeof(sdata));
	}
	else {
		set_sd_station(p, SREFUSE);
		Writen(connfd, p, sizeof(sdata));
	}

	Pthread_rwlock_unlock(&cstable_rwlock);
}

void Creply_to_c(client_data *buf) {
	ClientState *rival;
	server_data sdata;
	server_data *p = &stata;

	Pthread_rwlock_wrlock(&cstable_rwlock);

	rival = get_rival_state(buf->id);
    if( rival != NULL ) { 
	   set_sd_station(p, SCREATE_GAME);
	   set_sd_rid(p, rival->player.id);
	   change_cstation(buf->id, FIGHTING);
	   change_cstation(rival->player.id, FIGHTING);
	}
	else { // may not appear
		return;
	}

	Pthread_rwlock_unlock(&cstable_rwlock);

	Writen(rival->connfd, p, sizeof(sdata));
}

void Cshow_stuff(int connfd, client_data *buf) {
	ClientState *me, *rival;

	Pthread_rwlock_wrlock(&cstable_rwlock);

	me = get_cstate_byid(buf->id);
	rival = get_rival_state(buf->id);
	me->times ++;
	strncpy(me->stuff, buf->pk_stuff, STUFF_LEN);

	Pthread_rwlock_unlock(&cstable_rwlock);

	if( me->times == rival->times ) {
	}
	else {
	}
}

void Cchat(client_data *buf) {
	ClientState *rival;
	server_data sdata;
	server_data *p = &stata;

	Pthread_rwlock_rdlock(&cstable_rwlock);

	rival = get_rival_state(buf->id);
	if( rival != NULL ) {
		set_sd_station(p, SCHAT);
		set_sd_saying(p, buf->saying);
	}
	else {
		return;
	}

	Pthread_rwlock_unlock(&cstable_rwlock);

	Writen(rival->connfd, p, sizeof(sdata));
}

void Cquit(client_data *buf) {
	Pthread_rwlock_wrlock(&cstable_rwlock);

	change_cstation(buf->id, ONLINE);

	Pthread_rwlock_unlock(&cstable_rwlock);
}

void handle_client_data(int connfd, char *buf) {
	client_data *p = (client_data *)buf;
	switch( p->station ) {
		case CLOGIN:         return Clogin(connfd, p);
		case CNEED_TABLE:    return Cneed_table(connfd);
		case CCHOOSE_PLAYER: return Cchoose_player(connfd, p);
		case CREPLY_TO_C:    return Creply_to_c(p);
		case CSHOW_STUFF:    return Cshow_stuff(connfd, p);
		case CCHAT:          return Cchat(p);
		case CEXIT:          return Cquit(p);
		default:             printf("Unknown client's station\n");
	}
}

void wait_client_data(int connfd) {
	ssize_t n;
	char buf[MAXLINE];

	while(1) {
		n = readn(connfd, buf, MAXLINE);
		
		if( n > 0 ) {
			handle_client_data(connfd, buf, n);
		}
		else if( errno == EINTR ) {
			continue;
		}
		else {
			break;
		}
	}
}

void *service(void *cfd) {
	int connfd = (int)cfd;

	// main thread needn't to wait for each thread it creates
	Pthread_detach(pthread_self()); 

	// here we go
	wait_client_data(connfd);

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

	// Initialize a rwlock for shared cstate_table
	Pthread_rwlock_init(&cstate_rwlock, NULL); 

	// One thread per client
	while(1) {
		connfd = Accept(listenfd, client, &addrlen);
		Pthread_create(&tid, NULL, &service, (void *)connfd);
	}
}
	
