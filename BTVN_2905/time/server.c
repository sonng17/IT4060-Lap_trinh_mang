#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include<time.h>

/*
    Lập trình ứng dụng time_server thực hiện chức năng sau:
        • Chấp nhận nhiều kết nối từ các client sử dụng kỹ thuật
        multiprocessing.
        • Client gửi lệnh GET_TIME [format] để nhận thời gian từ
        server.
        • format là định dạng thời gian server cần trả về client. Các
        format cần hỗ trợ gồm:
        • dd/mm/yyyy – vd: 30/01/2023
        • dd/mm/yy – vd: 30/01/23
        • mm/dd/yyyy – vd: 01/30/2023
        • mm/dd/yy – vd: 01/30/23
        • Cần kiểm tra tính đúng đắn của lệnh client gửi lên server
*/
// ham tra ve thoi gian
void get_time(char *msg, char *format){
    time_t t = time(NULL);
    struct tm date = *localtime(&t);

    if(strncmp(format, "dd/mm/yyyy",10)==0){
        sprintf(msg,"%d/%d/%d", date.tm_mday, (date.tm_mon+1), (date.tm_year+1900));
    }else if(strncmp(format, "dd/mm/yy",8)==0){
        sprintf(msg,"%d/%d/%d", date.tm_mday, (date.tm_mon+1), (date.tm_year-100));
    }else if(strncmp(format, "mm/dd/yyyy",10)==0){
        sprintf(msg,"%d/%d/%d", (date.tm_mon+1), date.tm_mday, (date.tm_year+1900));
    }else if(strncmp(format, "mm/dd/yy",8)==0){
        sprintf(msg,"%d/%d/%d", (date.tm_mon+1), date.tm_mday, (date.tm_year-100));
    }else{
        sprintf(msg, "%s", "Sai format. Hay nhap lai.");
    }
}

void signalHandler(int signo)
{
    int pid = wait(NULL);
    printf("Child %d terminated.\n", pid);
}

int main() 
{
    int listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listener == -1)
    {
        perror("socket() failed");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(9000);

    if (bind(listener, (struct sockaddr *)&addr, sizeof(addr))) 
    {
        perror("bind() failed");
        return 1;
    }

    if (listen(listener, 5)) 
    {
        perror("listen() failed");
        return 1;
    }

    signal(SIGCHLD, signalHandler);

    while (1)
    {
        printf("Waiting for new client...\n");
        int client = accept(listener, NULL, NULL);
        // yeu cau dang nhâp
        char *msg = "yeu cau dang nhap GET_TIME [format]: \n";
        send(client, msg, strlen(msg), 0);
        if (fork() == 0)
        {
            // Tien trinh con
            close(listener);
            printf("child %d\n", getpid());
            // Xu ly ket noi tu client
            char buf[256];
            while (1)
            {
                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0)
                    break;
                buf[ret] = 0;
                printf("Received from %d: %s\n", client, buf);
                // xử lý tin nhắn từ client
                // Xu ly cu phap của lenh
                char cmd[32], id[32], tmp[32];
                ret = sscanf(buf, "%s%s%s", cmd, id, tmp);
                if (ret == 2)
                {
                    if(strncmp(buf, "GET_TIME", 8)==0)
                    {
                        // xu ly va tra ve thoi gian
                        char msg_time[1024];
                        get_time(msg_time, buf+9);
                        send(client, msg_time, strlen(msg_time), 0);
                    }
                    
                }
                else
                {
                    char *msg_err = "Sai tham so. Hay nhap lai.\n";
                    send(client, msg_err, strlen(msg_err), 0);
                }
            }

            close(client);
            exit(0);
        }

        // Tien trinh cha
        close(client);
    }
    
    close(listener);    

    return 0;
}