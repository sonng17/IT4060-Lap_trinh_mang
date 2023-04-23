#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 4) {
        printf("Usage: %s <server_ip> <server_port> <file_name>\n", argv[0]);
        return 1;
    }

    

    // Create socket
    int client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_sock == -1) {
        printf("Failed to create socket.\n");
        return 1;
    }

    // Connect to server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    int res = connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (res == -1) {
        printf("Failed to connect to server.\n");
        return 1;
    }

    // Open file to send
    char *file_name = argv[3];
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("Failed to open file.\n");
        return 1;
    }
    // Read file and send to server
    char buffer[1024],buf[2048];
    fgets(buffer,sizeof(buffer),fp);
    sprintf(buf, "%s;%s", file_name, buffer);
    send(client_sock,buf,sizeof(buf),0);

    int read_size;
    while ((read_size = fread(buf, 1, sizeof(buf), fp)) > 0) {
        if (send(client_sock, buf, read_size, 0) != read_size) {
            printf("Failed to send data to server.\n");
            fclose(fp);
            close(client_sock);
            return 1;
        }
    }

    // Close file and socket
    fclose(fp);
    close(client_sock);

    return 0;
}
