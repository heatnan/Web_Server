#include<sys/socket.h>
#include<stdio.h>
#include<string.h>
#define SIZE 3000

typedef struct {
 	char *recv_buf;
	int fd;	
} cfd;


void send_file(char *file_path,int socket_fd,char *file_type);
char* get_file_path(char *file_name);
char* get_file_name(char *str);
char* get_file_type(char *file_name);
void send_index_file(int socket_fd);
void handle_get(cfd lcfd);
int is_regular_file(const char* path);
int check_file_exist(const char* path);
void show_exception(int type,int socket_fd);
