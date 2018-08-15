#include<stdio.h>
#include<string.h>
#include"http.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#define BUFFER_SIZE 3000
/*
	function:get the request file name
	

*/
char* get_file_type(char *file_name)
{
	
	char *p_file_name=NULL;
	char name[3000];
	char file_type[300];
	char *p_type=(char *)malloc(3000*sizeof(char));
	char *unknow="plain";
	int k=0;
	
	if(p_type==NULL)
	{
		printf("malloc p_type failed!\n");
	}
	strcpy(name,file_name);

	p_file_name=name;
	
	memset(file_type,0,sizeof(file_type));
	if(p_file_name==NULL)
		return NULL;
	
    while(*p_file_name!='.'&&*p_file_name!='\0')
	{
		p_file_name++;
	}
	if(*p_file_name=='\0')
	{
		printf("I have no type!\n");
		strcpy(p_type,unknow);
		return p_type;
	}
	p_file_name++;
	k=0;

	while(*p_file_name!='\0')
	{
		file_type[k++]=*p_file_name;
		p_file_name++;
	}
	file_type[k]='\0';
	strcpy(p_type,file_type);
    printf("The request file format is:%s \n",file_type);
	
	return p_type;
}
char* get_file_path(char *file_name)
{
	char name[1000];
	char path[2000]="/home/heatnan/Smart_home/www";
	char *p_path=(char *)malloc(3000*sizeof(char));
	memset(name,0,sizeof(name));
	
	if(path==NULL)
	{
		printf("malloc path failed!\n");
	}
	
	strcpy(name,file_name);
		
	if(file_name==NULL)
		return NULL;

	strcat(path,name);
	printf("path =%s\n",path);
	strcpy(p_path,path);
	printf("path in fucntion = %p\n",p_path);
	return p_path;
	
	
}
void send_index_file(int socket_fd)
{
	char *p_type="html";
	char *p_path="/home/heatnan/Smart_home/www/index.html";
	send_file(p_path,socket_fd,p_type);
	
	
	
}
void handle_get(cfd lcfd)
{
	cfd hcfd;
	
	if(lcfd.recv_buf!=NULL)
	{
		memcpy(&hcfd,&lcfd,sizeof(cfd));
	}
	else
	{
		close(lcfd.fd);
		return ;
	}
	printf("lcfd %s  fd= %d\n",hcfd.recv_buf,hcfd.fd);	
	
	char *p_path=NULL;   
	char *p_type=NULL;   
	
	p_path=get_file_path(hcfd.recv_buf);
	
	p_type=get_file_type(hcfd.recv_buf);
	
	// judage ruquest file is regular file , ensure not dir ,not exists
	if(!is_regular_file(p_path))
	{
		printf("the request file  is not found\n");
		show_exception(404,hcfd.fd);
		free(p_path);
		free(p_type);
		close(hcfd.fd);		
		return;
	}
	
	send_file(p_path,hcfd.fd,p_type);
	
	return ;

}
void send_file(char *file_path,int socket_fd,char *file_type)
{
	int fd;
	char send_buffer[BUFFER_SIZE];
	char* p_path=NULL;
	p_path=file_path;
	fd=open(file_path,O_RDONLY);
	if(fd<0)
	{
		printf("***********Open file failed!***************\n");
		close(socket_fd);
		return ;
		
	}
	memset(send_buffer,0,sizeof(send_buffer));
        
	printf("file type %s\n",file_type);
  
		 sprintf(send_buffer,"HTTP/1.1 200 OK\r\n"\
                             "Conten-Type:%s\r\n"\
                             "\r\n",file_type);
     		printf("send http head!\n");

	
	
        int file_length=strlen(send_buffer);
        do{
        	if(write(socket_fd,send_buffer,file_length)<0)
		{
                	close(fd);
                	break;
                }
          }while((file_length=read(fd,send_buffer,sizeof(send_buffer)))>0);
	           	free(p_path);
			free(file_type);
			close(fd);
			close(socket_fd);
	printf("p_path=%p file_type=%p\n",p_path,file_type);
                	
}


int is_regular_file(const char* path)
{
	struct stat path_stat;
	stat(path,&path_stat);
	return S_ISREG(path_stat.st_mode);
}


int check_file_exists(const char *path)
{
	return 1;	
}


void show_exception(int type,int socket_fd)
{
	char msg[1000];
	memset(msg,0,sizeof(msg));
	switch(type)
	{
		case 404:
			strcpy( msg , "HTTP/1.1 404 NOT FOUND\r\n");
			break;
		default:
			strcpy(msg , "unknow error");
			break;
	}
	write(socket_fd,msg,strlen(msg));
	close(socket_fd);
	

}
