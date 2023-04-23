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
    
    char buffer[1024];
    int count = 0;
    int read_size;
    while ((read_size = recv(client, buffer, sizeof(buffer), 0)) > 0) {
        for (int i = 0; i < read_size; i++) {
            if (buffer[i] == '0' && buffer[i+1] == '1' && buffer[i+2] == '2' && buffer[i+3] == '3' && buffer[i+4] == '4' && buffer[i+5] == '5' && buffer[i+6] == '6' && buffer[i+7] == '7' && buffer[i+8] == '8' && buffer[i+9] == '9') {
                count++;
            }
        }
    }
    printf("Number of occurrences of \"0123456789\": %d\n", count);
    
    close(client);
    close(listener);
}
