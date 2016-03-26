#include"head.h"
//#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

int main()
{	
	//int sockfd;

	
		system("clear");
		printf("Welcome to NJUCE finger-guessing game Program!\n");
			/*struct sockaddr_in servaddr;
			sockfd=socket(AF_INET, SOCK_STREAM, 0); //AF_INET for ipv4; SOCK_STREAM for byte stream 
			memset(&servaddr, 0, sizeof(struct sockaddr_in));
			servaddr.sin_family=AF_INET;
			servaddr.sin_addr.s_addr=inet_addr("114.212.190.188");
			servaddr.sin_port=htons(SERV_PORT);
			//connect to the server
			if(!connect(sockfd, (struct sockaddr* )&servaddr, sizeof(servaddr)))//创建套接字连接服务器
			return true;
			else
			return false;
				system("clear");
		printf("Link Wrong!\n");
		*/
		char userid[20];
		int first_send_name = 0;
		do
		{
			if(first_send_name!=0)
			{
				printf("this name has been used\n");
				
			}
			first_send_name = 1;
			memset(userid,0,20);
			gets(userid);
		}while(!link_to_server(userid));
		main_function();

	return 0;
}

