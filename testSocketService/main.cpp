/*
 * testSocketService.c
 *
 *  Created on: 2012-8-16
 *      Author: ð©ÔÂ·±ÐÇ
 */
#include <WINSOCK2.H>
#include <stdio.h>

#define PORT           5150
#define MSGSIZE        (512*1024)

//#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET sListen;
    SOCKET sClient;
    SOCKADDR_IN local;
    SOCKADDR_IN client;
    char *szMessage;
    int ret;
    int iaddrSize = sizeof(SOCKADDR_IN);
    WSAStartup(0x0202, &wsaData);
	
	//FILE *fp;
    //fp = fopen("d:\\outfile.txt","a+");

    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    local.sin_family = AF_INET;
    local.sin_port = htons(PORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(sListen, (struct sockaddr *) &local, sizeof(SOCKADDR_IN));

    listen(sListen, 1);

    sClient = accept(sListen, (struct sockaddr *) &client, &iaddrSize);
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr),
            ntohs(client.sin_port));

    while (TRUE) {
        int all_len = 0;
        int all_cnt = 1;
        int sock_err = 0;
        szMessage = (char*)malloc(MSGSIZE);

        while(TRUE){
            ret = recv(sClient, szMessage, MSGSIZE, 0);
            all_len += ret;
            if( ret==0  || ret == SOCKET_ERROR){
                printf("err [ret %d]\n", ret);
                sock_err = 1;
                break;
            }
            else if(ret==MSGSIZE){
                all_cnt++;
                szMessage = (char*)realloc(szMessage, MSGSIZE*all_cnt);
            }else{
                break;
            }
        }

        printf("Received [%d bytes]\n", all_len);

        //char str1[100];
        //sprintf(str1, "Received [%d bytes]\n", all_len);
        //fwrite(str1, strlen(str1), sizeof(char), fp);

        free(szMessage);
        if(1==sock_err){
            break;
        }
    }
    //fclose(fp);
    return 0;
}
