#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BROADCAST_PORT 8888          // 广播端口
#define BROADCAST_IP   "255.255.255.255"  // 广播地址
#define MAX_MSG_LEN    1024          // 最大消息长度（预留空间）

int main() {
    int sockfd;
    struct sockaddr_in broadcast_addr;
    int broadcast_enable = 1;

    // 创建 UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 允许广播
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("setsockopt SO_BROADCAST failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 设置广播地址
    memset(&broadcast_addr, 0, sizeof(broadcast_addr));
    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_port = htons(BROADCAST_PORT);
    inet_pton(AF_INET, BROADCAST_IP, &broadcast_addr.sin_addr);

    // ==== 这里是你预留的广播内容空间 ====
    char message[MAX_MSG_LEN] = "Hello, this is a UDP broadcast message from C program! 你好，这是广播消息！";
    // 你可以修改上面这行，或者在运行时动态填充，比如：
    // snprintf(message, MAX_MSG_LEN, "当前时间: %ld", time(NULL));

    printf("正在广播消息：%s\n", message);
    printf("按 Enter 键发送一次广播，输入 'q' + Enter 退出...\n");

    while (1) {
        char input = getchar();
        if (input == 'q' || input == 'Q') break;
        while (getchar() != '\n');  // 清空输入缓冲区

        // 发送广播
        ssize_t sent = sendto(sockfd, message, strlen(message), 0,
                              (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
        if (sent < 0) {
            perror("sendto failed");
        } else {
            printf("广播发送成功 (%zd 字节)\n", sent);
        }
    }

    close(sockfd);
    printf("广播端退出。\n");
    return 0;
}