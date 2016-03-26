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
	
    char buff[1000];

    memset(&(buff),0,sizeof(buff));
    pthread_mutex_init(&gl_mutex,NULL);
	pthread_create(&threadID_to_get_player,NULL,&print_thread,NULL);
	while(1)
    {
        if(kbhit())
        {
            pthread_mutex_lock(&gl_mutex);
            memset(&(buff),0,sizeof(buff));
            scanf("%s",buff);
            printf("input:%s\n",buff);
			if(buff[0]=='P'&&buff[1]=='K')
			{
				pthread_cancel(threadID_to_get_player);
				Try_to_get_challenge(buff);
				printf("FUCK\n");
				
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
	
	//如果请求失败
/*	printf("faile to connect\n");
	sleep(3);
	pthread_mutex_unlock(&gl_mutex);
	pthread_create(&threadID_to_get_player,NULL,&print_thread,NULL);
*/	
	//如果请求成功并建立连接
	system("clear");
	printf("game start!\n");
	pthread_mutex_init(&gametime_mutex,NULL);
	pthread_create(&threadID_to_listengame,NULL,&play_time_thread,NULL);
	char buff2[1000];
	while(1)
    {
        if(kbhit())
        {
            pthread_mutex_lock(&gametime_mutex);
            memset(&(buff2),0,sizeof(buff2));
            scanf("%s",buff2);
            printf("input:%s\n",buff2);
			
			//printf("%s\n",buff);//可以分类去发送信号
			
            setbuf(stdin, NULL);        //clear stdin
            pthread_mutex_unlock(&gametime_mutex);
        }
        usleep(100);
    }

}

void *print_thread(void *para)
{
    int timestamp=0;

    while(1)
    {
        usleep(100);

        pthread_mutex_lock(&gl_mutex);
        if(timestamp>=5*100000)//可以换成抓包
        {	
			system("clear");
			printf("Login in Succeed\n");
			printf("(ITEMS:PK+(Player ID) to challenge)");
            printf("your friend is here!\n thread ID:%d\n",(int)pthread_self());
            timestamp=0;
        }
        else
        {
            timestamp+=100;
        }
        pthread_mutex_unlock(&gl_mutex);
    }

    return NULL;
}

void *play_time_thread(void *para)
{
    int timestamp=0;

    while(1)
    {
        usleep(100);

        pthread_mutex_lock(&gametime_mutex);
        if(timestamp>=5*100000)//可以换成抓包
        {	
            printf("there are message from sever; thread ID:%d\n",(int)pthread_self());
            timestamp=0;
        }
        else
        {
            timestamp+=100;
        }
        pthread_mutex_unlock(&gametime_mutex);
    }

    return NULL;
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



