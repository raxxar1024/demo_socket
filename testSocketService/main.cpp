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
    char szMessage[MSGSIZE];
    char cmd_buf[MSGSIZE];
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
        int index = 0;
        int index2 = 0;
        int cmd_len = 0;
        int sock_err = 0;

        while(TRUE){
            ret = recv(sClient, szMessage, MSGSIZE, 0);

            if(ret==0 || ret == SOCKET_ERROR){
                printf("err [ret %d]\n", ret);
                sock_err = 1;
                break;
            }

            cmd_len = *(unsigned int*)(szMessage);
            if(cmd_len>ret){
                index2 = ret;
                memcpy(cmd_buf, szMessage, ret);
                while(cmd_len > index2){
                    ret = recv(sClient, szMessage, cmd_len-index2, 0);
                    memcpy(cmd_buf+index2, szMessage, ret);
                    index2+=ret;
                }
                //process UI
                printf("Received 1 [%d bytes]\n", cmd_len);
                
            }
            
            if(ret == MSGSIZE){
                if(cmd_len < ret){
                    memcpy(cmd_buf, szMessage, cmd_len);
                    //process_UI cmd_buf
                    printf("Received 2 [%d bytes]\n", cmd_len);
                    index = cmd_len;
                    int cmd_len2 = *(unsigned int*)(szMessage+index);
                    index2 = MSGSIZE-index;
                    while(cmd_len2 > index2){
                        ret = recv(sClient, szMessage, cmd_len2-index2, 0);
                        memcpy(cmd_buf+index2, szMessage, ret);
                        index2+=ret;
                    }
                    //process UI
                    printf("Received 3 [%d bytes]\n", cmd_len2);
                    
                }
                else{
                    //this is error, cmd_len should not longer than MSGSIZE;
                    printf("this is error, cmd_len should not longer than MSGSIZE %d;\n", MSGSIZE);
                }
            }else{
                //process_UI
                printf("Received [%d bytes]\n", ret);
            }

        }

        //char str1[100];
        //sprintf(str1, "Received [%d bytes]\n", all_len);
        //fwrite(str1, strlen(str1), sizeof(char), fp);
        
        if(1==sock_err){
            break;
        }
    }
    //fclose(fp);
    return 0;
}
