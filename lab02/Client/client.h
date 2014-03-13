/*************************************************************************
    > File Name: lab.h
    > Author: lax
    > Mail: xingkungao@gmail.com
    > Created Time: Sat 01 Mar 2014 08:56:48 PM CST
 ************************************************************************/
#ifndef __CLIENT_H__ 
#define __CLIENT_H__
#pragma pack(1)
#include <stdio.h>

/* Format of data section in sendline */
struct sendline{
	unsigned char type;
	unsigned char subtype; 
	unsigned char city[30];
	unsigned char flag;
};
typedef struct sendline sendline;

/* Format of data section in recvline */
struct recvline {
	unsigned short flag;
	unsigned char city[30];
	unsigned short year;
	unsigned char month;
	unsigned char date;
	unsigned char code;
	unsigned char info[6];
	unsigned char pad[94];
};
typedef struct recvline recvline;


/* welcome */
#define WELCOME "Welcome to NJUCS Weather Forecast Demo Program!\
						\nPlease input City Name in Chinese pinyin(e.g. nanjing or beijing)\
						\n(c)cls,(#)exit\n"

/* warnning if weather forecast service for city queried is unavailable */
#define WARNING  "Sorry, Server does not have weather information for city "

/* warnning if weather forecast service of the given date queried is unavailable */
#define SUB_WARNING "Sorry, no given day's weather information for city "

/* instructions */
#define INSTRUCT  "Please enter the given number to query\
							\n1.today\n2.three days from today\n3.custom day by yourself\
							\n(r)back,(c)cls,(#)exit\n===================================================\n"

#define SUB_INSTRUCT  "Please enter the day number(below 10, e.g. 1 means today):"

/* input error warning */
#define ERROR_MSG "input error!\n"

#define SUB_ERROR_MSG  "Input error\n"

//#define SERV_IP "114.212.86.173"  /** to connect to remote server */
//#define SERV_IP "114.212.191.33"  /** to connect to remote server */
#define SERV_IP "127.0.0.1"      /** to have both server and clients running on local host */ 
#define SERV_PORT 6666          /** port number for server */
#define SENDLEN 33             /** length of data in sendline */
#define RECVLEN 137				/** length of data in recvline */
#define handle_error(msg) \
	do {printf(msg); printf("\n");exit(EXIT_FAILURE);} while(0) /* handle error */

#define MAXCMD 50

/* clear screen */
void cls(const char* str);

/* stuff the data prepared to the data section of sendline */
void pack_sendline(sendline* sline, unsigned char type, unsigned char subtype, const char* str, unsigned char flag);

/* parse the data section of received recvline */
int parse_recvline(recvline* rline);

/* read n bytes from given socket */
int readn(int fd, char *bp, size_t len);

/* write n bytes to given socket */
int writen(int fd, const char *bp, size_t len);

/* send weather query of specific day */
int resend(int fd, sendline*, recvline*, char*, char*, char*);

#endif
