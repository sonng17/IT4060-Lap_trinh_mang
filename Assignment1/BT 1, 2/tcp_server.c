#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


int main(int argc, char *argv[]){
    // Tao socket
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener != -1){
        printf("Socket created: %d\n", listener);
    }else {
        printf("Failed to create socket.\n");
        exit(1);
    }

    // Khai bao cau truc dia chi server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(atoi(argv[1]));

     // Gan dia chi voi socket
    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))){
        printf("bind() failed.\n");
        exit(1);
    }
    if (listen(listener, 5)){
        printf("listen() failed.\n");
        exit(1);
    }
    printf("Waiting for a new client ...\n");

    // Chap nhan ket noi
    int client = accept(listener, NULL, NULL);
    if (client == -1){
        printf("accept() failed.\n");
        exit(1);
    }
    printf("New client connected: %d\n", client);

    // Gui tin nhan cho client
    char *filename = argv[2];
    FILE *f = fopen(filename, "r");
    char buf[2048];
    
    fgets(buf, sizeof(buf), f);
    send(client, buf, sizeof(buf), 0);

    //lưu nội dung client gửi đến
    char msg[2048];
    char *filename2 = argv[3];
    FILE *f2 = fopen(filename2, "a");

    while (1)
    {
       int ret = recv(client, msg, sizeof(msg), 0);
        if (ret <= 0 || strncmp(msg, "exit", 4) == 0)
        {
            printf("Ket noi bi dong.\n");
            break;    
        }
        if (ret < sizeof(buf)){
            msg[ret] = 0;
        }
        printf("%d byte da nhan\n", ret);
        int store = fprintf(f2,"%s", msg);
        printf("So byte da ghi vao file: %d\n", store);
    }

    // Dong file
    fclose(f);
    fclose(f2);
    
    // Dong ket noi
    close(client);
    close(listener);
    return 0;
}