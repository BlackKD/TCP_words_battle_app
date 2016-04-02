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
		struct sockaddr_in servaddr;
			sockfd=socket(AF_INET, SOCK_STREAM, 0); //AF_INET for ipv4; SOCK_STREAM for byte stream
			if(sockfd < 0) {
				printf("Socket error!\n");
				return 0;
			}
			memset(&servaddr, 0, sizeof(struct sockaddr_in));
			servaddr.sin_family=AF_INET;
			servaddr.sin_addr.s_addr=inet_addr(SERV_ADDR);
			servaddr.sin_port=htons(SERV_PORT);
			//connect to the server
			if(connect(sockfd, (struct sockaddr* )&servaddr, sizeof(servaddr)) < 0) {//创建套接字连接服务器
				printf("Link Wrong!\n");
				exit(1);
			}
			else
				printf("Link Success!\n");
		
		
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

