#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct 
{
    char id[10];
    char hoten[255];
    char ngaySinh[20];
    char diemtb[5];
} Sinhvien;

int main(){
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
    addr.sin_port = htons(9000);
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
    // Nhan tin nhan tu client
    Sinhvien sv;
    Sinhvien* temp;
    char buf[sizeof(sv)];

    int ret = recv(client, buf, sizeof(buf), 0);
    temp = (Sinhvien *) buf; // gan con tro
    sv = *temp;
    printf("So byte nhan duoc: %d\n", ret);
    printf("%s", sv.id);
    printf("%s", sv.hoten);
    printf("%s", sv.ngaySinh);
    printf("%s", sv.diemtb);
    printf("\n");
    
    // Dong ket noi
    close(client);
    close(listener);
    return 0;
}