#ifndef _MINITCPIP_H
#define _MINITCPIP_H

#include <pcap.h>
#include <libnet.h>
#include "mini_socket.h"
#define uchar unsigned char
#define MAXBUFLEN 2048

int mini_connect(int socket);
int mini_recv(int socket, char* buf,int * buflen);
int mini_send(int socket,char*buf,int buflen);

char buf[MAXBUFLEN];
int buflen;
char recvbuf[MAXBUFLEN];
int recvedlen;

#endif
