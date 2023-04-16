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
    char buff[1024], clientIP[INET_ADDRSTRLEN];
    //int clientAddrLen = sizeof(addr);
	int ret, clientAddrLen = sizeof(clientAddr), clientPort;

    int client = accept(listener, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (client == -1){
        printf("accept() failed.\n");
        exit(1);
    }

    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP,  INET_ADDRSTRLEN);
    
    //int ret;
    //lưu nội dung client gửi đến
    char msg[2048]; 
    FILE *fp = fopen("log.txt", "w");
    if (fp == NULL) { // kiểm tra nếu file không mở được
      printf("Không thể mở tệp.");
      return 1;
   }

    while (1)
    {
        ret = recv(client, msg, sizeof(msg), 0);
        if (ret <= 0)
            break;
        else 
        {   
            printf("%s ", clientIP);
            fprintf(fp, "%s ",clientIP);

            time_t now = time(NULL);
            struct tm *local_time = localtime(&now);
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time);
            printf("%s", time_str);
            printf(" ");
            fprintf(fp, "%s", time_str);
            printf(" ");

            printf("%s",msg);
            fprintf(fp,"%s",msg);
            
            printf("\n");
        }
    }

    fclose(fp);
    
    close(client);
    close(listener);
}
