/*
 * @Author: Linyu
 * @LastEditors: Linyu
 * @LastEditTime: 2023-04-02 21:17:18
 * @Description: 
 * 
 * 
 * The following is the Chinese and English copyright notice, encoded as UTF-8.
 * 以下是中文及英文版权同步声明，编码为UTF-8。
 * Copyright has legal effects and violations will be prosecuted.
 * 版权具有法律效力，违反必究。
 * 
 * Copyright ©2021-2023 Sparkle Silicon Technology Corp., Ltd. All Rights Reserved.
 * 版权所有 ©2021-2023龙晶石半导体科技（苏州）有限公司
 */
#include "auto_test.h"//头文件集合
#define TEST_PRINT 0//测试端口接
#define ONCE_TEST 
//锁
pthread_mutex_t  LPC_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  KBC_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  PMC1_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  PMC2_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  PMC3_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  PMC4_lock=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  PMC5_lock=PTHREAD_MUTEX_INITIALIZER;
//链表
peripheral_list subroutine_list[]={//功能链表
    {.name="PMC1",  .select=0,.succeed=0,.error=0,.id=0,.func=pmc_subroutine,   .IO=PMC1_DR,.SR_CMD=PMC1_SR_CMD,.lock=&PMC1_lock},
    {.name="PMC2",  .select=0,.succeed=0,.error=0,.id=0,.func=pmc_subroutine,   .IO=PMC2_DR,.SR_CMD=PMC2_SR_CMD,.lock=&PMC2_lock},
    {.name="PMC3",  .select=0,.succeed=0,.error=0,.id=0,.func=pmc_subroutine,   .IO=PMC3_DR,.SR_CMD=PMC3_SR_CMD,.lock=&PMC3_lock},
    {.name="PMC4",  .select=0,.succeed=0,.error=0,.id=0,.func=pmc_subroutine,   .IO=PMC4_DR,.SR_CMD=PMC4_SR_CMD,.lock=&PMC4_lock},
    {.name="PMC5",  .select=0,.succeed=0,.error=0,.id=0,.func=pmc_subroutine,   .IO=PMC5_DR,.SR_CMD=PMC5_SR_CMD,.lock=&PMC5_lock},
    {.name="KBC",   .select=0,.succeed=0,.error=0,.id=0,.func=kbc_subroutine,   .IO=KBC_DR,.SR_CMD=KBC_SR_CMD,.lock=&KBC_lock},
    {.name="SMRAND",.select=0,.succeed=0,.error=0,.id=0,.func=sm_subroutine,    .IO=0,.SR_CMD=0},
    {.name="SMRDWR",.select=0,.succeed=0,.error=0,.id=0,.func=rw_subroutine,    .IO=0,.SR_CMD=0},
    {.name="SPACE", .select=0,.succeed=0,.error=0,.id=0,.func=Space_subroutine, .IO=PMC5_DR,.SR_CMD=PMC5_SR_CMD,.lock=&PMC5_lock},
    {.name="PWM0",  .select=0,.succeed=0,.error=0,.id=0,.func=PWM_subroutine,   .IO=PMC5_DR,.SR_CMD=PMC5_SR_CMD,.lock=&PMC5_lock},
    {.name="GPIOA", .select=0,.succeed=0,.error=0,.id=0,.func=GPIO_subroutine,  .IO=PMC5_DR,.SR_CMD=PMC5_SR_CMD,.lock=&PMC5_lock},
    {.name="GPIOB", .select=0,.succeed=0,.error=0,.id=0,.func=GPIO_subroutine,  .IO=PMC4_DR,.SR_CMD=PMC4_SR_CMD,.lock=&PMC4_lock},
    {.name="I2C0",  .select=0,.succeed=0,.error=0,.id=0,.func=I2C_subroutine,   .IO=KBC_DR,.SR_CMD=KBC_SR_CMD,.lock=&KBC_lock},
    {.name="I2C1",  .select=0,.succeed=0,.error=0,.id=0,.func=I2C_subroutine,   .IO=PMC1_DR,.SR_CMD=PMC1_SR_CMD,.lock=&PMC1_lock},
    {.name="I2C2",  .select=0,.succeed=0,.error=0,.id=0,.func=I2C_subroutine,   .IO=PMC2_DR,.SR_CMD=PMC2_SR_CMD,.lock=&PMC2_lock},
    {.name="I2C3",  .select=0,.succeed=0,.error=0,.id=0,.func=I2C_subroutine,   .IO=PMC3_DR,.SR_CMD=PMC3_SR_CMD,.lock=&PMC3_lock},
    {.name="UART0", .select=0,.succeed=0,.error=0,.id=0,.func=UART_subroutine,  .IO=PMC1_DR,.SR_CMD=PMC1_SR_CMD,.lock=&PMC1_lock},
    {.name="UART1", .select=0,.succeed=0,.error=0,.id=0,.func=UART_subroutine,  .IO=PMC4_DR,.SR_CMD=PMC4_SR_CMD,.lock=&PMC4_lock},
    {.name="UART2", .select=0,.succeed=0,.error=0,.id=0,.func=UART_subroutine,  .IO=PMC2_DR,.SR_CMD=PMC2_SR_CMD,.lock=&PMC2_lock},
    {.name="UART3", .select=0,.succeed=0,.error=0,.id=0,.func=UART_subroutine,  .IO=PMC3_DR,.SR_CMD=PMC3_SR_CMD,.lock=&PMC3_lock},
    {.name="CAN01",  .select=0,.succeed=0,.error=0,.id=0,.func=CAN_subroutine,  .IO=PMC1_DR,.SR_CMD=PMC1_SR_CMD,.lock=&PMC1_lock},
    {.name="CAN23",  .select=0,.succeed=0,.error=0,.id=0,.func=CAN_subroutine,  .IO=PMC2_DR,.SR_CMD=PMC2_SR_CMD,.lock=&PMC2_lock},
};
//变量
int mem_fd=-1;//共享内存
int log_fd=-1;//共享日志
int list_size=0;
int space_number=0;
unsigned char *map10=NULL;
unsigned char *map12=NULL;
unsigned char *map18=NULL;
//进程用
pthread_mutex_t *mutex=NULL;
pthread_mutexattr_t *lock_set=NULL;
int Ms_sleep=0;//延时,默认1
int Us_sleep=0;//微秒,默认0
time_t stop_time=0;//暂停时间
int subroutine;//0为线程方式，1为进程方式，默认为线程模式
time_t time_s,time_cnt,time_o,time_stop;
char time_char[30]={'0'};
//初始化函数
void kbc_init(void)
{
    //配置KBC 0x18000060/0x18000064
    //配置端口
    *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x07;
    *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x06;
    *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x61;
    *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x60;
    *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x63;
    *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x64;
    //配置中断
    *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x70;
    *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x01;
    *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x71;
    *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x00;
    //确认配置
    *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x30;
    *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x01;
}
void pmc_init(char *name)
{
    if(!strcmp(name,"PMC1"))
    {
        //配置PMC1 0x18000062/0x18000066
        //配置端口
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x07;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x11;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x61;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x62;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x63;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x66;
        //配置中断
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x70;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x05;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x71;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x00;
        //确认配置
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x30;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x01;
    }
    else if(!strcmp(name,"PMC2"))
    {
        //配置PMC2 0x18000068/0x1800006C
        //配置端口
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x07;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x12;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x61;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x68;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x63;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x6C;
        //配置中断
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x70;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x06;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x71;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x00;
        //确认配置
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x30;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x01;
    }
    else if(!strcmp(name,"PMC3"))
    {
        //配置PMC3 0x1800006A/0x1800006E
        //配置端口
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x07;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x17;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x61;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x6A;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x63;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x6E;
        //配置中断
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x70;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x07;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x71;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x00;
        //确认配置
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x30;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x01;
    }
    else if(!strcmp(name,"PMC4"))
    {
        //配置PMC4 0x18000074/0x18000078
        //配置端口
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x07;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x18;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x61;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x74;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x63;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x78;
        //配置中断
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x70;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x08;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x71;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x00;
        //确认配置
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x30;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x01;
    }
    else if(!strcmp(name,"PMC5"))
    {
        //配置PMC5 0x1800007A/0x1800007C
        //配置端口
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x07;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x19;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x61;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x7A;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x63;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x7C;
        //配置中断
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x70;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x09;
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x71;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x00;
        //确认配置
        *(volatile unsigned char *)((OFFSET_DEF&0x2e)+map18)=0x30;
        *(volatile unsigned char *)((OFFSET_DEF&0x2f)+map18)=0x01;
    }
}
void Init(void)
{
    mem_fd=open("/dev/mem",O_RDWR|O_NDELAY);//共享内存
    if(mem_fd < 0){
        perror("/dev/mem open fail");
        exit(mem_fd);
    }
    //配置初始地址
    map10 = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,mem_fd,BASE_DEF&0x10000000);
    if(map10 == MAP_FAILED)
    {
        close(mem_fd);
        perror("mmap share memroy fail");
        exit(-1);
    }
    map12 = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,mem_fd,BASE_DEF&0x12000000);
    if(map12 == MAP_FAILED)
    {
        close(mem_fd);
        perror("mmap share memroy fail");
        exit(-1);
    }
    map18 = (unsigned char *)mmap(NULL,MAP_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,mem_fd,BASE_DEF&0x18000000);
    if(map18 == MAP_FAILED)
    {
        close(mem_fd);
        perror("mmap share memroy fail");
        exit(-1);
    }
    log_fd=open("./test_log.txt",O_RDWR|O_CREAT|O_TRUNC,0666);//日志文件
    if(log_fd < 0)
    {
        perror("test_log.txt open fail");
        close(mem_fd);
        exit(log_fd);
    }
    pmc_init("PMC5");
}
void UnInit(void)
{
    int list_cnt=0;
    char log[200];
    double sprob=0;
    double eprob=0;
    u32 success=0;
    u32 error=0;
    if(subroutine)
    {
        printf("wait process\n");
    }
    time_o = time(NULL);
    time_cnt=time_o-time_s;
    sprintf(log,"[Total test duration:%06ld:%02ld:%02ld]\n",time_cnt/3600,time_cnt/60%60,time_cnt%60);
    printf("%s",log);
    write(log_fd,log,strlen(log));
    while (list_cnt<list_size)//进入对应函数对应函数
    {
        if(subroutine_list[list_cnt].select==1)
        {
            success = subroutine_list[list_cnt].succeed;
            error = subroutine_list[list_cnt].error;
            sprob = (double)success/(double)(success+error);
            eprob = (double)error/(double)(success+error);
            sprob *= 100;
            eprob *= 100;
            sprintf(log,"[%s test]\nsucceed:%010lu,error:%010lu\nsuccess rate:%02.3lf%%,error rate:%02.3lf%%\n",subroutine_list[list_cnt].name,success,error,sprob,eprob);
            printf("%s",log);
            write(log_fd,log,strlen(log));
        }
        else
        {
            sprintf(log,"[%s test] not run\n",subroutine_list[list_cnt].name);
            printf("%s",log);
            write(log_fd,log,strlen(log));
        }
        list_cnt++;
    }
    munmap(map10,MAP_SIZE);
    munmap(map12,MAP_SIZE);
    munmap(map18,MAP_SIZE);
    close(mem_fd);
    close(log_fd);
}
//信号函数
void sighandler(int sig)
{
    pthread_mutex_lock(&LPC_lock);
    if(sig == SIGINT)
    {
        UnInit();
        printf("\nexit now\n");
        exit(0);
    }
}
//发送并接收数据 to_where: TO_CMD TO_IO
unsigned char data_send_recv(int list_cnt, unsigned char to_where, unsigned char val)
{
    if(to_where == TO_CMD) *(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))=val;
    else *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO))=val;
    msleep(Ms_sleep,Us_sleep);
    while (!((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS))
    {
        msleep(Ms_sleep,Us_sleep);
    }
    val=*(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
    return val;
}
//发送并判断返回的数据 1对0错
unsigned char data_send_check(int list_cnt, unsigned char to_where, unsigned char val)
{
    if(to_where == TO_CMD) *(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))=val;
    else *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO))=val;
    msleep(Ms_sleep,Us_sleep);
    while (!((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS))
    {
        msleep(Ms_sleep,Us_sleep);
    }
    return val == *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO)) ? 1 : 0;
}
//主函数,轮询调用测试信息
int main(int argc,char *argv[])//主函数
{
    system("clear");
    time_s=time(NULL);
    list_size=sizeof(subroutine_list)/sizeof(peripheral_list);//链表大小
    space_number=sizeof(ECSpace)/sizeof(EC_Space);//链表大小
    #if TEST_PRINT
    printf("list size = %d\n",list_size);
    #endif
    if(argc == 1)//帮助信息
    {
        printf(test_version);
        printf(test_usage);
        return 0;
    }
    Init();//初始化
    int argcnt=1;
    while (argcnt<argc)//进入轮询
    {
        
        if(strstr(argv[argcnt],"time=")!=NULL)//延时配置
        {
            sscanf(argv[argcnt],"time=%d.%d",&Ms_sleep,&Us_sleep);
            printf("time set %dms %dus\n",Ms_sleep,Us_sleep);
            argcnt++;
            continue;
        }
        if(strstr(argv[argcnt],"stop=")!=NULL)//延时配置
        {
            sscanf(argv[argcnt],"stop=%ld",&stop_time);
            printf("stop set %lds\n",stop_time);
            argcnt++;
            continue;
        }
        if(!strcmp(argv[argcnt],"thread"))//线程配置
        {
            subroutine=0;
            printf("set %s now\n",argv[argcnt]);
            argcnt++;
            continue;
        }
        else if(!strcmp(argv[argcnt],"process"))//进程配置
        {
            subroutine=1;
            printf("set %s now\n",argv[argcnt]);
            argcnt++;
            continue;
        }
        int list_cnt=0;
        while (list_cnt<list_size)//查询对应函数
        {
            
            if(strcmp(argv[argcnt],subroutine_list[list_cnt].name))
            {list_cnt++;continue;}
            else
            {subroutine_list[list_cnt].select=1; break;}
        }
        argcnt++;
    }
    int list_cnt=0;
    int ret=0;
    while (list_cnt<list_size)//进入对应函数对应函数
    {
        if(subroutine_list[list_cnt].select==1)
        {
            if(subroutine)//进程模式
            {
                printf("%s process fork now\n",subroutine_list[list_cnt].name);
                subroutine_list[list_cnt].id=(pthread_t)fork();
                if(subroutine_list[list_cnt].id!=0)
                {
                    printf("%s process fork%lu\n",subroutine_list[list_cnt].name,subroutine_list[list_cnt].id);
                    list_cnt++;
                    continue;
                }
                else
                {
                    printf("%s process run now\n",subroutine_list[list_cnt].name);
                    subroutine_list[list_cnt].func(subroutine_list[list_cnt].name);
                }
            }
            else//线程模式
            {
                printf("%s pthread create now\n",subroutine_list[list_cnt].name);
                ret=pthread_create(&subroutine_list[list_cnt].id,NULL,subroutine_list[list_cnt].func,(void *)subroutine_list[list_cnt].name);
                if(ret)
                {
                    perror("pthread create fail");
                    continue;
                }
                printf("%s pthread create id %lu\n",subroutine_list[list_cnt].name,subroutine_list[list_cnt].id);
            }
        }
        list_cnt++;
    }
    signal(SIGINT,sighandler);
    printf("main loop now\n");
    while (1)
    {
        time_o=time(NULL);
        time_cnt=time_o-time_s;
        sprintf(time_char,"\n[%06ld:%02ld:%02ld]",(time_cnt/60/60),(time_cnt/60%60),(time_cnt%60));
        printf("%s",time_char);
        if(stop_time!=0 && time_cnt>=stop_time)break;
        //write(log_fd,time_char,strlen(time_char));
        sleep(1);
    }
    pthread_mutex_lock(&LPC_lock);
    UnInit();
    return 0;
}
void *pmc_subroutine(void *name)//pmc主程序
{
    printf ("%s START\n",(char *)name);
    pmc_init((char *)name);
    int list_cnt=0;
    unsigned char var=0;
    pthread_mutex_lock(subroutine_list[list_cnt].lock);
    if((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS)
    {
        *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
    }
    msleep(Ms_sleep,Us_sleep);
    pthread_mutex_unlock(subroutine_list[list_cnt].lock);
    while(list_cnt<list_size&&strcmp(subroutine_list[list_cnt].name,name))list_cnt++;
    srand(time(NULL));
    while (1)
    {
        var=rand()%256;
        pthread_mutex_lock(subroutine_list[list_cnt].lock);
        if(data_send_check(list_cnt,TO_IO,var))subroutine_list[list_cnt].succeed++;
        else
        {
            subroutine_list[list_cnt].error++;
            printf("error %8ld of %s\n",subroutine_list[list_cnt].error,subroutine_list[list_cnt].name);
        }
        pthread_mutex_unlock(subroutine_list[list_cnt].lock);
        msleep(10,0);
    }
    pthread_exit(name);
}
void *kbc_subroutine(void *name)//kbc子程序
{
    printf ("KBC START\n");
    kbc_init();
    int list_cnt=0;
    unsigned char var=0;
    if((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS)
    {
        *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
    }
    msleep(Ms_sleep,Us_sleep);
    while(list_cnt<list_size&&strcmp(subroutine_list[list_cnt].name,name))list_cnt++;
    srand(time(NULL));
    while (1)
    {
        var=rand()%256;
        
        if(data_send_check(list_cnt, TO_IO, var))subroutine_list[list_cnt].succeed++;
        else{ subroutine_list[list_cnt].error++;printf("error %8ld of %s\n",subroutine_list[list_cnt].error,subroutine_list[list_cnt].name);}
        msleep(10,0);
    }
}
void *sm_subroutine(void *name)//共享内存子程序
{
    printf("SM TEST START\n");
    //配置共享内存
    *(volatile unsigned long *)((OFFSET_DEF&0x2000)+map10)=0xffff0040;
    unsigned long addr=0x12000000;
    //printf("please scan test addr hex\n");
    //scanf("%lx",&addr);
    unsigned long base_addrs    =   addr & BASE_DEF;//设置基地址
    unsigned long offset_addr   =   addr & OFFSET_DEF;//偏移地址
    unsigned int sm_size=0x1000;
    const unsigned int size = sm_size;
    printf("base addrs = 0x%08lx,offset addrs = 0x%08lx\n,size = %d\n",base_addrs,offset_addr,size);
    srand(time(NULL));
    unsigned char val=0;//写入值
    unsigned char value=0;//读出值
    int list_cnt=0;
    while(list_cnt<list_size&&strcmp(subroutine_list[list_cnt].name,name))list_cnt++;
    while(1)
    {
        sm_size=0x1000;
        offset_addr=addr & OFFSET_DEF;
        while (sm_size--)
        {
            val=(unsigned char)rand()%256;
            #if TEST_PRINT
            value = val ;
            printf("addr = 0x%08lx,write 0x%02x,read 0x%02x\n",(base_addrs+offset_addr),val,value);
            #else
            pthread_mutex_lock(&LPC_lock);
            *(volatile unsigned char *)(map12+offset_addr) = val;
            pthread_mutex_unlock(&LPC_lock);
            msleep(Ms_sleep,Us_sleep);
            pthread_mutex_lock(&LPC_lock);
            value = *(volatile unsigned char *)(map12+offset_addr);
            pthread_mutex_unlock(&LPC_lock);
            msleep(Ms_sleep,Us_sleep);
            #endif
            if(value == val)subroutine_list[list_cnt].succeed++;
            else 
            {
                subroutine_list[list_cnt].error++;
                printf("error %8ld of addr = 0x%08lx,write 0x%02x,read 0x%02x\n",subroutine_list[list_cnt].error,(unsigned long)(map12+offset_addr),val,value);
            }
            offset_addr++;
        }
    }
    printf("SM TEST OVER\n");
}
void *rw_subroutine(void *name)//用户读写子程序
{
    printf("RW START\n");
    //配置共享内存
    *(volatile unsigned long *)((OFFSET_DEF&0x2000)+map10)=0xffff0040;
    unsigned char cmd[10]={0};
    unsigned long addr;
    unsigned long val;
    unsigned long byte;
    unsigned long base_addrs;//设置基地址
    unsigned long offset_addr;//偏移地址
    unsigned char * map_addr;
    printf("There are 3 commands, which are:\n read write exit\n");
    while (1)
    {
        printf("please scan test cmd(char)\n");
        scanf("%s",cmd);
        printf("cmd is %s\n",cmd);
        if(!strcmp((const char *)cmd,"exit"))break;
        printf("please scan test addr(hex) val(hex)\n");
        scanf("%lx %lx",&addr,&val);
        printf("please scan test val's  byte dec\n");
        scanf("%ld",&byte);
        base_addrs =   addr & BASE_DEF;
        offset_addr=   addr & OFFSET_DEF;
        printf("base addrs = 0x%08lx,offset addrs = 0x%08lx\n\n",base_addrs,offset_addr);
        #if TEST_PRINT
        map_addr=(unsigned char *)base_addrs;
        #else
        map_addr=(unsigned char *)mmap(NULL,MAP_SIZE,PROT_WRITE|PROT_READ,MAP_SHARED,mem_fd,base_addrs);
        if(map_addr == MAP_FAILED)
        {
            close(mem_fd);
            perror("mmap share memroy fail");
            exit(-1);
        }
        #endif
        if(!strcmp((const char *)cmd,"read"))
        {
            unsigned long cnt=0;
            while (cnt<=val)
            {
                #if TEST_PRINT
                if(byte == 1)printf("addr = 0x%08lx,read 0x%02x\n",(base_addrs+offset_addr),__UINT_LEAST8_MAX__);
                else if(byte == 2)printf("addr = 0x%08lx,read 0x%04x\n",(base_addrs+offset_addr),__UINT_LEAST16_MAX__);
                else if(byte == 4)printf("addr = 0x%08lx,read 0x%08x\n",(base_addrs+offset_addr),__UINT_LEAST32_MAX__);
                //else if(byte == 8)printf("addr = 0x%08lx,read 0x%016x\n",(base_addrs+offset_addr),val);
                #else
                if(byte == 1)printf("addr = 0x%08lx,read 0x%02x\n",(unsigned long)(map_addr+offset_addr),*(volatile unsigned char *)(map_addr+offset_addr));
                else if(byte == 2)printf("addr = 0x%08lx,read 0x%04x\n",(unsigned long)(map_addr+offset_addr),*(volatile unsigned short *)(map_addr+offset_addr));
                else if(byte == 4)printf("addr = 0x%08lx,read 0x%08lx\n",(unsigned long)(map_addr+offset_addr),*(volatile unsigned long *)(map_addr+offset_addr));
                #endif
                offset_addr+=byte;
                cnt++;
            }
            //scanf("\n");
        }
        else if(!strcmp((const char *)cmd,"write"))
        {
            #if TEST_PRINT
            if(byte == 1)printf("addr = 0x%08lx,write 0x%02x,read 0x%02x\n",(base_addrs+offset_addr),(unsigned char)val,(unsigned char)val);
            else if(byte == 2)printf("addr = 0x%08lx,read 0x%04x,read 0x%04x\n",(base_addrs+offset_addr),(unsigned short)val,(unsigned short)val);
            else if(byte == 4)printf("addr = 0x%08lx,read 0x%08lx,read 0x%08lx\n",(base_addrs+offset_addr),(unsigned long)val,(unsigned long)val);
            //else if(byte == 8)printf("addr = 0x%08lx,read 0x%016x\n",(base_addrs+offset_addr),val);
            #else
            if(byte == 1)
            {
                *(volatile unsigned char *)(map_addr+offset_addr)=(unsigned char)val;
                printf("addr = 0x%08lx,write 0x%02x,read 0x%02x\n",(unsigned long)(map_addr+offset_addr),(unsigned char)val,*(volatile unsigned char *)(map_addr+offset_addr));
            }
            else if(byte == 2)
            {
                *(volatile unsigned short *)(map_addr+offset_addr)=(unsigned short)val;
                printf("addr = 0x%08lx,write 0x%04x,read 0x%04x\n",(unsigned long)(map_addr+offset_addr),(unsigned short)val,*(volatile unsigned short *)(map_addr+offset_addr));
            }
            else if(byte == 4)
            {
                *(volatile unsigned long *)(map_addr+offset_addr)=(unsigned long)val;
                printf("addr = 0x%08lx,write 0x%08lx,read 0x%08lx\n",(unsigned long)(map_addr+offset_addr),(unsigned long)val,*(volatile unsigned long *)(map_addr+offset_addr));
            }
            #endif
        }
        else printf("no %s cmd\n",cmd);
        #if !TEST_PRINT
        munmap(map_addr,MAP_SIZE);
        #endif
    }
    
    printf("RW OVER\n");
    pthread_exit(0);
}
void *Space_subroutine(void *name)//ECSPACE子程序
{
    printf("Space Start\n");
    int list_cnt=0;
    int space_cnt=0;
    int space_fd=0;
    unsigned char var=0;
    char space_chars[100];
    pthread_mutex_lock(subroutine_list[list_cnt].lock);
    if((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS)
    {
        *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
    }
    msleep(Ms_sleep,Us_sleep);
    pthread_mutex_unlock(subroutine_list[list_cnt].lock);
    while(list_cnt<list_size&&strcmp(subroutine_list[list_cnt].name,name))list_cnt++;
    srand(time(NULL));
    while (1)
    {
        space_fd=open("./space_log.txt",O_RDWR|O_CREAT|O_TRUNC,0666);
        space_cnt=0;
        pthread_mutex_lock(subroutine_list[list_cnt].lock);
        while (space_cnt<space_number)
        {
            #if 1
            int write_val=rand()%256;
            var = data_send_recv(list_cnt, TO_CMD, WRITE_ECSPACE_CMD);
            if(var != WRITE_ECSPACE_CMD)
            {
                subroutine_list[list_cnt].error++;
                printf("error of ec_space write cmd %d\n",var);
                break;
            }
            var = data_send_recv(list_cnt, TO_IO, ECSpace[space_cnt].addr);
            if(var != ECSpace[space_cnt].addr)
            {
                subroutine_list[list_cnt].error++;
                printf("error of ec_space write addr %d\n",var);
                break;
            }
            ECSpace[space_cnt].val = data_send_recv(list_cnt, TO_IO, write_val);
            if(write_val != ECSpace[space_cnt].val)
            {
                subroutine_list[list_cnt].error++;
                printf("error of ec_space write val %d\n",ECSpace[space_cnt].val);
                break;
            }
            subroutine_list[list_cnt].succeed++;
            #else
            *(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))=READ_ECSPACE_CMD;
            while (!((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS))
            {
                msleep(Ms_sleep,Us_sleep);
            }
            var=*(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
            if(var != READ_ECSPACE_CMD)
            {
                subroutine_list[list_cnt].error++;
                printf("error of ec_space cmd %d\n",var);
                break;
            }
            subroutine_list[list_cnt].succeed++;
            *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO))=ECSpace[space_cnt].addr;
            while (!((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS))
            {
                msleep(Ms_sleep,Us_sleep);
            }
            ECSpace[space_cnt].val=*(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
            #endif
            sprintf(space_chars," %s ( %d ): %d\n",ECSpace[space_cnt].name,ECSpace[space_cnt].addr,ECSpace[space_cnt].val);
            write(space_fd,time_char,strlen(time_char));
            write(space_fd,space_chars,strlen(space_chars));
            space_cnt++;
        }
        pthread_mutex_unlock(subroutine_list[list_cnt].lock);
        close(space_fd);
        sleep(10);
    }
    pthread_exit(0);
}
void *PWM_subroutine(void *name)//PWM子程序
{
    printf("pwm && tach Start\n");
    int list_cnt=0;
    unsigned char var=0;
    unsigned char tachl=0;
    unsigned char tachh=0;
    unsigned short tach=0;
    pthread_mutex_lock(subroutine_list[list_cnt].lock);
    if((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS)
    {
        *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
    }
    msleep(Ms_sleep,Us_sleep);
    pthread_mutex_unlock(subroutine_list[list_cnt].lock);
    while(list_cnt<list_size&&strcmp(subroutine_list[list_cnt].name,name))list_cnt++;
    srand(time(NULL));
    while (1)
    {
        var=rand()%101;
        pthread_mutex_lock(subroutine_list[list_cnt].lock);
        if(!data_send_check(list_cnt, TO_CMD, PWM_MODULE_EN))
        {
            printf("error of PWM_MODULE_EN\n");
            subroutine_list[list_cnt].error++;
            continue;
        }
        if(!strcmp(name,"PWM0"))
        {
            if(data_send_check(list_cnt, TO_IO, 0x00));
            else {subroutine_list[list_cnt].error++;continue;}
        }
        else if(!strcmp(name,"PWM1"))
        {
            if(data_send_check(list_cnt, TO_IO, 0x01));
            else {subroutine_list[list_cnt].error++;continue;}
        }
        else if(!strcmp(name,"PWM2"))
        {
            if(data_send_check(list_cnt, TO_IO, 0x02));
            else {subroutine_list[list_cnt].error++;continue;}
        }
        else if(!strcmp(name,"PWM3"))
        {
            if(data_send_check(list_cnt, TO_IO, 0x03));
            else {subroutine_list[list_cnt].error++;continue;}
        }
        else if(!strcmp(name,"PWM4"))
        {
            if(data_send_check(list_cnt, TO_IO, 0x04));
            else {subroutine_list[list_cnt].error++;continue;}
        }
        tachl = data_send_recv(list_cnt, TO_IO, var);
        while (!((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS))
        {
            msleep(Ms_sleep,Us_sleep);
        }
        tachh=*(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
        pthread_mutex_unlock(subroutine_list[list_cnt].lock);
        tach=(((u16)tachh)<<8)|((u16)tachl);
        printf("%s drcr = %d,tach = %d",(char *)name,var,tach);
        subroutine_list[list_cnt].succeed++;
        sleep(1);
    }
    printf("pwm && tach exit\n");
    pthread_exit(0);
}
void *GPIO_subroutine(void *name)//GPIO子程序
{
    printf("GPIO Start\n");
    int list_cnt=0;
    unsigned char var0, var1, var2, var3;
    unsigned char lev=0;
    unsigned char i = 0;
    unsigned char error_flag = 0;
    unsigned char error_times = 0;
    pthread_mutex_lock(subroutine_list[list_cnt].lock);
    if((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS)
    {
        *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
    }
    msleep(Ms_sleep,Us_sleep);
    pthread_mutex_unlock(subroutine_list[list_cnt].lock);
    while(list_cnt<list_size&&strcmp(subroutine_list[list_cnt].name,name))list_cnt++;
    srand(time(NULL));
    while (1)
    {
        error_flag = 0;
        pthread_mutex_lock(subroutine_list[list_cnt].lock);
        
        if(!data_send_check(list_cnt, TO_CMD, GPIO_INIT))
        {
            printf("error of %s_INIT\n",subroutine_list[list_cnt].name);
            subroutine_list[list_cnt].error++;
            continue;
        }
        //确定输入/输出
        var0 = data_send_recv(list_cnt, TO_IO, lev);
        var1 = data_send_recv(list_cnt, TO_IO, lev);
        var2 = data_send_recv(list_cnt, TO_IO, lev);
        var3 = data_send_recv(list_cnt, TO_IO, lev);
        pthread_mutex_unlock(subroutine_list[list_cnt].lock);
        //检验结果
        if(var0 == 0x0) subroutine_list[list_cnt].succeed += 8;
        else 
        {
            error_flag++;
            error_times = 0; i = 0;
            printf("\n%s",(char *)name);
			while(var0 != 0)
			{
				if(var0 & 0x1){error_times++; printf(" %d", i);}
                var0 >>= 1;
				i++;
			}
            subroutine_list[list_cnt].succeed += 8-error_times;
            subroutine_list[list_cnt].error += error_times;
        }
        if(var1 == 0x0) subroutine_list[list_cnt].succeed += 8;
        else 
        {
            error_times = 0; i = 8;
            if(!error_flag)
            {
                error_flag++;
                printf("\n%s",(char *)name);
            }
			while(var1 != 0)
			{
				if(var1 & 0x1){error_times++; printf(" %d", i);}
                var1 >>= 1;
				i++;
			}
            subroutine_list[list_cnt].succeed += 8-error_times;
            subroutine_list[list_cnt].error += error_times;
        }
        if(var2 == 0x0) subroutine_list[list_cnt].succeed += 8;
        else 
        {
            error_times = 0; i = 16;
            if(!error_flag)
            {
                error_flag++;
                printf("\n%s",(char *)name);
            }
			while(var2 != 0)
			{
				if(var2 & 0x1){error_times++; printf(" %d", i);}
                var2 >>= 1;
				i++;
			}
            subroutine_list[list_cnt].succeed += 8-error_times;
            subroutine_list[list_cnt].error += error_times;
        }
        if(var3 == 0x0) subroutine_list[list_cnt].succeed += 8;
        else 
        {
            error_times = 0; i = 24;
            if(!error_flag)
            {
                error_flag++;
                printf("\n%s",(char *)name);
            }
			while(var3 != 0)
			{
				if(var3 & 0x1){error_times++; printf(" %d", i);}
                var3 >>= 1;
				i++;
			}
            if(error_flag) printf(" error.");
            subroutine_list[list_cnt].succeed += 8-error_times;
            subroutine_list[list_cnt].error += error_times;
        }
        lev=!lev;
        msleep(20,0); 
    }
    printf("GPIO exit\n");
    pthread_exit(0);
}
void *I2C_subroutine(void *name)//I2C子程序
{
    printf("I2C Start\n");
    int list_cnt=0;
    unsigned char var1=0;
    unsigned char var2=0;
    pthread_mutex_lock(subroutine_list[list_cnt].lock);
    if((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS)
    {
        *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
    }
    msleep(Ms_sleep,Us_sleep);
    pthread_mutex_unlock(subroutine_list[list_cnt].lock);
    while(list_cnt<list_size&&strcmp(subroutine_list[list_cnt].name,name))list_cnt++;
    srand(time(NULL));
    while (1)
    {
        pthread_mutex_lock(subroutine_list[list_cnt].lock);
        
        if(!data_send_check(list_cnt, TO_CMD, I2C_MODULE_EN))
        {
            printf("error of %s_MODULE_EN\n", (char *)name);
            subroutine_list[list_cnt].error++;
            continue;
        }
        while (!((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS))
        {
            msleep(Ms_sleep,Us_sleep);
        }
        var1=*(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
        msleep(Ms_sleep,Us_sleep);
        while (!((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS))
        {
            msleep(Ms_sleep,Us_sleep);
        }
        var2=*(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
        pthread_mutex_unlock(subroutine_list[list_cnt].lock);
        if(var1!=0x00 && var1!=0xff) {printf("%s temp = %d.%d\n", (char *)name, var1, ((var2>>5)&0x7)*125);subroutine_list[list_cnt].succeed++;}
        else {printf("%s error.", (char *)name);subroutine_list[list_cnt].error++;}
        msleep(10,0);
    }
    printf("I2C exit\n");
    pthread_exit(0);
}
void *UART_subroutine(void* name)//UART子程序
{
    printf("%s Start\n",name);
    int list_cnt=0;
    unsigned char var=0;
    unsigned char temp=0;
    pthread_mutex_lock(subroutine_list[list_cnt].lock);
    if((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS)
    {
        *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
    }
    msleep(Ms_sleep,Us_sleep);
    pthread_mutex_unlock(subroutine_list[list_cnt].lock);
    while(list_cnt<list_size&&strcmp(subroutine_list[list_cnt].name,name))list_cnt++;
    srand(time(NULL));
    while (1)
    {
        var = rand() % 128;
        pthread_mutex_lock(subroutine_list[list_cnt].lock);
        if(!data_send_check(list_cnt, TO_CMD, UART_MODULE_EN))
        {
            printf("error of %s_MODULE_EN\n",subroutine_list[list_cnt].name);
            subroutine_list[list_cnt].error++;
            continue;
        }
        temp = data_send_recv(list_cnt, TO_IO, var);
        if(var == temp) subroutine_list[list_cnt].succeed++;
        else if(temp == 0x80){printf(" %s timeout.", subroutine_list[list_cnt].name);subroutine_list[list_cnt].error++;}
        else{printf(" %s error.", subroutine_list[list_cnt].name);subroutine_list[list_cnt].error++;}
        pthread_mutex_unlock(subroutine_list[list_cnt].lock);
        msleep(10,0);
    }
    printf("UART exit\n");
    pthread_exit(0);
}
void *CAN_subroutine(void* name)//CAN子程序
{
    printf("%s Start\n",name);
    int list_cnt=0;
    unsigned char var=0;
    unsigned char val=8;    //一次接受数据量1～8
    pthread_mutex_lock(subroutine_list[list_cnt].lock);
    if((*(volatile u8 *)(map18+(subroutine_list[list_cnt].SR_CMD))) & OBFS)
    {
        *(volatile u8 *)(map18+(subroutine_list[list_cnt].IO));
    }
    msleep(Ms_sleep,Us_sleep);
    pthread_mutex_unlock(subroutine_list[list_cnt].lock);
    while(list_cnt<list_size&&strcmp(subroutine_list[list_cnt].name,name))list_cnt++;
    srand(time(NULL));
    while (1)
    {
        pthread_mutex_lock(subroutine_list[list_cnt].lock);
        if(!data_send_check(list_cnt, TO_CMD, CAN_MODULE_EN))
        {
            printf("error of %s_MODULE_EN\n",subroutine_list[list_cnt].name);
            subroutine_list[list_cnt].error++;
            continue;
        }
        var = data_send_recv(list_cnt, TO_IO, val);
        pthread_mutex_unlock(subroutine_list[list_cnt].lock);
        
        if(var == (2 * val))subroutine_list[list_cnt].succeed += var;
        else
        {
            subroutine_list[list_cnt].succeed += var;
            subroutine_list[list_cnt].error += (2*val - var);
            printf(" %s has %d error(s).", subroutine_list[list_cnt].name, 2*val - var);
        }
        msleep(10,0);
    }
    printf("CAN exit\n");
    pthread_exit(0);
}
