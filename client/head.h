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
#define MAXLINE 4096
#define SERV_PORT 4321//Ŀ��������˿�
#define true 1
#define false 0
int link_to_server(char *userid);
void main_function();
int sockfd;
int send_city_to_server(char *buffer,int len);
void Haved_find();
void need_today_weather(char *buffer,int len);
void need_threeday_weather(char *buffer,int len);
void need_customday_weather(char *buffer,int len);
#pragma pack(1)//���ݽṹ
struct client_data{
	char type[2];//��������
	char sourse[20];//��������
	char day;//����ʱ��
};
#pragma pack()

#pragma pack(1)
struct server_data{
	char type[2];//��������
	char sourse[20];//��������
	unsigned short year;//���
	char rank[2];//����
	char day;//ѯ������
	char weather[3][2];//�����¶�
	char temp[44];//���
};
#pragma pack()

#endif
