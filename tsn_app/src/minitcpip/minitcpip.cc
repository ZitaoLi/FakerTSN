#include "minitcpip.h"

int commn(int argc,char *argv[]){
    int ret;
    int i;
    char sip[40]="169.254.159.112";
    char sport[10]="7777";
    char dip[40]="169.254.159.111";
    char dport[10]="5000";
    struct socket ti;
    int s;
    ti.sip=inet_addr(sip);
    ti.dip=inet_addr(dip);
    ti.sport=(unsigned short)atoi(sport);
    ti.dport=(unsigned short)atoi(dport);
    s=mini_socket(&ti);
    if(s<0){
        printf("mini_socket() error\n");
        return 0;
    }
    ret=mini_connect(s);//connect to tcpip using TCP three time handshaking
    if(ret<0){
        printf("mini_connect() error\n");
        return 0;
    }
    while (1){
        //get input from stdin,  quit when EOF or "myquit!" input
        if(fgets(buf, sizeof(buf), stdin)==0)  break;
        if(strcmp(buf,"myquit!")==0)       break;
        ret=mini_send(s,buf,strlen(buf));
        if(ret<=0){
            printf("mini_send() return %d\n",ret);
            break;
        }
        ret=mini_recv(s,recvbuf,&recvedlen);
        if(ret<=0){
            printf("mini_recv() return %d\n",ret);
            break;
        }
        recvbuf[recvedlen]=0;
        printf("recved[%d bytes]:%s\n",recvedlen,recvbuf);
    }
    mini_close(s);//close tcpip using TCP three time handshaking
}
