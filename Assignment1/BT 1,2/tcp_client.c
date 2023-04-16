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
    char msg[256];
    while (1)
    {
        printf("Enter massage send to server: ");
        fgets(msg, sizeof(msg), stdin);
        send(client, msg, strlen(msg), 0);
        if (strncmp(msg, "exit", 4) == 0){
           break;
        }
        // Nhan tin nhan tu server
        char buf[2048];
        int len = recv(client, buf, sizeof(buf), 0);
        buf[len] = 0;
        printf("Data received from server: %s\n", buf);
    }
    

    // Ket thuc, dong socket
    close(client);
    return 0;
}