#ifndef _HEAD_
#define _HEAD_
#define debug
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include <pthread.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
pthread_mutex_t gl_mutex;
pthread_mutex_t gametime_mutex;
pthread_t threadID_to_get_player;
pthread_t threadID_to_listengame;
#define MAXLINE 4096
#define SERV_PORT 1234//目标服务器端口
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

#define win 1
#define fail 0
#define equal 2

//#define char uint8_t



#pragma pack(1)//数据结构
struct client_data{
	char station;
	char id[10];
	char pkid[10];
	char pkreply;
	char pk_stuff[10];
	char saying[500];
};
#pragma pack()

#pragma pack(1)
struct return_data{
	char lifetime;//剩余生命数
	char pk_stuff[10];
	char win_station;
};

#pragma pack()

#pragma pack(1)
struct player_data{
	char station;//0,死亡，1，存在可以请求，2，正忙
	char id[10];
};
#pragma pack()

typedef struct return_data return_data;
typedef struct player_data player_data;
typedef struct client_data client_data;
#pragma pack(1)
struct server_data{
	char station;//0,禁止访问，1，验证通过，2，刷新用户列表，3，pk连接建立，4，pk反馈，5，问询，6，通知胜负，7，聊天转发
	char another_id[10];
	return_data returndata;
	
	//下面这俩用来结束游戏
	char game_over[50];
	char game_station;
	
	player_data player[10];//当前登录
	char saying[500];
};
#pragma pack()
typedef struct server_data server_data;

#endif
