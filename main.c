/*program: c web server
author:heat nan
language:c
test: firefox/google
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include "http.h"
#include <errno.h>
#define BUFFER_SIZE 3000
#define PORT 8000

void *client_thread(void *arg);

int main()
{
	char buffer[BUFFER_SIZE];
	char* p_buffer;

	struct sockaddr_in server_addr,client_addr;
	socklen_t addr_len=sizeof(struct sockaddr_in);
	
	int server_sockfd,client_sockfd;
	pthread_t client;

	server_sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(server_sockfd<0) 
        {	printf("create socket failed!\n");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=INADDR_ANY;

	int enable = 1;
	if(setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&enable,sizeof(int)) < 0)
	{
		printf("setsockopt failed\n");
		exit(EXIT_FAILURE);
	}


	if(bind(server_sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))!=0){
		printf("bind error!\n");
		printf("msg:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	else
	
	printf("bind success!\n");
	
	if(listen(server_sockfd,5)==-1)
	{
		printf("listen failed!\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("start listening!\n");
	}
	
	while(1)
	{
		char c;
	
		client_sockfd=accept(server_sockfd,(struct sockaddr*)&client_addr,&addr_len);
	
		if(client_sockfd==-1)
		{
			printf("accept failed!\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("\n\none client connect!\n");
		}	        
 	
		memset(buffer,0,sizeof(buffer));
	
		int recv_len=recv(client_sockfd,buffer,sizeof(buffer),0);
		printf("   %d bytes recved!   \n",recv_len);
		printf("recv_buffer is %s\n",buffer);
		printf("the client fd is = %d\n",client_sockfd);
		p_buffer=buffer;


		if(recv_len > 800)
		{
			close(client_sockfd);
			printf("invald request\n");
			continue;
		}
		
	
		if( recv_len > 0 && recv_len < BUFFER_SIZE )
		{
			char cmd[10];
			char version[10];
			char get_request[3000];
			memset(get_request,0,sizeof(get_request));
			
			sscanf(buffer,"%s %s %s",cmd,get_request,version);

			printf("get_request is %s\n",get_request);

			cfd *p_cfg=malloc(sizeof(cfd));
			p_cfg->cmd=cmd;
			p_cfg->fd=client_sockfd;
			p_cfg->recv_buf=get_request;
			
			if(pthread_create(&client,NULL,&client_thread,p_cfg)!=0)
			{
				printf("create pthread failed!\n");
				close(p_cfg->fd);
				continue;
			}
			
			pthread_detach(client);
		}
		else
		{
			printf("..................here is a request not success!................\n");
		}                    
	}
	close(server_sockfd);
	return 0;
}



void *client_thread(void *arg)
{
	cfd lcfd;
	if(arg!=NULL)
	{
		memcpy(&lcfd,arg,sizeof(cfd));
		free(arg);
	}
	else
	{
		return NULL;
	}
	if(strcmp(lcfd.cmd,"GET") == 0)
	{
		handle_get(lcfd);
	}
	else if(strcmp(lcfd.cmd,"POST") == 0)
	{
		//TO DO
	}
	
}



