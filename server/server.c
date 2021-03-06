#include "netprogram.h"
#include "head.h"
#include "wrap_pthread.h"
#include <sys/time.h>
#include <sys/signal.h>

/* A Client's state */
struct ClientState_ { 
	player_data player;

	// connfd is available when player.station != NOT_ONLINE
	int connfd;

	// following are valid when player.station == FIGHTING
	char rival_id[ID_LEN];  
	char stuff[STUFF_LEN];
	int lifetime;
	int turn_num;
	int wait;  // if >= 0, I am waiting for rival's stuff  
};
typedef struct ClientState_ ClientState;

ClientState cstate_table[MAX_PLAYERS_NUM]; 
pthread_rwlock_t cstable_rwlock;

void init_table() {
	int i = 0;
    for(i = 0; i < MAX_PLAYERS_NUM; i ++) {
		cstate_table[i].player.station = NOT_ONLINE;
		cstate_table[i].wait = -1;
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
	
	for(i = 0; i < MAX_PLAYERS_NUM; i ++) {
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
	
	for(i = 0; i < MAX_PLAYERS_NUM; i ++) {
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
	ClientState *me = get_cstate_byid(myid);
	if(me != NULL)
	    rival = get_cstate_byid(me->rival_id);

	return rival;
}

int get_rival_cfd(char* myid) {
	ClientState *rival = get_rival_state(myid);
	int r;

	if(rival != NULL)
		r = rival->connfd;
	else 
		r = -1;

	return r;
}

int get_mycfd(char *myid) {
	ClientState *me = get_cstate_byid(myid);
	int r;

	if(me != NULL)
		r = me->connfd;
	else 
		r = -1;

	return r;
}

uint8_t get_player_station(char *id) {
	ClientState *c = get_cstate_byid(id);
	uint8_t r;

	if( c == NULL )
		r = NOT_ONLINE;
	else 
		r = c->player.station;

	return r;
}

ClientState *get_empty_entry() {
	int i = 0;
	ClientState *r = NULL;
	
	for(i = 0; i < MAX_PLAYERS_NUM; i ++) {
		ClientState *p = &(cstate_table[i]);

		if( p->player.station == NOT_ONLINE ) {
			r = p;
			break;
		}
	}

	return r;
}

void set_rival_id(char *myid, char *rid) {
	if( !myid ) return;

	ClientState *me = get_cstate_byid(myid);
	if(me == NULL) return;

	if(rid == NULL)
		me->rival_id[0] = '\0';
	else
		strncpy(me->rival_id, rid, ID_LEN);
}

int change_cstation(char *id, uint8_t s) {
	if(id == NULL) return 0;

	ClientState *c = get_cstate_byid(id);

	if(c != NULL) {
		c->player.station = s;
		if(s == FIGHTING) {
			c->lifetime = 5;
			c->turn_num = 0;
		}
	}
	else
		return 0;

	return 1;
}

void construct_players_arr(player_data arr[]) {
	int i;
	
	for(i = 0; i < MAX_PLAYERS_NUM; i ++) {
		ClientState *p = &(cstate_table[i]);

		if( p->player.station == NOT_ONLINE ) {
			set_player(&(arr[i]), NOT_ONLINE, NULL);
		}
		else {
			set_player(&(arr[i]), p->player.station, p->player.id);
		}
	}

}

void give_everyone_players() {
	printf("give_everyone_players\n");	
	int i;
	server_data sdata;
	server_data *p = &sdata;

    set_sd_station(p, SGIVE_TABLE);
	construct_players_arr(p->players);

	// send every player the players table
	for(i = 0; i < MAX_PLAYERS_NUM; i ++) {
		ClientState *r = &(cstate_table[i]);
		if( r->player.station != NOT_ONLINE ) {
			Writen(r->connfd, p, sizeof(sdata));
		}
	}
}

void game_result(ClientState *winner, ClientState *loser, int equal, char *winstr, char *losestr) {
	server_data wdata, ldata;
	server_data *p_win = &wdata, *p_lose = &ldata;
	if( equal ) {
		set_sd_rd(p_win, winner->lifetime, loser->stuff, EQUAL);
		set_sd_rd(p_lose, loser->lifetime, winner->stuff, EQUAL);
	} 
	else {
		loser->lifetime --;
		set_sd_rd(p_win,  winner->lifetime, loser->stuff, WIN);

		set_sd_rd(p_lose, loser->lifetime, winner->stuff, FAIL);

		if(loser->lifetime <= 0) { // gameover
			set_sd_station(p_win, SRETURN_WINNER);
			set_sd_station(p_lose, SRETURN_WINNER);
			set_sd_over(p_win, WIN, winstr);
			set_sd_over(p_lose, FAIL, losestr);

			// change station
			winner->player.station = ONLINE;
			loser->player.station = ONLINE; 
			
	        give_everyone_players();
		}
		else {
			set_sd_station(p_win,  SRETURN_BATTLE);
			set_sd_station(p_lose, SRETURN_BATTLE);
		}
	}

	Writen(winner->connfd, p_win, sizeof(wdata));
	Writen(loser->connfd, p_lose, sizeof(ldata));

}


void check_time_limit(int sig) {
	int i = 0;
	struct itimerval timer;

	Pthread_rwlock_wrlock(&cstable_rwlock);
	for(i = 0; i < MAX_PLAYERS_NUM; i ++) {
		ClientState *p = &(cstate_table[i]);

		if( p->player.station == FIGHTING && p->wait >= 0) {
			p->wait ++;
			if(p->wait > TIME_LIMIT) { // time out and p win
				ClientState *loser = get_rival_state(p->player.id);
				loser->turn_num = p->turn_num;
				p->wait = -1;
				game_result(p, loser, 0, "Because of time,", "Because of time");
			}
		}
	}
	Pthread_rwlock_unlock(&cstable_rwlock);

   signal(SIGALRM, check_time_limit);   
   timer.it_interval.tv_sec = 0;
   timer.it_interval.tv_usec = 0;
   timer.it_value.tv_sec = TIME_LIMIT; /* 10 seconds timer */
   timer.it_value.tv_usec = 0;
   setitimer(ITIMER_REAL, &timer, 0);
}

void init_timer() {
	struct itimerval timer;
	timer.it_interval.tv_sec = 0;
  	timer.it_interval.tv_usec = 0;
  	timer.it_value.tv_sec = TIME_LIMIT; /* 10 seconds timer */
  	timer.it_value.tv_usec = 0;
  	setitimer(ITIMER_REAL, &timer, 0);

  	signal(SIGALRM, check_time_limit); /* set the Alarm signal capture */
}

void Clogin(int connfd, client_data *buf) {
	printf("A client %s logging in \n", buf->id);
	server_data sdata;
	server_data *p = &sdata;

	Pthread_rwlock_wrlock(&cstable_rwlock);

	ClientState *me = get_cstate_byid(buf->id);
	if( me == NULL && (me = get_empty_entry()) ) { // NOT_ONLINE and the table is not full
		set_player(&(me->player), ONLINE, buf->id);
		me->connfd = connfd;
		set_sd_station(p, SRLINK);
	}
	else 
		set_sd_station(p, SREFUSE);

	Pthread_rwlock_unlock(&cstable_rwlock);

	Writen(connfd, p, sizeof(sdata));

	Pthread_rwlock_rdlock(&cstable_rwlock);
	give_everyone_players();
	Pthread_rwlock_unlock(&cstable_rwlock);
}

void Cneed_table(int connfd) {
	printf("Cneed_table\n");
	server_data sdata;
	server_data *p = &sdata;

	Pthread_rwlock_wrlock(&cstable_rwlock);
	set_sd_station(p, SGIVE_TABLE);
	Pthread_rwlock_unlock(&cstable_rwlock);

	Pthread_rwlock_rdlock(&cstable_rwlock);
	construct_players_arr(p->players);
	Pthread_rwlock_unlock(&cstable_rwlock);

	Writen(connfd, p, sizeof(sdata));
}

void Cchoose_player(int connfd, client_data *buf) {
	printf("Cneed_table\n");
	server_data sdata;
	server_data *p = &sdata;

	Pthread_rwlock_wrlock(&cstable_rwlock);

	char *rival_id = buf->pkid;
	int rival_cfd = get_mycfd(rival_id);
	if( rival_cfd >= 0) {
		set_sd_station(p, SASK_PLAYER);
		set_sd_rid(p, buf->id);

		set_rival_id(buf->id, rival_id);
		set_rival_id(rival_id, buf->id);

		Writen(rival_cfd, p, sizeof(sdata));
	}
	else {
		set_sd_station(p, SREFUSE);
		Writen(connfd, p, sizeof(sdata));
	}

	Pthread_rwlock_unlock(&cstable_rwlock);
}

void Creply_to_c(client_data *buf) {
	printf("Creply_to_c from %s\n", buf->id);
	ClientState *rival;
	server_data sdata;
	server_data *p = &sdata;

	Pthread_rwlock_wrlock(&cstable_rwlock);

	rival = get_rival_state(buf->id);
	if( rival == NULL ) {
		Pthread_rwlock_unlock(&cstable_rwlock);
		return; 
	}

	set_sd_rid(p, rival->player.id);
    if( buf->pkreply == 1 ) { 
	    set_sd_station(p, SCREAT_GAME);
	    change_cstation(buf->id, FIGHTING);
	    change_cstation(rival->player.id, FIGHTING);
	}
	else { 
	    set_sd_station(p, SREFUSE);
	}

	Pthread_rwlock_unlock(&cstable_rwlock);

	Writen(rival->connfd, p, sizeof(sdata));

	Pthread_rwlock_rdlock(&cstable_rwlock);
	give_everyone_players();
	Pthread_rwlock_unlock(&cstable_rwlock);
}

void Cshow_stuff(int connfd, client_data *buf) {
	printf("Cshow_stuff from %s\n", buf->id);
	ClientState *me, *rival;

	Pthread_rwlock_wrlock(&cstable_rwlock);

	me    = get_cstate_byid(buf->id);
	rival = get_rival_state(buf->id);

	me->turn_num ++;
	strncpy(me->stuff, buf->pk_stuff, STUFF_LEN);

	if( rival->wait < TIME_LIMIT && me->turn_num == rival->turn_num ) {
		int result = judge_win(me->stuff, rival->stuff);
		int equal = (result == 0) ? 1 : 0;

	    rival->wait = -1;
		me->wait = -1;

		ClientState *winner = (result > 0)  ? me : rival;
	    ClientState *loser  = (result <= 0) ? me : rival; 

		game_result(winner, loser, equal, "You win this time.", "You lose this time.");
	}
	else { // wait
		me->wait = 0;
	}

	Pthread_rwlock_unlock(&cstable_rwlock);
}

void Cchat(client_data *buf) {
	printf("Cchat from %s\n", buf->id);
	int rival_connfd;
	server_data sdata;
	server_data *p = &sdata;

	Pthread_rwlock_rdlock(&cstable_rwlock);

	rival_connfd = get_rival_cfd(buf->id);
	if( rival_connfd >= 0 ) {
		set_sd_station(p, SCHAT);
		set_sd_saying(p, buf->saying);
	}
	else { // may not appear
		return;
	}

	Pthread_rwlock_unlock(&cstable_rwlock);

	Writen(rival_connfd, p, sizeof(sdata));
}

void Cquitgame(client_data *buf) {
	printf("Cquitgame from %s\n", buf->id);
	ClientState *winner = get_rival_state(buf->id);
	ClientState *me  = get_cstate_byid(buf->id);
	
	server_data wdata;
	server_data *p_win = &wdata;
	set_sd_station(p_win, SRETURN_WINNER);
	set_sd_over(p_win, WIN, "Your rival went away.");

	Pthread_rwlock_wrlock(&cstable_rwlock);

	Writen(winner->connfd, p_win, sizeof(wdata));
	winner->player.station = ONLINE;
	me->player.station = ONLINE;

	Pthread_rwlock_unlock(&cstable_rwlock);

	Pthread_rwlock_rdlock(&cstable_rwlock);
	give_everyone_players();
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
		case CEXIT:          return Cquitgame(p);
		default:             printf("Unknown client's station\n");
	}
}

void wait_client_data(int connfd) {
	ssize_t n;
	char buf[MAXLINE];

	while(1) {
		n = readn(connfd, buf, sizeof(client_data));
		
		if( n > 0 ) {
			printf("handling a client data\n");
			handle_client_data(connfd, buf);
		}
		else if( errno == EINTR ) {
			continue;
		}
		else {
			Pthread_rwlock_wrlock(&cstable_rwlock);

			ClientState *p = get_cstate_bycfd(connfd);
			if(p != NULL) {
				printf("A client went away\n");
				p->player.station = NOT_ONLINE;
			}

			Pthread_rwlock_unlock(&cstable_rwlock);

			Pthread_rwlock_rdlock(&cstable_rwlock);
			give_everyone_players();
			Pthread_rwlock_unlock(&cstable_rwlock);
			break;
		}
	}
}

void *service(void *cfd) {
	printf("service thread\n");
	int connfd = *((int *)cfd);

	// main thread needn't\ to wait for each thread it creates
	Pthread_detach(pthread_self()); 

	// here we go
	wait_client_data(connfd);

	// closed the desctiptor shared with the main thread
	Close(connfd);

	// cfd was malloced
	free(cfd);

	return NULL;
}

void server_start() {
	int listenfd, connfd;
	pthread_t tid;
	struct sockaddr_in server;
	struct sockaddr client;
	socklen_t addrlen = sizeof(client);

    // Create a tcp socket
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	// Prepare the sockaddr_in structure
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(SERV_PORT);

	// Bind server's well-known port to socket
	Bind(listenfd, (struct sockaddr*)(&server), sizeof(server));

	// Convert the socket into a listening socket
	Listen(listenfd, LISTENQ);

	// Initialize a rwlock for shared cstate_table
	Pthread_rwlock_init(&cstable_rwlock, NULL); 

	printf("Server is running\n");
	// One thread per client
	while(1) {
		connfd = Accept(listenfd, &client, &addrlen);
		printf("Accept a client %d\n", connfd);
		int *p_cfd = (int *)malloc(sizeof(int));
		*p_cfd = connfd;
		Pthread_create(&tid, NULL, &service, (void *)p_cfd);
	}
}
	
int main() {
	init_table();
	init_timer();
	server_start();
	return 0;
}

