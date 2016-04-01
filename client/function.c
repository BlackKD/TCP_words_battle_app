//#include<stdio.h>
#include"head.h"


int link_to_server(char *userid)//判断连接成功函数
{
	//发送用户名给服务器
	if(set_client_data(CLOGIN,userid))
	{
		printf("send success 0\n");
		
	}
	
	server_data server;
	memset(&server,0,sizeof(struct server_data));
	if(recv(sockfd,&server,sizeof(struct server_data),0)==0){
		printf("error\n");
	}else
	{
		if(server.station == 1)
		{
		return true;
		}
		else{
		return false;
		}
	}
	return false;

}

void main_function()//主界面交互函数
{
	
    char buff[1000];
	
	#ifdef debug
	printf("entered main function\n");
	#endif
	
    memset(&(buff),0,sizeof(buff));
    pthread_mutex_init(&gl_mutex,NULL);
	char ifask = 0;
	pthread_create(&threadID_to_get_player,NULL,&print_thread,(void *)&ifask);
	while(1)
    {
        if(kbhit())
        {
            pthread_mutex_lock(&gl_mutex);
            memset(&(buff),0,sizeof(buff));
            scanf("%s",buff);
            printf("No use input:%s\n",buff);
			if(buff[0]=='P'&&buff[1]=='K')
			{
				char buff2[10];
				strncpy(buff2,buff+2,10);
				pthread_cancel(threadID_to_get_player);
				return Try_to_get_challenge(buff2);
				printf("FUCK\n");
				
				
			}
			if(ifask==1)
			{
			
			if(buff[0] == '0')
			{
					//char buff[1];
					//buff[0] = '0';
					set_client_data(CREPLY_TO_C,"0");
					system("clear");
					set_client_data(CNEED_TABLE,NULL);
					ifask = 0;
			}
			else if(buff[0] == '1')
			{
					set_client_data(CREPLY_TO_C,"1");
					pthread_cancel(threadID_to_get_player);
					ifask = 0;
					return agree_to_play();

			}else
			{
					printf("Input no useless\n");
			}
			
			}
			
            setbuf(stdin, NULL);        //clear stdin
            pthread_mutex_unlock(&gl_mutex);
        }
        usleep(100);
    }
	
	

	//printf("%s\n",buffer);
}

void Try_to_get_challenge(char *buff)
{
	system("clear");
	printf("Try_to_get_challenge......\n");
	//发送挑战请求
	set_client_data(CCHOOSE_PLAYER,buff);
	//接收分析请求包如果请求失败
		server_data server;
	memset(&server,0,sizeof(struct server_data));
	if(recv(sockfd,&server,sizeof(struct server_data),0)==0)
	{
				printf("error\n");
				exit(0);
		
	}else
	{
		if(server.station != 3)
		{
			printf("sorry the other can't play with you!\n");
			
			sleep(1);
			system("clear;");
			printf("input any word to go back!\n");
			return main_function();
		}
		else{
			
			//如果请求成功并建立连接
			agree_to_play();
			
			
		}
	}	
	

}


void agree_to_play()
{
	system("clear");
	printf("game start!,give your first choose(p1 for stone,p2 for Scissors,p3 for cloth;inpu s+words to chat\n");
	pthread_mutex_init(&gametime_mutex,NULL);
	int if_wait = 0;
	pthread_create(&threadID_to_listengame,NULL,&play_time_thread,(void *)&if_wait);
	char buff2[1000];
	while(1)
    {
		if(if_wait == 3)
		{
			pthread_mutex_unlock(&gametime_mutex);
			pthread_cancel(threadID_to_listengame);
			return main_function();
		}
        if(kbhit())
        {
            pthread_mutex_lock(&gametime_mutex);
            memset(&(buff2),0,sizeof(buff2));
            scanf("%s",buff2);
            printf("input:%s\n",buff2);
			
			if(buff2[0]=='p'&&(buff2[1] == '1'|| buff2[1] == '2' || buff2[1] == '3'))
			{
				if(if_wait == 0)
				{
				if(set_client_data(CSHOW_STUFF,buff2))
				{
					printf("send succeed!\n");
					if_wait = 1;
				}
				}else
				{
					printf("you have given your bomb!please wait!\n");
				}
			}else if(buff2[0] == 's')
			{
				char buff3[500];
				memset(&(buff3),0,sizeof(buff3));
				strncpy(buff3,buff2+1,499);
				if(set_client_data(CCHAT,buff3))
				{
					printf("send saying succeed!\n");
				}
			}else if(buff2[0] == '#')
			{
				if(set_client_data(CEXIT,buff2))
				{
					
					
					printf("send ending succeed!\n");
					pthread_mutex_unlock(&gametime_mutex);
					pthread_cancel(threadID_to_listengame);
					printf("ready to go back\n");
					//return;
					return main_function();

			}
			}
			else
			{
				printf("input no using!\n");
			}
			//printf("%s\n",buff);//可以分类去发送信号
			
            setbuf(stdin, NULL);        //clear stdin
            pthread_mutex_unlock(&gametime_mutex);
        }
        usleep(100);
    }
			
			
		
}


void *print_thread(void *para)
{	
	server_data server;
	memset(&server,0,sizeof(struct server_data));
	
	
    while(1)
    {
		
		usleep(100);
		int t  = 0;
		if(!(t = recv(sockfd,&server,sizeof(struct server_data),0)))
		{
			printf("error  t = %d\n",t);
			exit(1);
		}	
		else if(server.station == SGIVE_TABLE)
		{
			pthread_mutex_lock(&gl_mutex);
			system("clear");
			printf("Login in Succeed\n");
			printf("(ITEMS:PK+(Player ID) to challenge)");
            printf("your friend is here!\n thread ID:%d\n",(int)pthread_self());
			int i = 0;
			for(i= 0;i< 10;i++)
			{
				if(server.player[i].station == ONLINE)
				{
					printf("%d:%s, Waiting\n",i,server.player[i].id);
				}
				else if(server.player[i].station == FIGHTING) 
				{
					printf("%d:%s, busy",i,server.player[i].id);
				}
			}
            pthread_mutex_unlock(&gl_mutex);
        }else if(server.station == SASK_PLAYER)
		{
			pthread_mutex_lock(&gl_mutex);
			printf("You receive a challenge From %s!(input 1 to play 0 to refuse)\n",server.another_id);
			//setbuf(stdin, NULL);        //clear stdin
			*(char *)para = 1;
			pthread_mutex_unlock(&gl_mutex);

		}
		
            
	}
	
    return NULL;
}

void *play_time_thread(void *para)
{

	//int timelimit = 20;
	
	set_client_data(CNEED_TABLE,NULL);
	server_data server;
	memset(&server,0,sizeof(struct server_data));
	
    while(1)
    {
        usleep(100);

            struct timeval timeout={15,0};//3s
			setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
			setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
		int recvtime = recv(sockfd,&server,sizeof(struct server_data),0);
        if(recvtime > 0 )//可以换成抓包
        {	
			if(server.station == SRETURN_BATTLE)
			{
				pthread_mutex_lock(&gametime_mutex);
				printf("---------------------THIS TASK----------------------\n");
				//printf("you")
				if(server.returndata.win_station == 1)
				{
					printf("You win! the other show %s,now you have %d lives\n",server.returndata.pk_stuff,(int)server.returndata.lifetime);
				}else if(server.returndata.win_station == 0)
				{
					printf("You lose! the other show %s,now you have %d lives\n",server.returndata.pk_stuff,(int)server.returndata.lifetime);
				}else if(server.returndata.win_station == 2)
				{
					printf("You Tie! the other show %s,now you have %d lives\n",server.returndata.pk_stuff,(int)server.returndata.lifetime);
				}
				printf("Give another choosen!\n");
				*(int *)para = 0;
				pthread_mutex_unlock(&gametime_mutex);
				
			}
			else if(server.station == SCHAT)
			{
				pthread_mutex_lock(&gametime_mutex);
				printf("there are message from sever; thread ID:%d\n",(int)pthread_self());
				printf("The other says:%s\n",server.saying);
				pthread_mutex_unlock(&gametime_mutex);
			}
			else if(server.station == SRETURN_WINNER)
			{
				pthread_mutex_lock(&gametime_mutex);
				printf("*********************THE FINAL************************\n");
				if(server.game_station == 1)
				{
					printf("FINAL  YOU  WIN!!!!!!!!!!!!!!!!\n");
				}
				else
				{
					printf("FINAL  YOU  LOSE!!!!!!!!!!!!!!!!\n");
				}
				printf("%s input any word to goback\n",server.game_over);
				sleep(1);
				*(int *)para = 3;
				pthread_mutex_unlock(&gametime_mutex);
			}

		
            
        }
		
        else if(*(int *)para == 1)
        {
            		printf("Lost server!\n");
					sleep(1);
					//pthread_mutex_unlock(&gametime_mutex);
					*(int *)para = 3;
        }
/*		#ifdef debug
		printf("%d\n,%d",timelimit,*(int *)para);
		#endif
		if(*(int *)para == 1)
			{
				pthread_mutex_lock(&gametime_mutex);
				timelimit --;
				
				if(timelimit == 0)
				{
					printf("Lost server!\n");
					sleep(1);
					//pthread_mutex_unlock(&gametime_mutex);
					*(int *)para = 3;
				}
				pthread_mutex_unlock(&gametime_mutex);
			}
       */ 
    }

    return NULL;
}




int set_client_data(int station,char *buff)
{
	client_data client;
	memset(&client,0,sizeof(client_data));
	switch(station)
	{
		case CLOGIN:	
				client.station = CLOGIN;
				strncpy(client.id,buff,10);
				break;
		case CNEED_TABLE:	
				client.station = CNEED_TABLE;
				strncpy(client.id,userid,10);
				break;
		case CCHOOSE_PLAYER:	
				client.station = CCHOOSE_PLAYER;
				strncpy(client.id,userid,10);
				strncpy(client.pkid,buff,10);
				break;
		case CREPLY_TO_C: 
				client.station = CREPLY_TO_C;
				if(buff[0] == '1')
					client.pkreply = 1;
				else
					client.pkreply = 0;
				strncpy(client.id,userid,10);
				break;
		case CSHOW_STUFF: 
				client.station = CSHOW_STUFF;
				strncpy(client.id,userid,10);
				strncpy(client.pk_stuff,buff,10);
				break;
		case CCHAT: 
				client.station = CCHAT;
				printf("here:%d\n",client.station);
				strncpy(client.saying,buff,500);
				strncpy(client.id,userid,10);
				break;
		case CEXIT: 
				client.station = CEXIT;
				break;
		default:
		{
			printf("input error!\n");
			return 0;
		}
		
	}
	#ifdef debug
	//printf("your packet\n");
	//printf("%d:\n%s\n%s\n%d\n%s\n%s\n",client.station,client.id,client.pkid,client.pkreply,client.pk_stuff,client.saying);
	//sleep(2);
	#endif
	
	if(send(sockfd,(char*)&client,sizeof(client_data),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	return 1;
}

int kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}



