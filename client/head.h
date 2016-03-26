#ifndef _HEAD_
#define _HEAD_
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
#pragma pack(1)//数据结构
struct client_data{
	char type[2];//服务类型
	char sourse[20];//城市名称
	char day;//申请时间
};
#pragma pack()

#pragma pack(1)
struct server_data{
	char type[2];//服务类型
	char sourse[20];//城市名称
	unsigned short year;//年份
	char rank[2];//日期
	char day;//询问日期
	char weather[3][2];//天气温度
	char temp[44];//填充
};
#pragma pack()

#endif
