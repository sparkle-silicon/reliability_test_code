#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/select.h>

#define SERIAL_PORT "/dev/ttyUSB0"  // 串口设备，根据实际情况修改
#define BAUD_RATE B115200           // 串口波特率
#define TIMEOUT_USEC 100000         // 10毫秒转换为微秒
#define MAX_RETRIES 10000             // 最大重试次数

// 函数：设置串口参数
int configure_serial_port(int fd) {
    struct termios options;
    if (tcgetattr(fd, &options) < 0) {
        perror("Error getting serial port attributes");
        return -1;
    }

    cfsetispeed(&options, BAUD_RATE);
    cfsetospeed(&options, BAUD_RATE);

    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag |= CREAD | CLOCAL;

    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    if (tcsetattr(fd, TCSANOW, &options) < 0) {
        perror("Error setting serial port attributes");
        return -1;
    }

    return 0;
}

// 函数：打开串口
int open_serial_port() {
    int fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        perror("Unable to open serial port");
        return -1;
    }

    if (configure_serial_port(fd) < 0) {
        close(fd);
        return -1;
    }

    return fd;
}

// 函数：发送数据
int send_data(int fd, const unsigned char *data, size_t length) {
    ssize_t written = write(fd, data, length);
    if (written < 0) {
        perror("Error writing to serial port");
        return -1;
    }
    return 0;
}

// 函数：接收数据，使用select()避免阻塞
int receive_data(int fd, unsigned char *buffer, size_t length) {
    fd_set readfds;
    struct timeval timeout;

    // 设置超时为10毫秒
    timeout.tv_sec = 0;
    timeout.tv_usec = TIMEOUT_USEC;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    // 使用select()等待串口数据
    int ret = select(fd + 1, &readfds, NULL, NULL, &timeout);
    if (ret < 0) {
        perror("select() error");
        return -1;
    }

    if (ret == 0) {
        return 0;  // Timeout, no data
    }

    // 如果串口有数据，读取数据
    ssize_t n = read(fd, buffer, length);
    if (n < 0) {
        perror("Error reading from serial port");
        return -1;
    }

    return n;  // 返回读取的字节数
}

int main() {
    int fd = open_serial_port();
    if (fd == -1) {
        return 1;
    }

    unsigned char send_byte = 0x66;
    unsigned char response;
    unsigned char version[4];
    ssize_t n;
    int retries = 0;

    // 不停地发送0x66直到收到0x66
    while (retries < MAX_RETRIES) {
        if (send_data(fd, &send_byte, 1) < 0) {
            close(fd);
            return 1;
        }
        printf("Sent 0x66\n");

        n = receive_data(fd, &response, 1);
        if (n < 0) {
            close(fd);
            return 1;
        }

        if (n > 0 && (response == 0x66||response==0xEE)) {
            if(response==0x66)
                printf("Received 0x66, proceeding to receive version\n");
            else if(response==0xEE)
                printf("Failed to exit debugging mode. If it is after internal or external flash updates, it cannot be exited\n");
            break;
        }

        retries++;
        printf("No response received, retrying... (%d/%d)\n", retries, MAX_RETRIES);
    }

    if (retries >= MAX_RETRIES) {
        printf("Max retries reached, no valid response received.\n");
        close(fd);
        return 1;
    }


    close(fd);
    return 0;
}
