#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

/*************Variable************/
char check_flag = 1;
int dev_fd = 0;
unsigned long addr;
//unsigned long value;
unsigned char value;
char * ptr ;
unsigned long base_addr;
unsigned long other_addr;
uint32_t data_block_cnt = 0;
int update_cnt = 0;
uint32_t Update_Addr = 0;
int j = 0;

/*************Variable************/

/************MACRO****************/

#define   Version   "0.0.1"

#define   __USING_PMC2_PORT__

#define MAP_SIZE	0x2000
#define IO_BASE		0x0000018000000

//SHARE MEMORY MODULE REGISTER
#define  SMF_MAP_BASE   IO_BASE
#define  SMF_CMD        SMF_MAP_BASE+0x0
#define  SMF_FADDR0     SMF_MAP_BASE+0x1
#define  SMF_FADDR1     SMF_MAP_BASE+0x2
#define  SMF_FADDR2     SMF_MAP_BASE+0x3
#define  SMF_NUM        SMF_MAP_BASE+0x4
#define  SMF_SRA        SMF_MAP_BASE+0x5
#define  SMF_DR0        SMF_MAP_BASE+0x6
#define  SMF_DR1        SMF_MAP_BASE+0x7
#define  SMF_DR2        SMF_MAP_BASE+0x8
#define  SMF_DR3        SMF_MAP_BASE+0x9
#define  SMF_FSR        SMF_MAP_BASE+0xa
#define  SMF_FLASHSIZE  SMF_MAP_BASE+0xb
#define  SM_SR          SMF_MAP_BASE+0xc

//CMD/DATA CHANNEL
#ifdef __USING_PMC2_PORT__

#define EC_STATUS_PORT 	0x6C
#define EC_CMD_PORT 	0x6C
#define EC_DATA_PORT 	0x68
#define EC_OBF 		0x01
#define EC_IBF 		0x02

#else

#define EC_STATUS_PORT 	0x6C
#define EC_CMD_PORT 	0x6C
#define EC_DATA_PORT 	0x68
#define EC_OBF 		0x01
#define EC_IBF 		0x02

#endif

/************MACRO****************/

/************IO R/W SUPPORT FUNCTION************************/

void delay(unsigned int time)
{
 while (time--);
}

int io_byte_write(unsigned long addr, unsigned char value)
{
	dev_fd = open("/dev/mem",O_RDWR|O_SYNC);


	base_addr  = addr & 0xfffffffff00;
	other_addr = addr & 0xff;

	if(dev_fd < 0)
	{
		printf("Open /dev/mem failed \n");
		return 0;
	}

	unsigned char * map_base = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,dev_fd,base_addr);
	*(volatile unsigned char *)(map_base + other_addr) = value;
	if(dev_fd)
		close(dev_fd);

	munmap(map_base,MAP_SIZE);

}

uint8_t io_byte_read(unsigned long addr)
{
	dev_fd = open("/dev/mem",O_RDWR|O_SYNC);


	base_addr  = addr & 0xfffffffff00;
	other_addr = addr & 0xff;
	if(dev_fd < 0)
	{
		printf("Open /dev/mem failed \n");
		return 0;
	}

	unsigned char * map_base = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,dev_fd,base_addr);
        unsigned char var = *(volatile unsigned char *)(map_base + other_addr);
	//printf("0x%x\n", var);

	if(dev_fd)
		close(dev_fd);

	munmap(map_base,MAP_SIZE);
        
        return var;

}

/************IO R/W SUPPORT FUNCTION************************/

void chip_erase(void)
{
 //printf("erase flash chip start!\n");
 
 io_byte_write((IO_BASE + EC_CMD_PORT), 0xDC);   //SEND CHIP ERASE
}

/*************************main****************************/

int main(int argc, char *argv[])
{

 /*************initial variable**************/

 unsigned int length;

 int File_fd;

 char *File_Name = NULL; 

 FILE *fp;

 uint32_t data_sram_config;

 uint32_t File_Size = 0;

 uint32_t *Read_Buff_Ptr;

 uint8_t i = 0;

 uint8_t data_buff[256];

 uint32_t pagedata_buf[64];

 uint32_t page_addr;

 int progressbar_cnt = 0;

 char progressbar_buf[102];

 memset(progressbar_buf,'\0',sizeof(progressbar_buf));

 const char *lable = "|/-||";

 /*************initial variable**************/

 /**************executed code****************/

 io_byte_read(IO_BASE + EC_DATA_PORT);//清空数据寄存器
 if(argc == 1)
   {
        printf("please point out the file you want to update!\n");
   }
   else if(argc ==2)
   {
        File_Name = argv[1];

        if((fp = fopen(File_Name,"rb")) == NULL)
        {
        printf("file open error!!!\n");

        return -1;
        }

        //printf("open file success!\n");

        fseek(fp,0,2);

        File_Size = ftell(fp);

        fseek(fp,0,0);

        printf("file size is %d bytes!\n", File_Size);

        data_block_cnt = File_Size/256;

        if((File_Size%256) != 0)
        {
        data_block_cnt++;
        }

        printf("data_block_cnt is %d\n",data_block_cnt);

        Read_Buff_Ptr = (uint32_t *)malloc(File_Size+256);

        //printf("Read_Buff_Ptr is %p\n", Read_Buff_Ptr);

        fread(Read_Buff_Ptr,1,File_Size,fp);

        chip_erase();

        //printf("waiting for erase response 0x5A... ... \n");

        while(io_byte_read(IO_BASE + EC_DATA_PORT) != 0x5A);

        printf("______________________________________________________________________________________________________\n");
        printf("|                                                                                                     |\n");
        printf("|                                     EC Firmware Utility                                             |\n");
        printf("|                             Spksilicon Co., All rights reserved.                                    |\n");
        printf("|                                                                                                     |\n");
        printf("-------------------------------------------------------------------------------------------------------\n");
        printf("|                   NOTE:Please shut OS after progress bar load to 100% !!!                           |\n");
        printf("-------------------------------------------------------------------------------------------------------\n");

        //printf("EC firmware chip erase complete!\n");
        j=0;
        update_cnt=0;
        while(j<data_block_cnt)
        {
                //printf("当前烧录第%d块，总共有%d块\n",(j+1),data_block_cnt);
                page_addr = (uint32_t)(0x100*j);
                io_byte_write((SMF_FADDR0), (uint8_t)(page_addr & 0xFF));
                io_byte_write((SMF_FADDR1), (uint8_t)((page_addr >> 8) & 0xFF));
                io_byte_write((SMF_FADDR2), (uint8_t)((page_addr >> 16) & 0xFF));
                io_byte_write((IO_BASE + EC_DATA_PORT), 0xB5);   // send page address to ec
                //printf("waiting for 0x5B response ... ... \n");
                while(io_byte_read(IO_BASE + EC_DATA_PORT) != 0x5B);//wait for ec receive page address
                for(i=0; i<64; i++ )
                {
                        //printf("read_buff is 0x%lx\n", (Read_Buff_Ptr + 64*j + i));
                        pagedata_buf[i] = *(Read_Buff_Ptr + 64*j + i);
                        io_byte_write((SMF_DR0), (uint8_t)(pagedata_buf[i] & 0xFF));
                        io_byte_write((SMF_DR1), (uint8_t)((pagedata_buf[i] >> 8) & 0xFF));
                        io_byte_write((SMF_DR2), (uint8_t)((pagedata_buf[i] >> 16) & 0xFF));
                        io_byte_write((SMF_DR3), (uint8_t)((pagedata_buf[i] >> 24) & 0xFF));
                        //printf("DR0:0x%x,DR1:0x%x,DR2:0x%x,DR3:0x%x\n",io_byte_read(SMF_DR0),io_byte_read(SMF_DR1),io_byte_read(SMF_DR2),io_byte_read(SMF_DR3));
                        io_byte_write((IO_BASE + EC_DATA_PORT), 0xC5);
                        while(io_byte_read(IO_BASE + EC_DATA_PORT) != (i+1))//wait for ec receive page data
                        {
                                //校验失败且只有i=63时候才有可能进入if
                                if(io_byte_read(IO_BASE + EC_DATA_PORT) == 0xEE)
                                {
                                        check_flag=0;
                                        j=j-((page_addr%0x1000)/256);
                                        if(j<0)j=0;
                                        update_cnt=j;
                                        break;
                                }
                                else
                                {
                                        check_flag=1;
                                }
                        }
                }//page data send to ec
                if(check_flag==0)
                {
                        check_flag=1;
                        continue;
                }
                j++;
                update_cnt++;//update page count
                progressbar_cnt = (update_cnt*100)/data_block_cnt;
                if(progressbar_cnt <= 100)
                {
                        printf("[%-101s][%d%%][%c]\r", progressbar_buf,progressbar_cnt,lable[progressbar_cnt%4]);
                        fflush(stdout);
                        progressbar_buf[progressbar_cnt] = '=';
                }
                if(update_cnt >= data_block_cnt)
                {
                        //printf("烧录结束\n");
                        io_byte_write((IO_BASE + EC_DATA_PORT), 0xD5);   //
                }

        }//?end of for(int j=0; j<data_block_cnt; j++)
        free(Read_Buff_Ptr);
        fclose(fp);
    }
    else
    {
        printf("wrong arguments! please verify your argument number!\n");
    }

 /**************executed code****************/

 return 0;
}

