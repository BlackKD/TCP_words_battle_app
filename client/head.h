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
#define SERV_PORT 4321//目标服务器端口
#define true 1
#define false 0
int link_to_server(char *userid);
void main_function();
int sockfd;

int kbhit();
void *print_thread(void *para);
void Try_to_get_challenge(char *buff);
void *play_time_thread(void *para);
int set_client_data(int station,char *buff);


#pragma pack(1)//数据结构
struct client_data{
	int station;//0.验证ID，1，请求对战，2，发送对战，3，发送聊天，4，问询应答，5，请求结束
	char id[10];
	char pkid[10];
	char pk_stuff[10];
	char saying[500];
};
#pragma pack()

#pragma pack(1)
struct return_data{
	int lifetime;//剩余生命数
	char pk_stuff[10];
	int win_station;
};

#pragma pack()

#pragma pack(1)
struct player_data{
	int station;//0,死亡，1，存在可以请求，2，正忙
	char id[10];
};
#pragma pack()

typedef struct return_data return_data;
typedef struct player_data player_data;
typedef struct client_data client_data;
#pragma pack(1)
struct server_data{
	int station;//0,禁止访问，1，验证通过，2，刷新用户列表，3，pk连接建立，4，pk反馈，5，问询，6，通知胜负，7，聊天转发
	return_data returndata;
	int waittime;
	char saying;
	player_data player[10];//当前登录
};
#pragma pack()


#endif
