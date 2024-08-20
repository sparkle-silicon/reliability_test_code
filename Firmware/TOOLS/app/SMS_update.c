#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#define MAP_SIZE	0x2000000
#define EC_REG_BASE     0x1C000000
#define SRAM_BASE       0x12000000
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
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
uint32_t j = 0;
/************MACRO****************/
#define  SMF_MAP_BASE   0x18000000
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
/************MACRO****************/
/************IO R/W SUPPORT FUNCTION************************/
void delay(unsigned int time)
{
 while (time--);
}
int io_byte_write(unsigned long addr, unsigned char value)
{
	dev_fd = open("/dev/mem",O_RDWR|O_NDELAY);
	base_addr  = addr & 0xff000000;
	other_addr = addr & 0xffffff;
	if(dev_fd < 0)
	{
		printf("Open /dev/mem failed \n");
		return 0;
	}
	unsigned char * map_base = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,dev_fd,base_addr);
	//*(volatile unsigned long *)(map_base + other_addr) = value;
	*(volatile unsigned char *)(map_base + other_addr) = value;
	if(dev_fd)
		close(dev_fd);
	munmap(map_base,MAP_SIZE);
}
int io_string_write(unsigned long addr, unsigned char *str)
{
	dev_fd = open("/dev/mem",O_RDWR|O_NDELAY);
	base_addr  = addr & 0xff000000;
	other_addr = addr & 0xffffff;
	if(dev_fd < 0)
	{
		printf("Open /dev/mem failed \n");
		return 0;
	}
	unsigned char * map_base = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,dev_fd,base_addr);
        while (*str)
        {
                *(volatile unsigned char *)(map_base + other_addr) = *str;
                other_addr++;str++;
        }
        *(volatile unsigned char *)(map_base + other_addr) = *str;
	
	if(dev_fd)
		close(dev_fd);
	munmap(map_base,MAP_SIZE);
}
uint8_t io_byte_read(unsigned long addr)
{
	dev_fd = open("/dev/mem",O_RDWR|O_NDELAY);
	base_addr  = addr & 0xff000000;
	other_addr = addr & 0xffffff;
	if(dev_fd < 0)
	{
		printf("Open /dev/mem failed \n");
		return 0;
	}
	unsigned char * map_base = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,dev_fd,base_addr);
        //unsigned int var = *(volatile unsigned int *)(map_base + other_addr);
        unsigned char var = *(volatile unsigned char *)(map_base + other_addr);
	//printf("%#x\n", var);
	if(dev_fd)
		close(dev_fd);
	munmap(map_base,MAP_SIZE);
        
        return var;
}
int io_dword_write(unsigned long addr, unsigned long value)
{
	dev_fd = open("/dev/mem",O_RDWR|O_NDELAY);
	base_addr  = addr & 0xff000000;
	other_addr = addr & 0xffffff;
	if(dev_fd < 0)
	{
		printf("Open /dev/mem failed \n");
		return 0;
	}
	unsigned char * map_base = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,dev_fd,base_addr);
	*(volatile unsigned long *)(map_base + other_addr) = value;
	//*(volatile unsigned char *)(map_base + other_addr) = value;
	if(dev_fd)
		close(dev_fd);
	munmap(map_base,MAP_SIZE);
}
uint32_t io_dword_read(unsigned long addr)
{
	dev_fd = open("/dev/mem",O_RDWR|O_NDELAY);
	base_addr  = addr & 0xff000000;
	other_addr = addr & 0xffffff;
	if(dev_fd < 0)
	{
		printf("Open /dev/mem failed \n");
		return 0;
	}
	unsigned char * map_base = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,dev_fd,base_addr);
        unsigned int var = *(volatile unsigned int *)(map_base + other_addr);
        //unsigned char var = *(volatile unsigned char *)(map_base + other_addr);
	//printf("%#x\n", var);
	if(dev_fd)
		close(dev_fd);
	munmap(map_base,MAP_SIZE);
        
        return var;
}
/************IO R/W SUPPORT FUNCTION************************/
void chip_erase(void)
{
while (io_byte_read((SRAM_BASE + 0x100))!=0x00)
{
        /* code */
}    
 printf("erase flash chip start!\n");
 //io_byte_write(SMF_CMD,0xc7);
 //io_byte_write(SMF_CMD,0x20);
 //io_byte_write(SMF_FADDR0,0x00);
 //io_byte_write(SMF_FADDR1,0xc8);
 //io_byte_write(SMF_FADDR2,0x00);
 //io_byte_write(SMF_FSR,0x5);
 io_byte_write((SRAM_BASE + 0x100), 0x11);
 while (io_byte_read((SRAM_BASE + 0x100))!=0x22)
        {
                /* code */
        } 
}
void half_page_write(void)
{
 uint8_t Update_Addr0,Update_Addr1,Update_Addr2;
 //printf("data_block_cnt is %d\n", j);
 Update_Addr = (uint32_t)(0x100*j);
 //printf("Update_Addr is %#x\n", Update_Addr);
 Update_Addr0 = (uint8_t)(Update_Addr & 0xff);
 //printf("Update_Addr0 is %#x\n", Update_Addr0);
 Update_Addr1 = (uint8_t)((Update_Addr >> 8) & 0xff);
 //printf("Update_Addr1 is %#x\n", Update_Addr1);
 Update_Addr2 = (uint8_t)((Update_Addr >> 16) & 0xff);
 //printf("Update_Addr2 is %#x\n", Update_Addr2);
 //io_byte_write(SMF_CMD,0x02);
 //io_byte_write(SMF_FADDR0, Update_Addr0);
 //io_byte_write(SMF_FADDR1, Update_Addr1);
 //io_byte_write(SMF_FADDR2, Update_Addr2);
 //io_byte_write(SMF_NUM,0x80);
 //io_byte_write(SMF_FSR,0x5);
 io_byte_write((SRAM_BASE + 0x101), Update_Addr0);
 io_byte_write((SRAM_BASE + 0x102), Update_Addr1);
 io_byte_write((SRAM_BASE + 0x103), Update_Addr2);
 //io_byte_write((SRAM_BASE + 0x104), data_block_cnt);
 io_byte_write((SRAM_BASE + 0x100), 0xCC);
 update_cnt++;
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
 uint8_t *Sram_Base = (uint8_t *)(SRAM_BASE);
 uint8_t *Read_Buff_Ptr;
 uint32_t i = 0;
 uint8_t data_buff[256];
 io_string_write((SRAM_BASE + 0x100),"update firmware"); //default status
 //io_byte_write((SRAM_BASE + 0x100),0xAB); //default status
 /*************initial variable**************/
 /**************executed code****************/
 io_dword_write(0x10002000, 0xffff0040);
 data_sram_config = io_dword_read(0x10002000);
 printf("sram config is %#x!\nall is ready for ec firmware update!!!\n",data_sram_config);
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
          printf("open file success!\n");
          fseek(fp,0,2);
          File_Size = ftell(fp);
          fseek(fp,0,0);
          printf("file size is %d bytes!\n", File_Size);
          data_block_cnt = File_Size/256;
          if((File_Size%256) != 0)
           {
           data_block_cnt++;
           }
          //io_dword_write ((SRAM_BASE + 0x104), data_block_cnt);	//send data_block_cnt
          printf("data_block_cnt is %d\n",data_block_cnt);
          Read_Buff_Ptr = (uint8_t *)malloc(File_Size);
          printf("Read_Buff_Ptr is %p\n", Read_Buff_Ptr);
          fread(Read_Buff_Ptr,1,File_Size,fp);
          chip_erase();
          //printf("waiting for 0x55 response ... ... \n");
         // while(io_byte_read(SRAM_BASE + 0x100) != 0x55);
          printf("EC firmware chip erase complete!\n");
          //for(j=0; j<data_block_cnt; j++)
          for(j=0; j<data_block_cnt; j++)
          {
          //printf("EC num %d block Update start!\n", update_cnt);
          for(i=0; i<256; i++)
            {
            //printf("read_buff is %#x\n", *(Read_Buff_Ptr + 128*j + i));
            data_buff[i] = *(Read_Buff_Ptr + 256*j + i);
            //delay(100000);
            //printf("data_buff is %#x\n", data_buff[i]);
            io_byte_write((SRAM_BASE +0x200 + i), data_buff[i]);
            //delay(100000);
            //printf("Sram data is %#x\n", io_byte_read(SRAM_BASE + i));
            }//?end of for(i=0; i<128; i++)
            printf("EC can read data from SRAM\n");
         io_byte_write(SRAM_BASE + 0x100, 0xAA);
          //half_page_write();
          
          //printf("waiting for 0x55 response ... ... \n");
                printf("EC read OK\n");
          while(io_byte_read(SRAM_BASE + 0x100) != 0xBB);
          
         uint8_t Update_Addr0,Update_Addr1,Update_Addr2;
        //printf("data_block_cnt is %d\n", j);
        Update_Addr = (uint32_t)(0x100*j);
        //printf("Update_Addr is %#x\n", Update_Addr);
        Update_Addr0 = (uint8_t)(Update_Addr & 0xff);
        //printf("Update_Addr0 is %#x\n", Update_Addr0);
        Update_Addr1 = (uint8_t)((Update_Addr >> 8) & 0xff);
        //printf("Update_Addr1 is %#x\n", Update_Addr1);
        Update_Addr2 = (uint8_t)((Update_Addr >> 16) & 0xff);
        //printf("Update_Addr2 is %#x\n", Update_Addr2);
        //io_byte_write(SMF_CMD,0x02);
        //io_byte_write(SMF_FADDR0, Update_Addr0);
        //io_byte_write(SMF_FADDR1, Update_Addr1);
        //io_byte_write(SMF_FADDR2, Update_Addr2);
        //io_byte_write(SMF_NUM,0x80);
        //io_byte_write(SMF_FSR,0x5);
        io_byte_write((SRAM_BASE + 0x101), Update_Addr0);
        io_byte_write((SRAM_BASE + 0x102), Update_Addr1);
        io_byte_write((SRAM_BASE + 0x103), Update_Addr2);
        //io_byte_write((SRAM_BASE + 0x104), data_block_cnt);
        printf("EC can read addresses form SRAM\n");
        io_byte_write((SRAM_BASE + 0x100), 0xCC);
        printf("addresses read OK\n");
        while(io_byte_read(SRAM_BASE + 0x100) != 0xDD);
        update_cnt++;
          //printf ("update flash num %d completed!\n", update_cnt);
          if(update_cnt >= data_block_cnt)
          printf("EC Firmware Update Complete!Reset EC and run it again please!\n");
          }//?end of for(int j=0; j<data_block_cnt; j++)
        io_byte_write((SRAM_BASE + 0x100), 0xFF);
          free(Read_Buff_Ptr);
          fclose(fp);
          //printf("free buff and quid cache!\n");
         }
         else
            {
            printf("wrong arguments! please verify your argument number!\n");
            }
 /**************executed code****************/
 return 0;
}
