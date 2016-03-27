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
#define SERV_PORT 4321//Ŀ��������˿�
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


#pragma pack(1)//���ݽṹ
struct client_data{
	int station;//0.��֤ID��1�������ս��2�����Ͷ�ս��3���������죬4����ѯӦ��5���������
	char id[10];
	char pkid[10];
	char pk_stuff[10];
	char saying[500];
};
#pragma pack()

#pragma pack(1)
struct return_data{
	int lifetime;//ʣ��������
	char pk_stuff[10];
	int win_station;
};

#pragma pack()

#pragma pack(1)
struct player_data{
	int station;//0,������1�����ڿ�������2����æ
	char id[10];
};
#pragma pack()

typedef struct return_data return_data;
typedef struct player_data player_data;
typedef struct client_data client_data;
#pragma pack(1)
struct server_data{
	int station;//0,��ֹ���ʣ�1����֤ͨ����2��ˢ���û��б�3��pk���ӽ�����4��pk������5����ѯ��6��֪ͨʤ����7������ת��
	return_data returndata;
	int waittime;
	char saying;
	player_data player[10];//��ǰ��¼
};
#pragma pack()


#endif
