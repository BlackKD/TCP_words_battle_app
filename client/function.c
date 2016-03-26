//#include<stdio.h>
#include"head.h"


int link_to_server(char *userid)//判断连接成功函数
{
	//发送用户名给服务器
	
	//发送并解析服务器工作
	return true;

}

void main_function()//主界面交互函数
{
	printf("Login in Succeed\n(c)cls,(#)exit\n");
	char buffer[1000];
	//gets(buffer);
	scanf("%s",buffer);
	if(!strncmp("c",buffer,strlen(buffer)))
	{
		system("clear");
		printf("Welcome to NJUCE Weather Forecast Demo Program!\n");
		main_function();
		//printf("Please input City Name in Chinese pingyin(e.g.nanjing or beijing)\n(c)cls,(#)exit\n");

	}
	else if(!strncmp("#",buffer,strlen(buffer)))
	{
		exit(0);
	}
	else
	{
		if(send_city_to_server(buffer,strlen(buffer)))
		{	
			system("clear");
			Haved_find(buffer,strlen(buffer));
		}
		else
		{
			printf("Sorry, Server does not have weather information for city baotou!\n");
			printf("Welcome to NJUCE Weather Forecast Demo Program!\n");
			main_function();
		}
	}

	//printf("%s\n",buffer);
}

int send_city_to_server(char *buffer,int len)//将用户请求的城市发给服务器，并根据服务器的反映决定是否跳转到第二个界面
{
	//printf("%s %d,in send_to_server\n",buffer,len);

	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=1;
	client.type[1]=0;
	strncpy(client.sourse,buffer,len);
	/*printf("%s\n",client.sourse);
	printf("%x %x ",client.type[0],client.type[1]);
	int i = 0;
	for( i = 0 ; i < 21;i++)
	{
		printf("%x ",client.sourse[i]);
	}
	printf("here\n");*/
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
	//printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	/*else
	{
		printf("%x %x ",server.type[0],server.type[1]);
		for(i = 0 ; i < 20 ;i++)
		{
			printf("%x ",server.sourse[i]);
		}	
		for(i = 0 ; i < 4;i++)
		{
			printf("%x ",server.rank[i]);
		}
		printf("%x ",server.day);
		
		int j = 0;
		for(i = 0 ;i < 3;i++)
			for(j = 0; j < 2;j++)
			{
				printf("%x ",server.weather[i][j]);
			}
	}
	printf("there\n");*/
	if(server.type[0]==1)
	return true;
	else
	return false;

}

void Haved_find(char *buffer,int len)//在确定服务器有该城市的服务进行界面转换
{
	//system("clear");
	printf("Please enter the given number to query\n1.today\n2.three days from today\n3.custom day by yourself\n(r)back,(c)cls,(#)exit\n===================================\n");
	char temp;
	while(1)
	{
	scanf("%s",&temp);
	if(temp=='r')
	{
		system("clear");
		main_function();
	}
	else if(temp=='c')
	{
		system("clear");
		Haved_find(buffer,len);
	}
	else if(temp == '#')
	{
		exit(0);
	}
	else if(temp=='1')
	{
		need_today_weather(buffer,len);
	}
	else if(temp=='2')
	{
		need_threeday_weather(buffer,len);
	}
	else if(temp == '3')
	{
		need_customday_weather(buffer,len);
	}
	else
	{
		printf("input error\n");
	}
	}
}

void need_today_weather(char *buffer,int len)//进行第一天的处理
{
	//time_t timep;
	//struct tm *p;
	//time(&timep);
	//p = localtime(&timep);
	
	
	//printf("%s %d,in today_to_server\n",buffer,len);

	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=1;
	client.day = 1;
	strncpy(client.sourse,buffer,len);
	/*printf("%s\n",client.sourse);
	printf("%x %x ",client.type[0],client.type[1]);
	int i = 0;
	for( i = 0 ; i < 20;i++)
	{
		printf("%x ",client.sourse[i]);
	}
	printf("%x ",client.day);
	printf("here\n");*/
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
	//printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	/*else
	{
		printf("%x %x ",server.type[0],server.type[1]);
		for(i = 0 ; i < 20 ;i++)
		{
			printf("%x ",server.sourse[i]);
		}		
		for(i = 0 ; i < 4;i++)
		{
			printf("%x ",server.rank[i]);
		}
		printf("%x ",server.day);
		
		int j = 0;
		for(i = 0 ;i < 3;i++)
			for(j = 0; j < 2;j++)
			{
				printf("%x ",server.weather[i][j]);
			}
	}
	printf("there\n");*/
	if(server.type[0]==3&&server.type[1]==0x41)
	{
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
		//printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		printf("Today's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);
		//return true;
	}
	//return true;
	else
	printf("look for today weather fail\n");
}

void need_threeday_weather(char *buffer,int len)
{
	// time_t timep;
	// struct tm *p;
	// time(&timep);
	// p = localtime(&timep);
	 
	// printf("%s %d, three day_to_server\n",buffer,len);

	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=2;
	client.day = 3;
	strncpy(client.sourse,buffer,len);
//	printf("%s\n",client.sourse);
//	printf("%x %x ",client.type[0],client.type[1]);
	//int i = 0;
	//for( i = 0 ; i < 20;i++)
	//{
	//	printf("%x ",client.sourse[i]);
	//}
	//printf("%x ",client.day);
	//printf("here\n");
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
	//printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	/*else
	{
		printf("%x %x ",server.type[0],server.type[1]);
		for(i = 0 ; i < 20 ;i++)
		{
			printf("%x ",server.sourse[i]);
		}		
		for(i = 0 ; i < 4;i++)
		{
			printf("%x ",server.rank[i]);
		}
		printf("%x ",server.day);
		
		int j = 0;
		for(i = 0 ;i < 3;i++)
			for(j = 0; j < 2;j++)
			{
				printf("%x ",server.weather[i][j]);
			}
	}
	printf("there\n");*/
	if(server.type[0]==3&&server.type[1]==0x42)
	{
		printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		//printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
			printf("The 1th day's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);

		if(server.weather[1][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[1][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[1][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[1][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[1][0] ==0x0)
			strncpy(weather,"shower",6);
			printf("The 2th day's Weather is: %s; Temp:%d\n",weather,server.weather[1][1]);

		if(server.weather[2][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[2][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[2][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[2][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[2][0] ==0x0)
			strncpy(weather,"shower",6);
			printf("The 3th day's Weather is: %s; Temp:%d\n",weather,server.weather[2][1]);
		//return true;
	}
	//return true;
	else
	printf("look for today weather fail\n");


}

void need_customday_weather(char *buffer,int len)//用户自主输入查询天数
{
//	printf("Please enter the day number(below 10, e.g. 1 means today):");
	char temp[10];
//	scanf("%s",&temp);

	// time_t timep;
	 //struct tm *p;
	 //time(&timep);			         
	 //p = localtime(&timep);
	// printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
	while(1)
	{
	 printf("Please enter the day number(below 10, e.g. 1 means today):");
	 scanf("%s",temp);
	 if(!strcmp("1",temp))
	 {
	//	printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
	//	printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		 //printf("send 1 to server\n");
		need_today_weather(buffer,len);
		break;
	 }
	 else if(!strcmp("2",temp))
	 {
	//printf("%s %d,in 2_to_server\n",buffer,len);
	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=1;
	client.day = 2;
	strncpy(client.sourse,buffer,len);
	//printf("%s\n",client.sourse);
	//printf("%x %x ",client.type[0],client.type[1]);
	//int i = 0;
	//for( i = 0 ; i < 20;i++)
	//{
	//	printf("%x ",client.sourse[i]);
	//}
	//printf("%x ",client.day);
	//printf("here\n");
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
	//printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	//else
	//{
	//	printf("%x %x ",server.type[0],server.type[1]);
	//	for(i = 0 ; i < 20 ;i++)
	//	{
	//		printf("%x ",server.sourse[i]);
	//	}		
	//	for(i = 0 ; i < 4;i++)
	//	{
	//		printf("%x ",server.rank[i]);
	//	}
	//	printf("%x ",server.day);
	//	
	//	int j = 0;
	//	for(i = 0 ;i < 3;i++)
	//		for(j = 0; j < 2;j++)
	//		{
	//			printf("%x ",server.weather[i][j]);
	//		}
	//}
	//printf("there\n");
	if(server.type[0]==3&&server.type[1]==0x41)
	{
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
		printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		//printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		printf("The 2th's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);
		//return true;
	}
	//return true;
	else
	printf("Sorry, no given day's weather information for city %s!\n",buffer);
						break;
	 }
	 else if(!strcmp("3",temp))
	 {
//	printf("%s %d,in 3_to_server\n",buffer,len);
	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=1;
	client.day = 3;
	strncpy(client.sourse,buffer,len);
	//printf("%s\n",client.sourse);
	//printf("%x %x ",client.type[0],client.type[1]);
	//int i = 0;
	//for( i = 0 ; i < 20;i++)
	//{
	//	printf("%x ",client.sourse[i]);
	//}
	//printf("%x ",client.day);
	//printf("here\n");
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
//	printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	/*else
	{
		printf("%x %x ",server.type[0],server.type[1]);
		for(i = 0 ; i < 20 ;i++)
		{
			printf("%x ",server.sourse[i]);
		}		
		for(i = 0 ; i < 4;i++)
		{
			printf("%x ",server.rank[i]);
		}
		printf("%x ",server.day);
		
		int j = 0;
		for(i = 0 ;i < 3;i++)
			for(j = 0; j < 2;j++)
			{
				printf("%x ",server.weather[i][j]);
			}
	}
	printf("there\n");*/
	if(server.type[0]==3&&server.type[1]==0x41)
	{
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
		printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		//printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		printf("The 3th's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);
		//return true;
	}
	//return true;
	else
	printf("Sorry, no given day's weather information for city %s!\n",buffer);
						break;
	 }
	 else if(!strcmp("4",temp))
	 {
//	printf("%s %d,in 4_to_server\n",buffer,len);
	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=1;
	client.day = 4;
	strncpy(client.sourse,buffer,len);
	//printf("%s\n",client.sourse);
	//printf("%x %x ",client.type[0],client.type[1]);
	//int i = 0;
	//for( i = 0 ; i < 20;i++)
	//{
	//	printf("%x ",client.sourse[i]);
	//}
	//printf("%x ",client.day);
	//printf("here\n");
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
//	printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	/*else
	{
		printf("%x %x ",server.type[0],server.type[1]);
		for(i = 0 ; i < 20 ;i++)
		{
			printf("%x ",server.sourse[i]);
		}		
		for(i = 0 ; i < 4;i++)
		{
			printf("%x ",server.rank[i]);
		}
		printf("%x ",server.day);
		
		int j = 0;
		for(i = 0 ;i < 3;i++)
			for(j = 0; j < 2;j++)
			{
				printf("%x ",server.weather[i][j]);
			}
	}
	printf("there\n");*/
	if(server.type[0]==3&&server.type[1]==0x41)
	{
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
		printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		//printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		printf("The 4th's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);
		//return true;
	}
	//return true;
	else
	printf("Sorry, no given day's weather information for city %s!\n",buffer);
						break;
	 }
	 else if(!strcmp("5",temp))
	 {                 
		 	//printf("%s %d,in 5_to_server\n",buffer,len);
	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=1;
	client.day = 5;
	strncpy(client.sourse,buffer,len);
	/*printf("%s\n",client.sourse);
	printf("%x %x ",client.type[0],client.type[1]);
	int i = 0;
	for( i = 0 ; i < 20;i++)
	{
		printf("%x ",client.sourse[i]);
	}
	printf("%x ",client.day);
	printf("here\n");*/
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
	//printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	/*else
	{
		printf("%x %x ",server.type[0],server.type[1]);
		for(i = 0 ; i < 20 ;i++)
		{
			printf("%x ",server.sourse[i]);
		}		
		for(i = 0 ; i < 4;i++)
		{
			printf("%x ",server.rank[i]);
		}
		printf("%x ",server.day);
		
		int j = 0;
		for(i = 0 ;i < 3;i++)
			for(j = 0; j < 2;j++)
			{
				printf("%x ",server.weather[i][j]);
			}
	}
	printf("there\n");*/
	if(server.type[0]==3&&server.type[1]==0x41)
	{
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
		printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		//printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		printf("The 5th's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);
		//return true;
	}
	//return true;
	else
	printf("Sorry, no given day's weather information for city %s!\n",buffer);
				break;
	 }
	 else if(!strcmp("6",temp))
	 {
	//printf("%s %d,in 6_to_server\n",buffer,len);
	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=1;
	client.day = 6;
	strncpy(client.sourse,buffer,len);
	//printf("%s\n",client.sourse);
	//printf("%x %x ",client.type[0],client.type[1]);
	//int i = 0;
	//for( i = 0 ; i < 20;i++)
	//{
	//	printf("%x ",client.sourse[i]);
	//}
	//printf("%x ",client.day);
	//printf("here\n");
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
	//printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	/*else
	{
		printf("%x %x ",server.type[0],server.type[1]);
		for(i = 0 ; i < 20 ;i++)
		{
			printf("%x ",server.sourse[i]);
		}		
		for(i = 0 ; i < 4;i++)
		{
			printf("%x ",server.rank[i]);
		}
		printf("%x ",server.day);
		
		int j = 0;
		for(i = 0 ;i < 3;i++)
			for(j = 0; j < 2;j++)
			{
				printf("%x ",server.weather[i][j]);
			}
	}
	printf("there\n");*/
	if(server.type[0]==3&&server.type[1]==0x41)
	{
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
	printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		//	printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		printf("The 6th's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);
		//return true;
	}
	//return true;
	else
	printf("Sorry, no given day's weather information for city %s!\n",buffer);
						break;
	 }
	 else if(!strcmp("7",temp))
	 {
	//printf("%s %d,in 7_to_server\n",buffer,len);
	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=1;
	client.day = 7;
	strncpy(client.sourse,buffer,len);
	//printf("%s\n",client.sourse);
	//printf("%x %x ",client.type[0],client.type[1]);
	//int i = 0;
	//for( i = 0 ; i < 20;i++)
	//{
	//	printf("%x ",client.sourse[i]);
	//}
	//printf("%x ",client.day);
	//printf("here\n");
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
	//printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	//else
	//{
	//	printf("%x %x ",server.type[0],server.type[1]);
	//	for(i = 0 ; i < 20 ;i++)
	//	{
	//		printf("%x ",server.sourse[i]);
	//	}		
	//	for(i = 0 ; i < 4;i++)
	//	{
	//		printf("%x ",server.rank[i]);
	//	}
	//	printf("%x ",server.day);
	//	
	//	int j = 0;
	//	for(i = 0 ;i < 3;i++)
	//		for(j = 0; j < 2;j++)
	//		{
	//			printf("%x ",server.weather[i][j]);
	//		}
	//}
	//printf("there\n");
	if(server.type[0]==3&&server.type[1]==0x41)
	{
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
	printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		//	printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		printf("The 7th's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);
		//return true;
	}
	//return true;
	else
	printf("Sorry, no given day's weather information for city %s!\n",buffer);
						break;
	 }
	 else if(!strcmp("8",temp))
	 {
	//printf("%s %d,in 8_to_server\n",buffer,len);
	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=1;
	client.day = 8;
	strncpy(client.sourse,buffer,len);
	//printf("%s\n",client.sourse);
	//printf("%x %x ",client.type[0],client.type[1]);
	//int i = 0;
	//for( i = 0 ; i < 20;i++)
	//{
	//	printf("%x ",client.sourse[i]);
	//}
	//printf("%x ",client.day);
	//printf("here\n");
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
	//printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	//else
	//{
	//	printf("%x %x ",server.type[0],server.type[1]);
	//	for(i = 0 ; i < 20 ;i++)
	//	{
	//		printf("%x ",server.sourse[i]);
	//	}		
	//	for(i = 0 ; i < 4;i++)
	//	{
	//		printf("%x ",server.rank[i]);
	//	}
	//	printf("%x ",server.day);
	//	
	//	int j = 0;
	//	for(i = 0 ;i < 3;i++)
	//		for(j = 0; j < 2;j++)
	//		{
	//			printf("%x ",server.weather[i][j]);
	//		}
	//}
	//printf("there\n");
	if(server.type[0]==3&&server.type[1]==0x41)
	{
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
	printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		//printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		printf("The 8th's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);
		//return true;
	}
	//return true;
	else
	printf("Sorry, no given day's weather information for city %s!\n",buffer);
						break;
	 }
	 else if(!strcmp("9",temp))
	 {
	//printf("%s %d,in 9_to_server\n",buffer,len);
	struct client_data client;//for the client data
	memset(&client, 0 , sizeof(client));
	struct server_data server;
	//strncpy(client.type,"")
	//send thing
	client.type[0]=2;
	client.type[1]=1;
	client.day = 9;
	strncpy(client.sourse,buffer,len);
	/*printf("%s\n",client.sourse);
	printf("%x %x ",client.type[0],client.type[1]);
	int i = 0;
	for( i = 0 ; i < 20;i++)
	{
		printf("%x ",client.sourse[i]);
	}
	printf("%x ",client.day);
	printf("here\n");*/
	//while(1);
	if(send(sockfd,(char*)&client,sizeof(client),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	//struct server_data server;
	//char recvline[MAXLINE];
	memset(&server, 0 , sizeof(server));
	//printf("test\n");
	if(recv(sockfd,&server,sizeof(server),0)==0)
	{
		printf("error\n");
	}
	/*else
	{
		printf("%x %x ",server.type[0],server.type[1]);
		for(i = 0 ; i < 20 ;i++)
		{
			printf("%x ",server.sourse[i]);
		}		
		for(i = 0 ; i < 4;i++)
		{
			printf("%x ",server.rank[i]);
		}
		printf("%x ",server.day);
		
		int j = 0;
		for(i = 0 ;i < 3;i++)
			for(j = 0; j < 2;j++)
			{
				printf("%x ",server.weather[i][j]);
			}
	}
	printf("there\n");*/
	if(server.type[0]==3&&server.type[1]==0x41)
	{
		char weather[10]={0};
		if(server.weather[0][0] ==0x1)
			strncpy(weather,"clear",5);
		if(server.weather[0][0] ==0x2)
			strncpy(weather,"cloudy",6);
		if(server.weather[0][0] ==0x3)
			strncpy(weather,"rain",4);
		if(server.weather[0][0] ==0x4)
			strncpy(weather,"fog",3);
		if(server.weather[0][0] ==0x0)
			strncpy(weather,"shower",6);
		printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,htons(server.year),server.rank[0],server.rank[1]);
		//printf("City:%s  Today is: %d/%d/%d Weather information is as follows:\n",buffer,1900+p->tm_year,1+p->tm_mon,p->tm_mday);
		printf("The 9th's Weather is: %s; Temp:%d\n",weather,server.weather[0][1]);
		//return true;
	}
	//return true;
	else
	printf("Sorry, no given day's weather information for city %s!\n",buffer);
						  break;
	
	 }else
	 {
		 printf("Input error\n");
	 }
	}
}
