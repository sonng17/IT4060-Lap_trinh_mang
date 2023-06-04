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

/*
    Khi đã kết nối với 1 client nào đó, yêu cầu client gửi user và pass, so sánh với
    file cơ sở dữ liệu là một file text, mỗi dòng chứa một cặp user + pass ví dụ:
    admin admin
    guest nopass
    … N
    ếu so sánh sai, không tìm thấy tài khoản thì báo lỗi đăng nhập.
    Nếu đúng thì đợi lệnh từ client, thực hiện lệnh và trả kết quả cho client.
    Dùng hàm system(“dir > out.txt”) để thực hiện lệnh.
    dir là ví dụ lệnh dir mà client gửi.
    > out.txt để định hướng lại dữ liệu ra từ lệnh dir, khi đó kết quả lệnh dir sẽ
    được ghi vào file văn bản.
*/

int checkUserPass(char *s, FILE *f){
    char *msg;
    while (fgets(msg, strlen(s)+1, f) != NULL)
    {
        if(strncmp(s, msg, strlen(s))==0) return 1;
    }
    return 0;
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
        char *msg = "yeu cau dang nhap username pass: \n";
        send(client, msg, strlen(msg), 0);
        if (fork() == 0)
        {
            // Tien trinh con
            close(listener);
            printf("child %d", getpid());
            // Xu ly ket noi tu client
            int login = 0;
            char buf[256];
            while (1)
            {

                int ret = recv(client, buf, sizeof(buf), 0);
                if (ret <= 0)
                    break;
                buf[ret] = 0;
                printf("Received from %d: %s\n", client, buf);
                // xử lý đăng nhập
                if(login == 0)
                {
                    // Xu ly cu phap lenh dang nhap
                        char cmd[32], id[32], tmp[32];
                        ret = sscanf(buf, "%s%s%s", cmd, id, tmp);
                        
                        if (ret == 2)
                        {
                            FILE *f = fopen("pass.txt", "r");
                            if (checkUserPass(buf, f) == 1)
                            {
                                login = 1;
                                char *msg = "Dung user and pass. Hay nhap lenh de chuyen tiep.\n";
                                send(client, msg, strlen(msg), 0);
                            }
                            else
                            {
                                char *msg = "Sai user or pass. Hay nhap lai.\n";
                                send(client, msg, strlen(msg), 0);
                            }
                            fclose(f);
                        }
                        else
                        {
                            char *msg = "Sai tham so. Hay nhap lai.\n";
                            send(client, msg, strlen(msg), 0);
                        }
                }else
                {
                    char sendbuf[1024];
                    char *comand = "> out.txt";
                    //strcat(buf, " > out.txt");
                    sprintf(sendbuf, "%s %s", buf, comand);
                    system(buf);
                        
                    // Forward du lieu cho user
                    send(client, buf, strlen(buf), 0);
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