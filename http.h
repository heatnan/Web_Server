#ifndef http_h
#define http_h

#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#define SIZE 3000

typedef struct {
 	char *cmd;
	char *recv_buf;
	int fd;	
} cfd;


void send_file(char *file_path,int socket_fd,const char *file_type);
void get_file_path(const char *file_name,char* file_path);
char* get_file_name(char *str);
const char* get_file_type(const char *file_name);
void send_index_file(int socket_fd);
void handle_get(cfd lcfd);
int is_regular_file(const char* path);
void show_exception(int type,int socket_fd);


#endif
