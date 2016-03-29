#ifndef _HEAD_
#define _HEAD_
#define DEBUG

#define ID_LEN 10
#define SAYING_LEN 500
#define STUFF_LEN 10
#define MAX_PLAYERS_NUM 10

#define MAXLINE 4096
#define SERV_PORT 4321//目标服务器端口

#define true 1
#define false 0

#define CLOGIN 0
#define CNEED_TABLE 2
#define CCHOOSE_PLAYER 3
#define CREPLY_TO_C 4
#define CSHOW_STUFF 5
#define CCHAT 7
#define CEXIT 8

#define SREFUSE 0
#define SRLINK	1
#define SGIVE_TABLE	2
#define SCREAT_GAME 3
#define SASK_PLAYER 4
#define SRETURN_BATTLE 5
#define SRETURN_WINNER 6
#define SCHAT	7

// a game's result
#define WIN 1
#define FAIL 0
#define EQUAL 2

// a client's state
#define NOT_ONLINE 0
#define ONLINE 1
#define FIGHTING 2    // playing with someone

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

struct return_data{
	char lifetime;     //剩余生命数
	char pk_stuff[STUFF_LEN];
	char win_station;
};

struct player_data{
	char station;     // NOT_ONLINE Or ONLINE Or FIGHTING
	char id[ID_LEN];
};
#pragma pack()

typedef struct return_data return_data;
typedef struct player_data player_data;
typedef struct client_data client_data;

#pragma pack(1)
struct server_data{
	char station;            //0,禁止访问，1，验证通过，2，刷新用户列表，3，pk连接建立，4，pk反馈，5，问询，6，通知胜负，7，聊天转发
	char another_id[ID_LEN];
	return_data returndata;
	
	char game_over[50];      // when the game is over, the client should print this string
	char game_station;       // WIN Or FAIL Or Equal
	
	player_data player[MAX_PLAYERS_NUM];  
	char saying[SAYING_LEN];
};
#pragma pack()
typedef struct server_data server_data;

#endif
