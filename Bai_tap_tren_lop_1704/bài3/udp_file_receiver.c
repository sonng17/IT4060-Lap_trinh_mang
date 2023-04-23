#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>


int main(int argc, char *argv[])
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener != -1){
        printf("Socket created: %d\n", listener);
    }else {
        printf("Failed to create socket.\n");
        exit(1);
    }


    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

    
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))){
        perror("bind() failed");
        return 1;
    }
    if (listen(listener, 5)){
        perror("listen() failed");
        return 1;
    }
    printf("Waiting for a new client ...\n");

    
    struct sockaddr_in clientAddr;
    char buff[1280], clientIP[INET_ADDRSTRLEN];
	int ret, clientAddrLen = sizeof(clientAddr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1){
        printf("accept() failed.\n");
        exit(1);
    }
    
    // Receive file from client and save to disk
    char msg[2048];
    FILE *fp = fopen("store_from_send.txt", "w");
    if (fp == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }
    ret = recv(client, msg, sizeof(msg), 0);
    for (int i = 0; i<sizeof(msg);i++){
        if(msg[i]==';'){
            strcpy(msg, msg+i+1);
            fprintf(fp,"%s",msg);
            printf("Đã lưu thành công.\n");
            break;
        }
    }
    
    fclose(fp);
    close(client);
}
