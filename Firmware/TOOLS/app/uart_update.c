#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#if __gnu_linux__
#include <sys/types.h>
#include <sys/stat.h>
#include <error.h>
#include <signal.h>
#include <termios.h>
#include <time.h>
#include <sys/time.h>
#elif _WIN64|_WIN32//win64
// Windows头文件
#include <windows.h>
#include <io.h>
// 声明Windows版本的unistd.h相关函数
#define open _open
#define close _close
#define read _read
#define write _write
#endif
// gcc ./TOOLS/app/uart_update.c -o uart_update
struct timeval t0, t1, t2;
int dev_fd = 0;
int bin_fd = 0;
// #define BAUDRATE B57600
#define BAUDRATE B115200
// #define BAUDRATE B256000
#define USB_DEV "/dev/ttyUSB0"
struct termios termios0, termios1;
void sighandler(int signel)
{
    close(dev_fd);
    close(dev_fd);
    exit(0);
}
int main(int argc, char **argv)
{
    int args = argc;
    printf("./a.out <file name>\n");
    while(args--)
    {
        printf("argc:%d,argv:%s\n", args, argv[args]);
    }
    // 打开串口
    int dev_fd = open(USB_DEV, O_NOCTTY | O_RDWR);
    if(dev_fd == -1)
    {
        perror(USB_DEV " open fail");
        exit(dev_fd);
    }
    if(fcntl(dev_fd, F_SETFL, 0) == -1) // 更改控制
    {
        perror(USB_DEV " fcntl fail");
        close(dev_fd);
        exit(dev_fd);
    }
    if(tcgetattr(dev_fd, &termios0) == -1) // 获取设置
    {
        perror(USB_DEV " tcgetattr fail");
        close(dev_fd);
        exit(dev_fd);
    }
    termios1 = termios0;
    cfmakeraw(&termios1);             // 设置原始模式
    cfsetispeed(&termios1, BAUDRATE); // 输入波特率
    cfsetospeed(&termios1, BAUDRATE); // 输出波特率
    termios1.c_cflag |= (CLOCAL | CREAD);
    termios1.c_cflag &= ~INPCK; // 关闭就校验
    termios1.c_cflag &= ~CSIZE;
    termios1.c_cflag &= ~PARENB; // 关闭校验位
    termios1.c_cflag |= CS8;     // 8位
    termios1.c_cflag &= ~CSTOPB;
    termios1.c_cc[VTIME] = 0;
    termios1.c_cc[VMIN] = 1;
    tcflush(dev_fd, TCIFLUSH);
    if(tcsetattr(dev_fd, TCSANOW, &termios1) != 0)
    {
        perror(USB_DEV " tcsetattr fail");
        close(dev_fd);
        exit(dev_fd);
    }
    // 打开文件
    if(argc != 1)
        bin_fd = open(argv[1], O_RDONLY);
    else
        bin_fd = open("./ec_main.bin", O_RDONLY);
    if(bin_fd == -1)
    {
        if(argc != 1)
            printf("%s ", argv[1]);
        else
            printf("./ec_main.bin ");
        perror("open fail");
        close(dev_fd);
        exit(bin_fd);
    }
    struct stat binstat;
    if(argc == 1)
        stat("./ec_main.bin", &binstat);
    else
        stat(argv[1], &binstat);
    signal(SIGINT, sighandler);
    printf("dev_fd = %d,bin_fd = %d,bin size = %ld\n", dev_fd, bin_fd, binstat.st_size);
    // 进行解包
    unsigned short state = 0, cnt = 0;
    unsigned char check[16] = { 0 };
    unsigned char cnt_max = binstat.st_size / 1024;
    unsigned int cnt_max_size = binstat.st_size % 1024;
    unsigned char buff[1024];
    unsigned char host_uart_rx = 0;
    const unsigned char error_Update = 0x06;
    double sec0, sec1, sec2;
    if(cnt_max_size)
        cnt_max++;
    printf("cnt_maxx: %u,cnt_max_size: %u\n", cnt_max, cnt_max_size);
    const unsigned char *cmd = "update firmware\n";
    gettimeofday(&t0, NULL);
    gettimeofday(&t1, NULL);
    sec0 = (double)t0.tv_sec + (double)t0.tv_usec / 1000000;
    sec1 = (double)t1.tv_sec + (double)t1.tv_usec / 1000000;
    write(dev_fd, cmd, strlen(cmd)); // 输入命令
    do
    {
        read(dev_fd, &host_uart_rx, 1);
        putc(host_uart_rx, stdout);
    }
    while(host_uart_rx != 0xff); // 判断命令分析结束
    gettimeofday(&t2, NULL);
    sec2 = (double)t2.tv_sec + (double)t2.tv_usec / 1000000;
    printf("\n cmd sec: %3.3lf\n", (sec2 - sec1));
    gettimeofday(&t1, NULL);
    sec1 = (double)t1.tv_sec + (double)t1.tv_usec / 1000000;
    do
    {
        read(dev_fd, &host_uart_rx, 1);
        putc(host_uart_rx, stdout);
        putc('\n', stdout);
    }
    while(host_uart_rx != '0'); // 判断进入DRAM
    do
    {
        read(dev_fd, &host_uart_rx, 1);
        putc(host_uart_rx, stdout);
        putc('\n', stdout);
    }
    while(host_uart_rx != '1'); // 判断擦写完成
    gettimeofday(&t2, NULL);
    sec2 = (double)t2.tv_sec + (double)t2.tv_usec / 1000000;
    printf(" start sec: %3.3lf\n", (sec2 - sec1));
    gettimeofday(&t1, NULL);
    sec1 = (double)t1.tv_sec + (double)t1.tv_usec / 1000000;
    do
    {
        read(dev_fd, &host_uart_rx, 1);
        putc(host_uart_rx, stdout);
        putc('\n', stdout);
    }
    while(host_uart_rx != '2'); // 判断擦写完成
    gettimeofday(&t2, NULL);
    sec2 = (double)t2.tv_sec + (double)t2.tv_usec / 1000000;
    printf(" erase sec: %3.3lf\n", (sec2 - sec1));
    gettimeofday(&t1, NULL);
    sec1 = (double)t1.tv_sec + (double)t1.tv_usec / 1000000;
    while(cnt <= cnt_max)
    {
    reburn:
        memset(buff, 0xff, 1024);
        if(cnt == cnt_max)
            read(bin_fd, buff, cnt_max_size);
        else
            read(bin_fd, buff, 1024);
        if(cnt == cnt_max)
            state = 1;
        memset(check, 0x0, 16);
        for(unsigned char checki = 0; checki < 8; checki++)
        {
            register unsigned char checksum, check_cnt;
            for(check_cnt = 0, checksum = 0; check_cnt < 128; check_cnt++)
            {
                checksum += buff[checki * 128 + check_cnt];
            }
            checksum &= 0xff;
            check[checki * 2] = (unsigned char)(checksum);
            check[(checki * 2) + 1] = ~(unsigned char)(checksum);
        }
        do
        {
            read(dev_fd, &host_uart_rx, 1);
            putc(host_uart_rx, stdout);
            putc('\n', stdout);
        }
        while(host_uart_rx != 'A'); // 判断可以写入
    rewrite:
        write(dev_fd, &state, sizeof(state));
        write(dev_fd, &cnt, sizeof(cnt));
        write(dev_fd, check, sizeof(check));
        write(dev_fd, buff, sizeof(buff));
        do
        {
            read(dev_fd, &host_uart_rx, 1);
            putc(host_uart_rx, stdout);
            putc('\n', stdout);
        }
        while(host_uart_rx != 'B'); // 判断数据接收完毕
        do
        {
            read(dev_fd, &host_uart_rx, 1);
            putc(host_uart_rx, stdout);
            putc('\n', stdout);
            if(host_uart_rx == 'E')
            {
                while(host_uart_rx != 'A')
                {
                    write(dev_fd, &error_Update, 1);
                    read(dev_fd, &host_uart_rx, 1);
                }
                goto rewrite;
            }
            if(host_uart_rx == 'F')
            {
                while(host_uart_rx != 'A')
                {
                    write(dev_fd, &error_Update, 1);
                    read(dev_fd, &host_uart_rx, 1);
                    cnt = 0;
                    lseek(dev_fd, 0, SEEK_SET);
                }
                goto reburn;
            }
        }
        while(host_uart_rx != 'D');
        cnt++;
        gettimeofday(&t2, NULL);
        sec2 = (double)t2.tv_sec + (double)t2.tv_usec / 1000000;
        printf(" cnt:%hd sec: %3.3lf\n", cnt, (sec2 - sec1));
        gettimeofday(&t1, NULL);
        sec1 = (double)t1.tv_sec + (double)t1.tv_usec / 1000000;
    }
    do
    {
        read(dev_fd, &host_uart_rx, 1);
        putc(host_uart_rx, stdout);
        putc('\n', stdout);
    }
    while(host_uart_rx != 'Z');
    gettimeofday(&t2, NULL);
    sec2 = (double)t2.tv_sec + (double)t2.tv_usec / 1000000;
    printf(" over sec: %3.3lf\n", (sec2 - sec1));
    gettimeofday(&t1, NULL);
    sec1 = (double)t1.tv_sec + (double)t1.tv_usec / 1000000;
    printf(" all sec: %3.3lf\n", (sec1 - sec0));
    close(bin_fd);
    close(dev_fd);
    system("cat " USB_DEV);
    return 0;
}