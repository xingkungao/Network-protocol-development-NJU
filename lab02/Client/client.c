/*************************************************************************
    > File Name: lab.c
    > Author: lax
    > Mail: xingkungao@gmail.com
    > Created Time: Sat 01 Mar 2014 09:31:18 PM CST ************************************************************************/

#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "client.h"

const char WEATHER[5][7] = {"shower", "clear", "clody", "rain", "fog"};

int main() {
	struct sockaddr_in servaddr; 
	int sockfd;
	int status;
	char cmd[1024];
	char inst[1024];
	char sub_inst[1024];
	sendline sline;
	recvline rline;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		handle_error("error creating socket");
	}
	
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERV_IP);
	servaddr.sin_port = htons(SERV_PORT);

	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		handle_error("error connecting to server");
	}

	printf("size is %ld\n", sizeof(rline));
	cls(WELCOME);

	while (1) {
		gets(cmd);
		int i = 0;
		/* delete all prefixing space character */
		while(cmd[i] == 0x20 || cmd[i] == 0x9) i++;
		strncpy(cmd, cmd+i, strlen(cmd+i));
		cmd[strlen(cmd) - i] = 0;

		if (!strlen(cmd))  {
			continue;
		}

		/* get "c" from keyboard, clear sceen */
		else if (!strcmp(cmd, "c")) { 
			cls(WELCOME);
		}

		/* get "#" from keyboard, terminates */
		else if (!strcmp(cmd, "#"))  {
			close(sockfd);
			return 0;
		}

		/* get anything else, send the query to server */
		else {
			/* pack the sending packet */
			pack_sendline(&sline, 1, 0, cmd, 0);

			/* send the query packet */
			if (writen(sockfd, (char*)&sline, sizeof(sline)) < 0) {
				handle_error("error sending socket!");
			}

			/* receive the acknowledgment packet */
			if (readn(sockfd, (char*)&rline, sizeof(rline)) < 0)  {
				handle_error("error receiving socket");
			}

			/* parse the received packet */
			status = parse_recvline(&rline);

			/* weather infomation available for the queried city */
			if (status == 0x0300)  {
				cls(INSTRUCT);
				if (resend(sockfd, &sline, &rline, cmd, inst, sub_inst) < 0) {
					return 0;
					handle_error("lose connection!");
				}
			}
		}
	}
	return 0;
}

