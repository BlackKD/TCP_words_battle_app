#include"head.h"
#define LISTENQ 8


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

int set_server_data(int station,int connfd)
{
	server_data server;
	memset(&server,0,sizeof(server_data));
	switch(station)
	{
		case SREFUSE:	
				server.station = SREFUSE;
				break;
		case SRLINK:	
				server.station = SRLINK;
				break;
		case SGIVE_TABLE:	
				server.station = SGIVE_TABLE;
				strncpy(server.player[0].id,"baoqiong",10);
				strncpy(server.player[1].id,"xipeng",10);
				server.player[0].station = 1;
				server.player[1].station = 2;
				break;
		case SCREAT_GAME: 
				server.station = SCREAT_GAME;
				strncpy(server.another_id,"baoqiong",10);
				break;
		case SASK_PLAYER: 
				server.station = SASK_PLAYER;
				strncpy(server.another_id,"baoqiong",10);
				break;
		case SRETURN_BATTLE: 
				server.station = SRETURN_BATTLE;
				server.returndata.lifetime = 3;
				server.returndata.win_station = 1;
				strncpy(server.returndata.pk_stuff,"p1",10);
				break;
		case SRETURN_WINNER: 
				server.station = SRETURN_WINNER;
				server.game_station = 0;
				strncpy(server.game_over,"you lose!",10);
				break;
		case SCHAT:
				server.station = SCHAT;
				strncpy(server.saying,"HAHAHA!",10);
				break;
		default:
		{
			printf("input error!\n");
			return 0;
		}
		
	}
	#ifdef debug
	printf("server packet!:\n");
	printf("station:%d\n",server.station);
	printf("returndata:%d\n",server.returndata.lifetime);
	printf("returndata:%s\n",server.returndata.pk_stuff);
	printf("returndata:%d\n",server.returndata.win_station);
	printf("game_over:%s\n",server.game_over);
	printf("game_station:%d\n",server.game_station);
	printf("player_data:%d\n",server.player[0].station);
	printf("player_data:%s\n",server.player[0].id);
	printf("saying:%s\n\n",server.saying);
	sleep(1);
	#endif
	if(send(connfd,(char*)&server,sizeof(server_data),0)==0)
	{
		printf("send error\n");
		exit(0);
	}
	
	return 1;
}

void *print_thread(void *connfd)
{
	char buff[500];
	while(1)
    {
        if(kbhit())
        {
            pthread_mutex_lock(&gl_mutex);
            memset(&(buff),0,sizeof(buff));
            scanf("%s",buff);
            switch(buff[0])
			{
				case '0':set_server_data(SREFUSE,*(int *)connfd);break;
				case '1':set_server_data(SRLINK,*(int *)connfd);break;
				case '2':set_server_data(SGIVE_TABLE,*(int *)connfd);break;
				case '3':set_server_data(SCREAT_GAME,*(int *)connfd);break;
				case '4':set_server_data(SASK_PLAYER,*(int *)connfd);break;
				case '5':set_server_data(SRETURN_BATTLE,*(int *)connfd);break;
				case '6':set_server_data(SRETURN_WINNER,*(int *)connfd);break;
				case '7':set_server_data(SCHAT,*(int *)connfd);break;
				default:printf("error input:%s\n",buff);
				
			}
			printf("%s\n",buff);
            setbuf(stdin, NULL);        //clear stdin
            pthread_mutex_unlock(&gl_mutex);
        }
        usleep(100);
    }

    return NULL;
}


int main()
{

	int listenfd,connfd;
	//pid_t childpid;
	socklen_t clilen;
	//char buf[MAXLINE];
	struct sockaddr_in cliaddr,servaddr;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);
        //connect to the server
	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
    listen(listenfd,LISTENQ);//开始监听
	printf("server is runing\n");
	client_data client;
	memset(&client, 0, sizeof(struct client_data));
	pthread_mutex_init(&gl_mutex,NULL);
	
	
	//while(1)
	while(1)
	{
	clilen = sizeof(cliaddr);
	connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
	printf("recived request..\n");
	pthread_create(&threadID_to_get_player,NULL,&print_thread,(void *)&connfd);
	int n = 0;
	while((n = recv(connfd,&client,sizeof(struct client_data),0))>0)
	{
		pthread_mutex_lock(&gl_mutex);
		printf("station:%d\n",client.station);
		printf("id:%s\n",client.id);
		printf("pkid:%s\n",client.pkid);
		printf("pkreply:%d\n",client.pkreply);
		printf("pk_stuff:%s\n",client.pk_stuff);
		printf("saying:%s\n\n",client.saying);
		memset(&client, 0, sizeof(struct client_data));
		pthread_mutex_unlock(&gl_mutex);
	}
	if(n<0)
	{
		printf("error!\n");
		exit(1);
	}
	close(connfd);
	
	}
	close(listenfd);
}
		
		
		
		
