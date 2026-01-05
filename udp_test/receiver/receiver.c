#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BROADCAST_PORT 8888
#define MAX_MSG_LEN    1024

int main() {
    int sockfd;
    struct sockaddr_in local_addr, sender_addr;
    socklen_t addr_len = sizeof(sender_addr);
    char buffer[MAX_MSG_LEN];

    // 创建 UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 允许端口复用（多个接收程序可同时运行）
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    // 绑定本地地址和端口
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // 接收任何接口的广播
    local_addr.sin_port = htons(BROADCAST_PORT);

    if (bind(sockfd, (struct sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP 广播接收端已启动，监听端口 %d...\n", BROADCAST_PORT);
    printf("按 Ctrl+C 退出。\n\n");

    while (1) {
        ssize_t recv_len = recvfrom(sockfd, buffer, MAX_MSG_LEN - 1, 0,
                                    (struct sockaddr*)&sender_addr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[recv_len] = '\0';  // 字符串结尾

        char sender_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sender_addr.sin_addr, sender_ip, INET_ADDRSTRLEN);

        printf("收到来自 %s:%d 的广播消息 (%zd 字节)：\n%s\n\n",
               sender_ip, ntohs(sender_addr.sin_port), recv_len, buffer);
    }

    close(sockfd);
    return 0;
}