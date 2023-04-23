#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

// Server nhan file tu client

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
    //int clientAddrLen = sizeof(addr);
	int ret, clientAddrLen = sizeof(clientAddr);

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1){
        printf("accept() failed.\n");
        exit(1);
    }
    
    char msg[2048]; 
    ret = recv(client, msg, sizeof(msg), 0);
    int count = 0;
    char hostname[256], drives[1024], drive[256], size[256];

    // Đếm số dấu ":" trong chuỗi msg
    for (int i = 0; i < strlen(msg); i++) {
        if (msg[i] == ':') {
            count++;
        }
    }
    int i;
    // Tách tên máy tính và danh sách các ổ đĩa từ chuỗi msg
    for (i = 0; i < strlen(msg); i++) {
        // N?u g?p d?u cách d?u tiên, in ra k?t qu? và thoát kh?i vòng l?p
        if (msg[i] == ' ') {
            printf("Ten may: ");
            int j;
            for ( j = 0; j < i; j++) {
                printf("%c", msg[j]);
            }
            break;
        }
    }
    printf("\n");
    strcpy(msg, msg+i+1);
    printf("So o tim duoc: %d\n",count);
    
    
    // Tách các ổ đĩa từ chuỗi drives
    char *token;

    // Tách chuỗi msg thành các chuỗi con và in ra dưới định dạng "x - y"
    token = strtok(msg, " ");
    while (token != NULL) {
        char *colon = strchr(token, ':');
        if (colon != NULL) {
        *colon = '-';
        }
        printf("\t%s\n", token);
        token = strtok(NULL, " ");
    }


    close(client);
    close(listener);
}
