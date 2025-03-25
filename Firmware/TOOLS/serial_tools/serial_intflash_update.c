#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>

/* 串口更新外部flash程序

 * 注意：
 * 1.确认好串口设备与波特率
 * 2.只支持输入bin文件，不支持输入hex文件
 *
 * 使用：
 * 1. 编译：gcc -o serial_intflash_update serial_intflash_update.c
 * 2. 运行：./serial_intflash_update [bin文件] [flash起始地址]
 * 如果需要备份则在运行时加上参数-b 需要更新后复位加上参数-r例如：
 * ./serial_intflash_update [bin文件] [flash起始地址] -b -r
*/

#define SERIAL_PORT "/dev/ttyUSB0"  // 串口设备文件
#define BLOCK_SIZE 256  // 每次发送的块大小
#define ACK_BYTE 0xAA  // 单片机响应的确认字节
#define ERR_BYTE 0xEE  // 单片机响应的错误字节
#define STP_BYTE 0xCC  // 终止更新字节
// #define TIMEOUT_SEC 20  // 等待 ACK 的超时时间，单位秒
#define MAX_RETRIES 500             // 最大重试次数
#define TIMEOUT_USEC 50000         // 50ms读一次下位机回应

int backup_enabled = 0;
int rest_flag=0;
unsigned int backup_addr = 0;
unsigned int backup_size = 0;

unsigned char ERR_CODE = 0xff;  // 错误码
unsigned char response;
typedef struct Flash_Firmware_Info
{
    unsigned int Flash_Fw_Size;
    unsigned int Flash_CopyAddr;
    unsigned int Fw_CRC32;
    unsigned char Back_En;
    unsigned char Rest_Flag;
} sFlash_Firmware_Info;
sFlash_Firmware_Info flashAndFWInfo;

// 函数：接收数据，使用select()避免阻塞
int receive_data(int fd, unsigned char *buffer, size_t length)
{
    fd_set readfds;
    struct timeval timeout;

    // 设置超时为10毫秒
    timeout.tv_sec = 0;
    timeout.tv_usec = TIMEOUT_USEC;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    // 使用select()等待串口数据
    int ret = select(fd + 1, &readfds, NULL, NULL, &timeout);
    if(ret < 0)
    {
        perror("select() error");
        return -1;
    }

    if(ret == 0)
    {
        return 0;  // Timeout, no data
    }

    // 如果串口有数据，读取数据
    ssize_t n = read(fd, buffer, length);
    if(n < 0)
    {
        perror("Error reading from serial port");
        return -1;
    }

    return n;  // 返回读取的字节数
}

void delay_seconds(int seconds)
{
    sleep(seconds);
}

// CRC-8常用参数：
// 多项式 (x^8 + x^2 + x + 1) => 0x07
// 初始值 0x00
// 输出 XOR 值 0x00
// 数据输入是字节（8位）
unsigned char CRC8(const unsigned char *data, size_t length)
{
    unsigned char crc = 0x00;  // 初始值
    unsigned char polynomial = 0x07;  // CRC-8多项式：x^8 + x^2 + x + 1 (0x07)

    for(size_t i = 0; i < length; i++)
    {
        crc ^= data[i];  // 将当前字节与 CRC 值异或

        // 对每一位执行 CRC 计算
        for(int j = 0; j < 8; j++)
        {
            if(crc & 0x80)
            {  // 如果 CRC 的最高位为1
                crc = (crc << 1) ^ polynomial;  // 左移并异或多项式
            }
            else
            {
                crc <<= 1;  // 否则仅左移
            }
        }
    }
    return crc;  // 返回最终的 CRC 值
}


// CRC32 手动实现
unsigned int CRC32(unsigned int crc, const unsigned char *buf, size_t len)
{
    crc = ~crc;
    while(len--)
    {
        crc ^= *buf++;
        for(int i = 8; i; i--)
        {
            crc = (crc & 1) ? (crc >> 1) ^ 0xedb88320 : crc >> 1;
        }
    }
    return ~crc;
}

// 打开串口
int open_serial_port(const char *port_name)
{
    int fd = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1)
    {
        perror("Unable to open serial port");
        return -1;
    }
    return fd;
}

// 配置串口
int configure_serial_port(int fd)
{
    struct termios options;
    if(tcgetattr(fd, &options) < 0)
    {
        perror("tcgetattr");
        return -1;
    }

    cfsetispeed(&options, B115200);  // 设置波特率
    cfsetospeed(&options, B115200);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;  // 8位数据位
    options.c_cflag &= ~CSTOPB;  // 1位停止位
    options.c_cflag &= ~PARENB;  // 无奇偶校验
    options.c_cflag &= ~CRTSCTS;  // 禁用硬件流控制
    options.c_cflag |= CREAD | CLOCAL;  // 允许接收数据并忽略调制解调器状态线

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // 设置为原始模式
    options.c_iflag &= ~(IXON | IXOFF | IXANY);  // 禁用软件流控制
    options.c_oflag &= ~OPOST;

    if(tcsetattr(fd, TCSANOW, &options) < 0)
    {
        perror("tcsetattr");
        return -1;
    }

    return 0;
}

// 等待单片机确认（带超时）
int wait_for_ack(int fd)
{
    ssize_t n;
    int retries = 0;
    response = 0;
    while(retries < MAX_RETRIES)
    {

        n = receive_data(fd, &response, 1);
        if(n < 0)
        {
            //close(fd);
            return 1;
        }

        if(n > 0 && response == ACK_BYTE)
        {
            printf("Received ACK_BYTE: 0x%02X\n", response);
            return 0;
        }
        else if(n > 0 && response == ERR_BYTE)
        {
            printf("Received ERR_BYTE: 0x%02X\n", response);
            return 2;
        }
        else if(n > 0 && response == STP_BYTE)
        {
            printf("Received STP_BYTE: 0x%02X\n", response);
            return 0;
        }

        retries++;
        printf("No response received, retrying... (%d/%d)\n", retries, MAX_RETRIES);
    }
    if(retries >= MAX_RETRIES)
    {
        printf("Max retries reached, no valid response received.\n");
        //close(fd);
        return 1;
    }
}



// 发送数据
ssize_t send_data(int fd, const void *data, size_t length)
{
    return write(fd, data, length);
}

// 发送文件大小和CRC32值
int send_file_header(int fd, size_t file_size, unsigned int crc32)
{
    unsigned char header[257] = { 0 };
    flashAndFWInfo.Flash_Fw_Size = file_size;
    flashAndFWInfo.Fw_CRC32 = crc32;
    memcpy(header, &flashAndFWInfo, sizeof(flashAndFWInfo));

    header[256] = CRC8(header, 256);
    printf("CRC8: 0x%02X\n", header[256]);
    // 发送头部数据
again:
    if(send_data(fd, header, sizeof(header) + 1) < 0)
    {
        perror("Failed to send file header");
        return -1;
    }

    // 等待确认
    if(wait_for_ack(fd))
    {
        while((response != ERR_BYTE) && (response != ACK_BYTE))//直到下位机回复数据
        {
            read(fd, &response, 1);
            send_data(fd, &ERR_CODE, 1);
        }
        printf("Received ERR_ACK: 0x%02X\n", response);
        if(response == ERR_BYTE)
        {
            while(wait_for_ack(fd));
            printf("header send retry\n");
            goto again;
        }
    }
    if(response == STP_BYTE)
    {
        printf("stop update\n");
        return -1;
    }

    return 0;
}

// 发送文件内容
int send_file_data(int fd, const char *file_path)
{
    FILE *file = fopen(file_path, "rb");
    if(!file)
    {
        perror("Failed to open file");
        return -1;
    }

    // 获取文件大小
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);//文件大小
    fseek(file, 0, SEEK_SET);
    printf("Original file size: %ld bytes\n", file_size);

    // 如果文件大小不是4096的倍数，进行4096字节对齐并补充0xFF
    char *file_data = NULL;
    unsigned int crc32 = 0;
    if(file_size % 4096 != 0)
    {
        long aligned_size = file_size + (4096 - (file_size % 4096));  // 计算对齐后的大小
        printf("Aligned file size: %ld bytes\n", aligned_size);
        file_data = malloc(aligned_size);  // 分配足够的内存

        if(!file_data)
        {
            perror("Failed to allocate memory");
            fclose(file);
            return -1;
        }

        fread(file_data, 1, file_size, file);  // 读取原始文件数据
        memset(file_data + file_size, 0xFF, aligned_size - file_size);  // 用0xFF填充对齐的部分

        // 计算CRC32
        crc32 = CRC32(0, (unsigned char *)file_data, aligned_size);
        printf("CRC32: 0x%08x\n", crc32);
        file_size = aligned_size;
    }
    else
    {
        // 文件大小已经是4096的倍数，直接计算CRC32
        file_data = malloc(file_size);
        if(!file_data)
        {
            perror("Failed to allocate memory");
            fclose(file);
            return -1;
        }

        fread(file_data, 1, file_size, file);  // 读取文件数据

        // 计算CRC32
        crc32 = CRC32(0, (unsigned char *)file_data, file_size);
        printf("CRC32: 0x%08x\n", crc32);
    }

    //发送开始进行更新的命令
    char update_cmd[16] = { 0x75 ,0x70 ,0x64 ,0x61 ,0x74 ,0x65 ,0x20 ,0x66 ,0x69 ,0x6d ,0x77 ,0x61 ,0x72 ,0x65 ,0x2D ,0x69};
    if(send_data(fd, update_cmd, sizeof(update_cmd)) < 0)
    {
        printf("Failed to send update command\n");
        fclose(file);
        return -1;
    }
    // 等待确认
    if(wait_for_ack(fd))
    {
        return -1;
    }
    printf("update command ack received\n");

    // 发送dram加密文件大小、CRC32值和哈希密文
    printf("Header sent\n");
    if(send_file_header(fd, file_size, crc32) != 0)
    {
        fclose(file);
        return -1;
    }
    printf("Header received\n");

    // 逐块发送文件内容
    unsigned char buffer[BLOCK_SIZE + 1];
    unsigned int block_count = 0;
    fseek(file, 0, SEEK_SET); // 将文件指针移动到文件起始
    printf("The file size to be sent is %ld bytes and the number of blocks is %ld\n", (file_size), ((file_size) / 256));
    while(block_count < ((file_size) / 256))
    {
        for(int i = 0; i < BLOCK_SIZE; i++)
        {
            buffer[i] = file_data[block_count * BLOCK_SIZE + i];
        }
        // 计算CRC8
        buffer[BLOCK_SIZE] = CRC8(buffer, BLOCK_SIZE);
        printf("Sending block %d All block: %ld  current block crc8: 0x%x\n", block_count + 1, ((file_size) / 256), buffer[BLOCK_SIZE]);
        // 发送文件块
    anew:
        if(send_data(fd, buffer, (BLOCK_SIZE + 1)) < 0)//固定发送257字节数据块
        {
            printf("Failed to send file block");
            fclose(file);
            return -1;
        }
        // 等待确认
        if(wait_for_ack(fd))
        {
            while((response != ERR_BYTE) && (response != ACK_BYTE))//直到下位机回复数据
            {
                read(fd, &response, 1);
                send_data(fd, &ERR_CODE, 1);
            }
            printf("Received ERR_ACK: 0x%02X\n", response);
            if(response == ERR_BYTE)
            {
                while(wait_for_ack(fd));
                printf("block %d retry\n", block_count+1);
                goto anew;
            }
        }
        block_count++;
    }
    free(file_data);
    fclose(file);
    return 0;
}

int main(int argc, char *argv[])
{
// 检查命令行参数
    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s <path_to_bin_file> <flash_copy_address>\n", argv[0]);
        return -1;
    }
    // 检查是否启用备份功能/更新后复位cpu功能
    if(argc >= 4 && argc<=5)
    {
        rest_flag=0;
        backup_enabled=0;
        for (int i = 3; i < (argc); i++)
        {
            if (strcmp(argv[i], "-r") == 0)
            {
                rest_flag = 1;
                printf("Reset cpu after update.\n");
            }
            if(strcmp(argv[i], "-b") == 0)
            {
                backup_enabled = 1;
                printf("Backup enabled.\n");
            }
        }
    }
    else
    {
        backup_enabled=0;
        rest_flag=0;
        printf("Backup disabled.\n");
    }
    flashAndFWInfo.Back_En=backup_enabled;
    flashAndFWInfo.Rest_Flag=rest_flag;
    const char *file_path = argv[1];  // 从命令行参数获取文件路径
    unsigned int flash_copy_addr = strtol(argv[2], NULL, 16);
    flash_copy_addr&=0xFFFFF000;//4k对齐
    printf("Flash copy address: 0x%08x\n", flash_copy_addr);
    flashAndFWInfo.Flash_CopyAddr = flash_copy_addr;

    // 打开串口
    int fd = open_serial_port(SERIAL_PORT);
    if(fd == -1)
    {
        return -1;
    }

    // 配置串口
    if(configure_serial_port(fd) != 0)
    {
        close(fd);
        return -1;
    }

    // 发送BIN文件
    if(send_file_data(fd, file_path) != 0)
    {
        close(fd);
        return -1;
    }

    // 关闭串口
    close(fd);
    return 0;
}
