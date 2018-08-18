#include<stdio.h>
#include<string.h>
#include"http.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#define BUFFER_SIZE 3000
	

const char* get_file_type(const char *file_name)
{
	const char *dot = strrchr(file_name,'.');

	if(!dot)
	{
		return "";
	}
	
	return dot + 1;
}

void get_file_path(const char *file_name,char* file_path)
{
	char name[1000];
	memset(name,0,sizeof(name));
	strcpy(name,file_name);
	
	if(strlen(name) <= 0)
	{
		return ;
	}

	char cur_path[PATH_MAX];
	memset(cur_path,0,sizeof(cur_path));
	
	if(getcwd(cur_path,sizeof(cur_path)) == NULL){	
		printf("get local path failed\n");
		return ;			
	}
		
	printf("path =%s\n",cur_path);
	strcpy(file_path,cur_path);
	strcat(file_path,"/www");
	strcat(file_path,name);
	printf("path =%s\n",file_path);
	return ;

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
	
	char file_path[PATH_MAX];   

	memset(file_path,0,sizeof(file_path));

	get_file_path(hcfd.recv_buf,file_path);
	
	printf(" file path = %s\n",file_path);	
	
	const char*file_type =  get_file_type(hcfd.recv_buf);

	printf("the file type = %s\n",file_type);	
	// judage ruquest file is regular file , ensure not dir ,not exists
	if(!is_regular_file(file_path))
	{
		printf("the request file  is not found\n");
		show_exception(404,hcfd.fd);
		close(hcfd.fd);		
		return;
	}
	
	send_file(file_path,hcfd.fd,file_type);
	
	return ;

}

void send_file(char *file_path,int socket_fd,const char *file_type)
{
	int fd;
	char send_buffer[BUFFER_SIZE];
	char p_path[PATH_MAX];
	strcpy(p_path,file_path);
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
