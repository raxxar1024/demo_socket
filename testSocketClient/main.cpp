/*
 * testSocketClient.c
 *
 *  Created on: 2012-8-16
 *      Author: ��·���
 */
#include <WINSOCK2.H>
#include <stdio.h>

//���������ʹ�õĳ���
#define SERVER_ADDRESS "127.0.0.1" //��������IP��ַ
#define PORT           5150         //�������Ķ˿ں�
#define MSGSIZE ((1024*256+2048+4)*2)       //���������Ĵ�С
//#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    //���������׽���
    SOCKET sClient;
    //����Զ�̷������ĵ�ַ��Ϣ
    SOCKADDR_IN server;
    //�շ�������
    char szMessage[MSGSIZE];
    char szMessage1[MSGSIZE];
    //�ɹ������ֽڵĸ���
    //int ret;

    // Initialize Windows socket library
    WSAStartup(0x0202, &wsaData);

    // �����ͻ����׽���
    sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //AF_INETָ��ʹ��TCP/IPЭ���壻
                                                         //SOCK_STREAM, IPPROTO_TCP����ָ��ʹ��TCPЭ��
    // ָ��Զ�̷������ĵ�ַ��Ϣ(�˿ںš�IP��ַ��)
    memset(&server, 0, sizeof(SOCKADDR_IN)); //�Ƚ������ַ��server��Ϊȫ0
    server.sin_family = PF_INET; //������ַ��ʽ��TCP/IP��ַ��ʽ
    server.sin_port = htons(PORT); //ָ�����ӷ������Ķ˿ںţ�htons()���� converts values between the host and network byte order
    server.sin_addr.s_addr = inet_addr(SERVER_ADDRESS); //ָ�����ӷ�������IP��ַ
                                                        //�ṹSOCKADDR_IN��sin_addr�ֶ����ڱ���IP��ַ��sin_addr�ֶ�Ҳ��һ���ṹ�壬sin_addr.s_addr�������ձ���IP��ַ
                                                        //inet_addr()���ڽ� �����"127.0.0.1"�ַ���ת��ΪIP��ַ��ʽ
    //�����ղ�ָ���ķ�������
    connect(sClient, (struct sockaddr *) &server, sizeof(SOCKADDR_IN)); //���Ӻ������sClient��ʹ���������
                                                                        //server������Զ�̷������ĵ�ַ��Ϣ
    while (TRUE) {
        printf("Send:");
        //�Ӽ�������
        gets(szMessage); //The gets() functionreads characters from stdin and loads them into szMessage
        if(0==strcmp(szMessage,"1")){
            // ��1��������
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
                send(sClient, szMessage1, MSGSIZE, 0); //sClientָ�����ĸ����ӷ��ͣ� szMessageָ�����������ݵı����ַ ��strlen(szMessage)ָ�����ݳ���
                //Sleep(10);
            }
        }
        if(0==strcmp(szMessage,"2")){
            // ��2����100����
            szMessage1[0] = 0x66;
            szMessage1[1] = 0x01;
            *(unsigned int*)(szMessage1+2) = 100;
            send(sClient, szMessage1, 100, 0); //sClientָ�����ĸ����ӷ��ͣ� szMessageָ�����������ݵı����ַ ��strlen(szMessage)ָ�����ݳ���
            //Sleep(10);
        }
    }

    // �ͷ����Ӻͽ��н�������
    closesocket(sClient);
    WSACleanup();
    return 0;
}
