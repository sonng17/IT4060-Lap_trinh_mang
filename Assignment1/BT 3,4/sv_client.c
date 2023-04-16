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
    char hoten[50];
    char ngaySinh[20];
    char diemtb[5];
} Sinhvien;

int len_sv(Sinhvien sv){
    return strlen(sv.id) + strlen(sv.diemtb) + strlen(sv.hoten) + strlen(sv.ngaySinh);
}

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
    Sinhvien sv;
    printf("Nhap id: ");
    scanf("%s", sv.id);
    printf("\n");

    printf("Nhap ho ten: ");
    scanf("%s",sv.hoten);
    printf("\n");

    printf("Nhap ngay sinh: ");
    scanf("%s",sv.ngaySinh);
    printf("\n");

    printf("Nhap diem: ");
    scanf("%s",sv.diemtb);
    printf("\n");
    
    char buf[300] = "";
    strcpy(buf, " ");
    strcat(buf, sv.id);
    strcat(buf, " ");
    strcat(buf, sv.hoten);
    strcat(buf, " ");
    strcat(buf, sv.ngaySinh);
    strcat(buf, " ");
    strcat(buf, sv.diemtb);
    send(client, buf, sizeof(buf), 0);
    // Dong ket noi
    close(client);
    return 0;
}