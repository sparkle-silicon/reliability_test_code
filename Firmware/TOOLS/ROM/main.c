#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
//gcc ./TOOLS/ROM/main.c -o rom.out
int main(int argc, char **argv)
{
    int ret;
    struct stat file_stat;
    int addr;
    int cnt=0;
    //if(argc>=2)
    //addr=strtol(argv[1],&argv[2],16);
    //else
    // {
    //     printf("%s addr_hex\n",argv[0]);
    //     return 0;
    // } 
    //printf("addr = %#x\n",addr);
    int elf_fd=open("./ec_main.bin",O_RDWR,S_IRWXU|S_IRWXG|S_IRWXO );
	if(elf_fd<0){perror("elf_fd open error");exit(-1);}
    ret = fstat(elf_fd, &file_stat);
    if(ret<0){perror("fstat elf_fd error");exit(-1);}
	void *elf_mp = mmap(NULL,file_stat.st_size,PROT_READ,MAP_SHARED,elf_fd,0);
    if(elf_mp<0){perror("elf_mp mmap error");exit(-1);}
    //unsigned char *read_mp=addr+(unsigned char *)elf_mp;
    unsigned char *read_mp=(unsigned char *)elf_mp;
    cnt=0;
    while(cnt<file_stat.st_size)//找到位置
    {
        if(*(unsigned int *)(read_mp+cnt)==0x00010084)
        {
            if(*(unsigned int *)(read_mp+cnt+4)==0x00010400)
            {
                printf("find rom code\n");
                break;
            }
        }
        cnt++;
    }
    if(cnt>=file_stat.st_size)
    {
         printf("not find rom code\n");
        ret = munmap(elf_mp,file_stat.st_size);
        close(elf_fd);
        exit(-1);
    }
    else read_mp+=cnt;
	int rom_fd = open("./rom_main.bin",O_RDWR|O_CREAT|O_TRUNC,S_IRWXU|S_IRWXG|S_IRWXO );
	if(rom_fd<0){perror("rom_main open error");exit(-1);}
    ret = ftruncate(rom_fd,0x8000);
     if(ret<0){perror("ftruncate rom_fd error");exit(-1);}
	void *rom_mp = mmap(NULL,0x8000,PROT_READ|PROT_WRITE,MAP_SHARED,rom_fd,0);
    if(rom_mp<0){perror("rom_mp mmap error");exit(-1);}
    unsigned char *wirte_mp=(unsigned char *)rom_mp;
    cnt=0;
    while(cnt<0x8000)
    {
       *(wirte_mp+cnt)=*(read_mp+cnt);
       cnt++;
    }
    ret = munmap(elf_mp,file_stat.st_size);
    ret = munmap(rom_mp,0x8000);
    close(elf_fd);
    close(rom_fd);
    return 0;
}