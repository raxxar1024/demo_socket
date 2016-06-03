/*
 * testSocketClient.c
 *
 *  Created on: 2012-8-16
 *      Author: 皓月繁星
 */
#include <WINSOCK2.H>
#include <stdio.h>

//定义程序中使用的常量
#define SERVER_ADDRESS "127.0.0.1" //服务器端IP地址
#define PORT           5150         //服务器的端口号
#define MSGSIZE ((1024*256+2048+4)*2)       //发缓冲区的大小
//#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    //连接所用套节字
    SOCKET sClient;
    //保存远程服务器的地址信息
    SOCKADDR_IN server;
    //收发缓冲区
    char szMessage[MSGSIZE];
    char szMessage1[MSGSIZE];
    //成功接收字节的个数
    //int ret;

    // Initialize Windows socket library
    WSAStartup(0x0202, &wsaData);

    // 创建客户端套节字
    sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INET指明使用TCP/IP协议族；
                                                         //SOCK_STREAM, IPPROTO_TCP具体指明使用TCP协议
    // 指明远程服务器的地址信息(端口号、IP地址等)
    memset(&server, 0, sizeof(SOCKADDR_IN)); //先将保存地址的server置为全0
    server.sin_family = PF_INET; //声明地址格式是TCP/IP地址格式
    server.sin_port = htons(PORT); //指明连接服务器的端口号，htons()用于 converts values between the host and network byte order
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS); //指明连接服务器的IP地址
                                                        //结构SOCKADDR_IN的sin_addr字段用于保存IP地址，sin_addr字段也是一个结构体，sin_addr.s_addr用于最终保存IP地址
                                                        //inet_addr()用于将 形如的"127.0.0.1"字符串转换为IP地址格式
    //连到刚才指明的服务器上
    connect(sClient, (struct sockaddr *) &server, sizeof(SOCKADDR_IN)); //连接后可以用sClient来使用这个连接
                                                                        //server保存了远程服务器的地址信息
    while (TRUE) {
        printf("Send:");
        //从键盘输入
        gets(szMessage); //The gets() functionreads characters from stdin and loads them into szMessage
        if(0==strcmp(szMessage,"1")){
            // 按1后发送数据
            int i = 0;
            for(i=0;i<6;i++){
                printf("time%d start\n", i);
                szMessage1[0] = 0x66;
                szMessage1[1] = 0x01;
                *(unsigned int*)(szMessage1+2) = (MSGSIZE/2);
                szMessage1[0+(MSGSIZE/2)] = 0x66;
                szMessage1[1+(MSGSIZE/2)] = 0x01;
                *(unsigned int*)(szMessage1+(MSGSIZE/2)+2) = (MSGSIZE/2);
                //int j = 0;
                //for(j=0;j<4;j++){
                //    printf("0x%02x ",szMessage1[j]&0xff);
                //}
                printf("time%d end\n", i);
                send(sClient, szMessage1, MSGSIZE, 0); //sClient指明用哪个连接发送； szMessage指明待发送数据的保存地址 ；strlen(szMessage)指明数据长度
                //Sleep(10);
            }
        }
        if(0==strcmp(szMessage,"2")){
            // 按2后发送100数据
            szMessage1[0] = 0x66;
            szMessage1[1] = 0x01;
            *(unsigned int*)(szMessage1+2) = 100;
            send(sClient, szMessage1, 100, 0); //sClient指明用哪个连接发送； szMessage指明待发送数据的保存地址 ；strlen(szMessage)指明数据长度
            //Sleep(10);
        }
    }

    // 释放连接和进行结束工作
    closesocket(sClient);
    WSACleanup();
    return 0;
}
