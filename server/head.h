#ifndef _HEAD_
#define _HEAD_
#define DEBUG

#define ID_LEN 10
#define SAYING_LEN 500
#define STUFF_LEN 10
#define MAX_PLAYERS_NUM 10
#define PROMPT_LEN 50  // game_over string's len

#define MAXLINE 4096
#define SERV_PORT 1234//目标服务器端口
#define SERV_ADDR "114.212.190.188"

#define true 1
#define false 0

#define CLOGIN 0
#define CNEED_TABLE 2
#define CCHOOSE_PLAYER 3
#define CREPLY_TO_C 4
#define CSHOW_STUFF 5
#define CCHAT 7
#define CEXIT 8

// pk_stuff
#define SCISSOR "p1" 
#define STONE   "p2"
#define CLOTH   "p3"

#pragma pack(1)
struct client_data{ // the structure of the data sent by client 
	char station;
	char id[ID_LEN];
	char pkid[ID_LEN];
	char pkreply;
	char pk_stuff[STUFF_LEN];
	char saying[SAYING_LEN];
};

#pragma pack()

typedef struct client_data client_data;

/*
 * if stuff1 win stuff2, 1 will be returned
 * if equal, 0 will be returned
 * if lose, -1 will be returned
 */
static inline int judge_win(char *stuff1, char *stuff2) {
	char c1 = stuff1[1];
	char c2 = stuff2[1];
	switch( c1 ) {
		case '1': if( c2 == '1' ) return 0;
			      if( c2 == '2' ) return -1;
			      if( c2 == '3' ) return 1;

		case '2': if( c2 == '1' ) return 1;
			      if( c2 == '2' ) return 0;
			      if( c2 == '3' ) return -1; 
		
		case '3': if( c2 == '1' ) return -1;
			      if( c2 == '2' ) return 1;
			      if( c2 == '3' ) return 0;

		default:  return 1;
	}
}

#include "server_data.h"

#endif
