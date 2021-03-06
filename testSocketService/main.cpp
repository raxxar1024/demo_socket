/*
 * testSocketService.c
 *
 *  Created on: 2012-8-16
 *      Author: 皓月繁星
 */
#include <WINSOCK2.H>
#include <stdio.h>

#define PORT           5150
#define MSGSIZE        (512*1024)

//#pragma comment(lib, "ws2_32.lib")


void process_buf(char* buf, int all_len, SOCKET socket)
{
    char tmp_buf[MSGSIZE];
    int ret = 0;
    if(0 == all_len){
        return;
    }

    if(all_len >= 6){
        int cmd_len = *(unsigned int*)(buf+2);
        if(cmd_len > all_len){//实际命令长度大于传入的数据长度
            int index = all_len;
            memcpy(tmp_buf, buf, all_len);
            while(cmd_len > index){
                ret = recv(socket, tmp_buf+index, cmd_len-index, 0);
                index+=ret;
            }
            //process_ui(tmp_buf, index);
            printf("Received 4 [%d bytes]\n", index);
        }else{
            //process_ui(buf, cmd_len);
            printf("Received 5 [%d bytes]\n", cmd_len);
            process_buf(buf+cmd_len, all_len-cmd_len, socket);
        }
    }
    else{//不足6个字节，无法获取到命令长度
        memcpy(tmp_buf, buf, all_len);
        ret = recv(socket, tmp_buf+all_len, MSGSIZE-all_len, 0);
        process_buf(tmp_buf, all_len+ret, socket);
    }
}

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
        //int index = 0;
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

            if(szMessage[0]!=0x66 && szMessage[0]!=0x01){
                printf("Received [%d bytes], start is 0x%02x 0x%02x\n", ret,
                    szMessage[0], szMessage[0]);
                break;
            }

            cmd_len = *(unsigned int*)(szMessage+2);
            if(cmd_len>=ret){
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
            else{
                if(ret == MSGSIZE){
                    memcpy(cmd_buf, szMessage, cmd_len);
                    //process_UI cmd_buf
                    printf("Received 2 [%d bytes]\n", cmd_len);

                    //char tmp_buf[MSGSIZE+4];
                    //memcpy(tmp_buf, szMessage+cmd_len, MSGSIZE-cmd_len);
                    process_buf(szMessage+cmd_len, MSGSIZE-cmd_len, sClient);

                    //
                    #if 0
                    if(cmd_len < ret){
                        memcpy(cmd_buf, szMessage, cmd_len);
                        //process_UI cmd_buf
                        printf("Received 2 [%d bytes]\n", cmd_len);
                        memcpy(cmd_buf, szMessage+cmd_len, MSGSIZE-cmd_len);
                        //index = cmd_len;
                        int cmd_len2 = *(unsigned int*)(szMessage+cmd_len+2);
                        index2 = MSGSIZE-cmd_len;
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
                    #endif
                    //
                }else{
                    //process_UI
                    printf("Received [%d bytes]\n", ret);
                }
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
