#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>



int main( int argc, char *argv[]){
    // Khai bao socket
    int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // Khai bao dia chi cua server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    // Ket noi den server
    int res = connect(client, (struct sockaddr *)&addr, sizeof(addr));
    if (res == -1) {
        printf("Khong ket noi duoc den server!");
        return 1;
    }

    // Gui tin nhan den server
    char hostname[256];
    char drives[1024]; 

    printf("Nhập tên máy tính: ");
    fgets(hostname, sizeof(hostname), stdin);
    printf("Nhập danh sách các ổ đĩa (ví dụ: C:1000 D:2000 E:500): ");
    fgets(drives, sizeof(drives), stdin);

    // Đóng gói dữ liệu và gửi đến info_server
    char buf[1280];
    sprintf(buf, "%s;%s", hostname, drives);

    

    send(client, buf, sizeof(buf), 0);
    // Dong ket noi
    close(client);
    return 0;
}