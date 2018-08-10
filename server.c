#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <sys/types.h>		//primitive system data types(包含很多类型重定义，如pid_t、int8_t等)   
#include <sys/socket.h>		//与套接字相关的函数声明和结构体定义，如socket()、bind()、connect()及struct sockaddr的定义等

#include <unistd.h>			//read,write
#include <netinet/in.h>	 	//某些结构体声明、宏定义，如struct sockaddr_in、PROTO_ICMP、INADDR_ANY等
#include <arpa/inet.h>		//某些函数声明，如inet_ntop()、inet_ntoa()等

#include <error.h>			//perror

#include <sys/stat.h>
#include <fcntl.h>
#include "serial.h"


#define MAXLITE 50
#define devpath "/dev/ttyATH0"

typedef struct sockaddr SA;

int main(int argc, char *argv[])
{
	//串口初始化函数"/dev/ttyATH0"
	int fd = ？;
	if (fd < 0){
		perror("serial init:");
	}
	printf("fd = %d, serial init success\n");
	int opt = -1;
	socklen_t client;
	struct sockaddr_in servaddr, clientaddr;

	//创建套接字
	int socketfd = ？;
	if (socketfd < 0){
		perror("fail to socket :");
		exit(-1);
	}
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	//端口重用	
	
	bzero(&servaddr, sizeof(servaddr));
//填写网络三元组信息	
/*	servaddr.sin_family 		= PF_INET;*/
/*	servaddr.sin_addr.s_addr 	= inet_addr("127.0.0.1");*/
/*	servaddr.sin_port			= htons(8888);*/
/*
	servaddr.sin_family			= PF_INET;	//TCP 协议			//填充地址信息
	servaddr.sin_addr.s_addr 	= inet_addr(argv[1]);
	servaddr.sin_port 			= htons(atoi(argv[2]));
*/	
	
	//绑定地址信息及创建监听队列
	？？
	
	while(1){
		printf("wait for a new client\n");
		
		//等待客户端连接
		？
		if (connfd < 0){
			perror("fail to accept");
//			exit(-1);
		} else {
			while(1){
				int len = ？;//从套接字接收客户端发送的指令
				if (len < 0){
					perror("read:");
				}else if (len == 0){
					break;
				}else {
					？;//往串口发
				}
			}
		}
	}
	close(fd);
}
