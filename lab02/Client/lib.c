/*************************************************************************
    > File Name: lib.c
    > Author: lax
    > Mail: xingkungao@gmail.com
    > Created Time: Thu 06 Mar 2014 11:24:46 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <arpa/inet.h>
#include "client.h"

extern char WEATHER[5][7];

void cls(const char* str) {
	printf("\e[1;1H\e[2J%s", str);
}

void pack_sendline(sendline* send, unsigned char type, unsigned char subtype,\
		const char* str, unsigned char flag) {
	int i;
	memset(send, 0, sizeof(*send));
	unsigned char* p = send->city;
	send->type = type;
	send->subtype = subtype;
	//while (*str++ == 0x20);
	if (str != NULL) {
		for (i = 0; i < 29 && *str && *str != 0x20; i++) 
			*p++ = *str++;
	}
	*p = 0;
	send->flag = flag;
}

int parse_recvline(recvline* recv) {
	unsigned short flag = ntohs(recv->flag);
	int i;
	int code = recv->code;
	unsigned char* p = recv->info;
	 /*
	  * 0x0400: // city is not available
		0x0300: // city is available
	 */

	if (flag == 0x0400) {
		printf("%s%s!\n", WARNING, recv->city);
	}
	else if (flag == 0x0241) { 
		printf("%s%s!\n", SUB_WARNING, recv->city);
	}
	else if (flag == 0x0141 || flag == 0x0142) { // one day's weather forecast
		printf("City: %s  Today is: %u/%02d/%02d  Weather information is as follows:\n"\
				, recv->city, ntohs(recv->year), recv->month, recv->date);
		if (flag == 0x0141)
			code = 1;
		for(i = 0; i < code; i++) {
			printf("Today's Weather is: %s;  Temp:%02d\n", WEATHER[*(p+2*i)], *(p+2*i+1));
		}
	}
	memset(recv, 0, sizeof(*recv));
	return flag;
}

/* readn - read exactly n bytes */
int readn(int fd, char *bp, size_t len) {
	int cnt;
	int rc;

	cnt = len;
	while (cnt > 0) {
		rc = recv(fd, bp, cnt, 0);
		if (rc < 0)	{			/* read error? */
			if ( errno == EINTR )	/* interrupted? */
				continue;			/* restart the read */
		//	handle_error("lose connection\n");
			return -1;				/* return error */
		}
		if (rc == 0)				/* EOF? */
			return len - cnt;		/* return short count */
		bp += rc;
		cnt -= rc;
	}
	return len;
}

/* writen- write exactly n bytes */
int writen(int fd, const char *bp, size_t len) {
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = bp;
	nleft = len;
	while (nleft > 0) {
		if ((nwritten = send(fd, ptr, nleft, 0)) <= 0) {
			if (nwritten < 0) {
				if (errno == EINTR)  {
					nwritten = 0;
					continue;
				}
				return -1;
			}
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return len;
}



int resend(int sockfd, sendline* sline, recvline* rline, \
		char* cmd, char *inst, char* sub_inst) {
	while (1) {
		scanf("%s", (char*)inst);
		if (!strcmp(inst, "1") || !strcmp(inst, "2")) {
			if (!strcmp(inst, "1")) 
				pack_sendline(sline, 0x02, 0x1, cmd, 1);
			else
				pack_sendline(sline, 0x02, 0x02, cmd, 3);
				
			if (writen(sockfd, (char*)sline, sizeof(*sline)) < 0)
				return -1;

			if (readn(sockfd, (char*)rline, RECVLEN) < 0) {
				return -1;
			}
			parse_recvline(rline);
		}
		else if (!strcmp(inst, "c")) {
			cls(INSTRUCT);
		}
		else if (!strcmp(inst, "#")) {
			close(sockfd);
			exit(0);
		}
		else if (!strcmp(inst, "r")) {
			cls(WELCOME);
			break;
		}
		else if (!strcmp(inst, "3")) {
			while (1) {
				printf("%s", SUB_INSTRUCT);
				scanf("%s", (char*)sub_inst);
				if (strlen(sub_inst) != 1) {
					printf("%s", SUB_ERROR_MSG);
				}
				else if (*sub_inst >= '1' && *sub_inst <= '9') {
					pack_sendline(sline, 0x02, 0x01, cmd, *sub_inst - 48);
					if (writen(sockfd, (char*)sline, sizeof(*sline)) < 0)
						return -1;
					if (readn(sockfd, (char*)rline, RECVLEN) < 0) 
						return -1;
					parse_recvline(rline);
					break;
				}
				else {
					printf("%s", SUB_ERROR_MSG);
				}
			}
		}
		else {
			printf("%s", ERROR_MSG);
		}
	}
	return 1;
}


