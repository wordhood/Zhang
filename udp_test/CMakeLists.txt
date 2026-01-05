cmake_minimum_required(VERSION 3.10)  # 最低 CMake 版本

# 项目名称和语言
project(UDP_Broadcast_Example LANGUAGES C)

# 设置 C 标准（可选，推荐 C99 或更高）
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加发送端可执行文件
add_executable(sender
    sender/sender.c
)

# 添加接收端可执行文件
add_executable(receiver
    receiver/receiver.c
)

