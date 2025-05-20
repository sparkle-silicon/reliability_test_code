// #include "KERNEL_DMA.H"
// void DMA_INIT()
// {
// //DAM_I3C测试
// uint32_t rdata1,wdata1;
// uint32_t rd[512],wd[512];
// uint32_t addr,tmp;

// //dma config
// uint8_t dst_tr_width,src_tr_width,dst_msize,src_msize,tt_fc,ch_prior,dest_per,src_per;
// uint8_t dinc,sinc;
// uint8_t	int_en,dst_scatter_en,src_gather_en,llp_dst_en,llp_src_en,done,ch_susp,hs_sel_src,hs_sel_dst;
// uint16_t blk_ts;

// //i3c config
// uint32_t  rdata,wdata;
// uint8_t cmd_attr,byte_strb;
// uint8_t tid;
// uint8_t cp,roc,sdap,rnw,toc;
// //1.prepare data
// for (int i = 0; i < 4; i++)
// {
// 	REG32(0x28000+i*4) = i+1;
// }
// //2.iomux config
// sysctl_iomux_config(GPIOC,11,3);//i3c_scl0 master0
// sysctl_iomux_config(GPIOC,12,3);//i3c_sda0

// sysctl_iomux_config(GPIOC,13,3);//i3c_scl1 master1
// sysctl_iomux_config(GPIOB,1,3);//i3c_sda1

// I3C_Init(0x4c);
// // 读
// cmd_attr=0;
// tid=0x8;
// rnw =1;
// toc =1;
// wdata = 0;
// wdata =toc<<30 | rnw<<28 |tid<<3;
// COMMAND_QUEUE_PORT = wdata;

// ////////////////////////////
// //CTLx
// //////////////////////////////                                       
// 	int_en=0;
// 	dst_tr_width=0;//0:8bit 目标地址传输宽度
// 	src_tr_width=0;//2:32bit 源地址传输宽度
// #if DMA_EN
// 	dinc=0;        //0：目标地址增加 1：目标地址减少 2: 目标地址no change
// 	sinc=2;        //0：源地址增加 1：源地址减少 2：源地址no change
// #else
// 	dinc=2;        //0：目标地址增加 1：目标地址减少 2: 目标地址no change
// 	sinc=0;        //0：源地址增加 1：源地址减少 2：源地址no change
// #endif
// 	dst_msize=0;
// 	src_msize=0;
// 	dst_scatter_en=0;
// 	src_gather_en=0;
// #if DMA_EN
// 	tt_fc=2;       //1：内存到外设  2：外设到内存
// #else
// 	tt_fc=1;       //1：内存到外设  2：外设到内存
// #endif
// 	llp_dst_en=0;
// 	llp_src_en=0;
// 	wdata1=llp_src_en<<28|llp_dst_en<<27|tt_fc<<20|dst_scatter_en<<18|src_gather_en<<17|src_msize<<14|dst_msize<<11|sinc<<9|dinc<<7|src_tr_width<<4|dst_tr_width<<1|int_en;
// 	DMA_CTL0_low = wdata1;  //配置CTL0的低32位[0:31]    0x10 0120 
// 	done=1;
// 	blk_ts=1;
// 	wdata1=done<<12|blk_ts;
// 	rdata1 = DMA_CTL0_high;           // 把BLOCK_TS[32:35]清零
// 	rdata1 &= ~(0xf);   
// 	DMA_CTL0_high = rdata1 | wdata1;   //把0x1004写入高32位[32:63]，那现在实际block_ts大小为4
// //////////////////////////////	
// //CFG
// //////////////////////////////
// 	ch_prior=0;
// 	ch_susp=0;
// 	hs_sel_src=0;//hw handshake
// 	hs_sel_dst=0;
// 	wdata1 = (hs_sel_src << 11) | (hs_sel_dst << 10) | (ch_susp << 8) | (ch_prior << 5);
// 	rdata1 = DMA_CFG0_low;
// 	rdata1 &= ~((0x1 << 11) | (0x1 << 10));      //源和目标设备都设置为硬件握手
// 	DMA_CFG0_low = (rdata1 | wdata1);
// #if DMA_EN	
// 	src_per=0;//i3c0_rx 
// 	REG32(0x30500) = 0x1;
// 	rdata1 = DMA_CFG0_high;
// 	DMA_CFG0_high = (rdata1 | (src_per << 7));
// 	DMA_SAR0 = &TX_DATA_PORT; //源地址
// 	DMA_DAR0 = 0x28000;  //目标地址
// #else
// 	dest_per = 1;
// 	rdata1 = DMA_CFG0_high;
// 	DMA_CFG0_high = (rdata1 | (dest_per << 11));
// 	DMA_SAR0 = 0x28000; //源地址
// 	DMA_DAR0 = &TX_DATA_PORT;  //目标地址
// #endif
// 	DMA_DmaCfgReg = 0x1;//DMA使能
// 	wdata1 = (0x1 | (0x1<<8));
// 	DMA_ChEnReg  = wdata1;
// 	vDelayXms(10);
// 	printf("iram0 data 0x28000:%x\n",REG32(0x28000));
// }

// // //DMA_SPIM测试
// 	uint32_t rdata1,wdata1;
// 	uint32_t rd[512],wd[512];
// 	uint32_t addr,tmp;
// 	uint32_t test_data[32];
// 	uint8_t dst_tr_width,src_tr_width,dst_msize,src_msize,tt_fc,ch_prior,dest_per,src_per;    //分别为目的地传输宽度、源传输宽度、目的地传输大小、源传输大小、总线传输功能 和 通道优先级
// 	uint8_t dinc,sinc;   //分别为 目的地地址增量 和 源地址增量
// 	uint8_t int_en,dst_scatter_en,src_gather_en,llp_dst_en,llp_src_en,done,ch_susp,hs_sel_src,hs_sel_dst;  //控制 DMA 操作的各种功能或状态   //int_en (中断使能),dst_scatter_en (目标散布使能),src_gather_en (源聚集使能),llp_dst_en (目标链表使能),llp_src_en (源链表使能),done (DMA 完成标志),ch_susp (通道挂起),hs_sel_src (源硬件握手选择),hs_sel_dst (目标硬件握手选择)
// 	uint32_t blk_ts;    //定义 DMA 传输的块大小，通常是指每次传输的数据量，单位为字节
// 	uint8_t s_data[32];
// 	uint8_t d_data[32];
// //1.准备发送数据
// for (int i = 0; i < 4; i++)
// {
// 	test_data[0] = 0xa0440155;
// 	test_data[1] = 0xa1450256;
// 	test_data[2] = 0xa2460357;
// 	test_data[3] = 0xa3470458;
// 	// test_data[0] = 0xa044;
// 	// test_data[1] = 0xa145;
// 	// test_data[2] = 0xa246;
// 	// test_data[3] = 0xa347;
// 	// test_data[i] = 0x56a0+i; 
// 	REG32(0x28000+i*4) = test_data[i];
// 	printf("before iram0 data:[%x]:%x\n",&REG32(0x28000+i*4),REG32(0x28000+i*4));
// }

// for (int i = 0; i < 8; i++)
// {
// 	s_data[i] = 0xc0+i;
// }

// //2.配置引脚复用，初始化spim
// 	sysctl_iomux_spim_cs();
// 	sysctl_iomux_spim();
// 	SPI_Init(0, SPIM_CPOL_LOW, SPIM_CPHA_FE, SPIM_MSB, 0x7, 1);
// 	// sectorerase(0x8000,0);
// 	// spibyteprogram(0x8000,s_data,4,0);
// //////////////////////////////
// //CTLx
// //////////////////////////////
// 	int_en=0;
// 	dst_tr_width=0;//0:8bit
// 	src_tr_width=0;//2:32bit
// 	dinc=0;        //2: no change
// 	sinc=2;        
// 	dst_msize=0;
// 	src_msize=0;
// 	dst_scatter_en=0;
// 	src_gather_en=0;
// 	tt_fc=2;       //1: mem to peri
// 	llp_dst_en=0;
// 	llp_src_en=0;
// 	wdata1=llp_src_en<<28|llp_dst_en<<27|tt_fc<<20|dst_scatter_en<<18|src_gather_en<<17|src_msize<<14|dst_msize<<11|sinc<<9|dinc<<7|src_tr_width<<4|dst_tr_width<<1|int_en;
// 	DMA_CTL0_low = wdata1;  //配置CTL0的低32位[0:31]    0x10 0120 
// 	done=1;
// 	blk_ts=4;
// 	wdata1=done<<12|blk_ts;
// 	rdata1 = DMA_CTL0_high;           // 把BLOCK_TS[32:35]清零
// 	rdata1 &= ~(0xf);   
// 	DMA_CTL0_high = rdata1 | wdata1;   //把0x1004写入高32位[32:63]，那现在实际block_ts大小为4
// //CFG
// //////////////////////////////
// 	ch_prior=0;
// 	ch_susp=0;
// 	hs_sel_src=0;//hw handshake
// 	hs_sel_dst=0;

// 	wdata1 = (hs_sel_src << 11) | (hs_sel_dst << 10) | (ch_susp << 8) | (ch_prior << 5);
// 	rdata1 = DMA_CFG0_low;
// 	rdata1 &= ~((0x1 << 11) | (0x1 << 10));      //源和目标设备都设置为硬件握手
// 	DMA_CFG0_low = (rdata1 | wdata1);

// 	// dest_per = 9;
// 	src_per = 8;        
// 	rdata1 = DMA_CFG0_high;
// 	// DMA_CFG0_high = (rdata1 | (dest_per << 11));
// 	DMA_CFG0_high = (rdata1 | (src_per << 7));
// 	// DMA_SAR0 = 0x28000; //源地址
// 	DMA_SAR0 = &SPIM_DA; //源地址
// 	// DMA_DAR0 = &SPIM_DA;  //目标地址
// 	DMA_DAR0 = 0x28000;  //目标地址
// 	DMA_DmaCfgReg = 0x1;//DMA使能
// 	// wdata1 = (0x1 | (0x1<<8));
// 	// DMA_ChEnReg  = wdata1;
// 	// spiread(0x7000,d_data,8,0);

// 	SPI_Flash_CS_High(0); //SPI片选高
// 	// vDelayXms(1);
// 	spiread(0x8000,d_data,4,0);
// 	// printf("DMA_ChEnReg:%x\n",DMA_ChEnReg);
// 	// vDelayXms(10);
// 	for (int i = 0; i < 8; i++)
// 	{
// 		if (test_data[i]!=d_data[i])
// 		{
// 			printf("error!!\n");
// 		}
// 		else
// 		{}
// 		printf("test_data:%x,d_data:%x,iram0 data:%x\n",test_data[i],d_data[i],REG32(0x28000+i*4));
// 	}


// //DAM_I3C测试

// uint32_t rdata1,wdata1;
// uint32_t rd[512],wd[512];
// uint32_t addr,tmp;

// //dma config
// uint8_t dst_tr_width,src_tr_width,dst_msize,src_msize,tt_fc,ch_prior,dest_per,src_per;
// uint8_t dinc,sinc;
// uint8_t	int_en,dst_scatter_en,src_gather_en,llp_dst_en,llp_src_en,done,ch_susp,hs_sel_src,hs_sel_dst;
// uint16_t blk_ts;

// //i3c config
// uint32_t  rdata,wdata;
// uint8_t cmd_attr,byte_strb;
// uint8_t tid;
// // uint8_t cmd,data_b0,data_b1,data_b2;
// uint8_t cp,roc,sdap,rnw,toc;
// // uint8_t dev_indx,speed;
// // uint8_t dev_count;
// // uint16_t dl;


// //1.prepare data

// for (int i = 0; i < 4; i++)
// {
// 	REG32(0x28000+i*4) = i+1;
// }

// //2.iomux config
// sysctl_iomux_config(GPIOC,11,3);//i3c_scl0 master0
// sysctl_iomux_config(GPIOC,12,3);//i3c_sda0

// sysctl_iomux_config(GPIOC,13,3);//i3c_scl1 master1
// sysctl_iomux_config(GPIOB,1,3);//i3c_sda1



// //i3c reset
// // SYSCTL_RST0 |= (1<<6);//i3c reset
// // SYSCTL_RST0 = 0;
// // SYSCTL_RST0 |= (1<<4);//i3c clk enable
// // SYSCTL_RST0 = 0;

// // DEVICE_CTRL=(0x1<<31)|(0x1<<28)|0x1;//i3c enable

// I3C_Init(0x4c);


// // 读
// cmd_attr=0;
// tid=0x8;
// rnw =1;
// toc =1;

// wdata = 0;
// wdata =toc<<30 | rnw<<28 |tid<<3;
// // printf("wdata:%x\n",wdata);
// COMMAND_QUEUE_PORT = wdata;

// rdata = RX_DATA_PORT;
// for (int  i = 0; i < 4; i++)
// {
// 	printf("rdata:%x\n",rdata);
// }

// ////////////////////////////
// //CTLx
// //////////////////////////////                                       
// 	int_en=0;
// 	dst_tr_width=0;//0:8bit 目标地址传输宽度
// 	src_tr_width=0;//2:32bit 源地址传输宽度

// 	dinc=2;        //0：目标地址增加 1：目标地址减少 2: 目标地址no change
// 	sinc=0;        //0：源地址增加 1：源地址减少 2：源地址no change
	
// 	dst_msize=0;
// 	src_msize=0;
// 	dst_scatter_en=0;
// 	src_gather_en=0;
// 	tt_fc=1;       //1：内存到外设  2：外设到内存
// 	llp_dst_en=0;
// 	llp_src_en=0;
// 	wdata1=llp_src_en<<28|llp_dst_en<<27|tt_fc<<20|dst_scatter_en<<18|src_gather_en<<17|src_msize<<14|dst_msize<<11|sinc<<9|dinc<<7|src_tr_width<<4|dst_tr_width<<1|int_en;
// 	DMA_CTL0_low = wdata1;  //配置CTL0的低32位[0:31]    0x10 0120 
// 	done=1;
// 	blk_ts=1;
// 	wdata1=done<<12|blk_ts;
// 	rdata1 = DMA_CTL0_high;           // 把BLOCK_TS[32:35]清零
// 	rdata1 &= ~(0xf);   
// 	DMA_CTL0_high = rdata1 | wdata1;   //把0x1004写入高32位[32:63]，那现在实际block_ts大小为4
// //////////////////////////////	
// //CFG
// //////////////////////////////
// 	ch_prior=0;
// 	ch_susp=0;
// 	hs_sel_src=0;//hw handshake
// 	hs_sel_dst=0;

// 	wdata1 = (hs_sel_src << 11) | (hs_sel_dst << 10) | (ch_susp << 8) | (ch_prior << 5);
// 	rdata1 = DMA_CFG0_low;
// 	rdata1 &= ~((0x1 << 11) | (0x1 << 10));      //源和目标设备都设置为硬件握手
// 	DMA_CFG0_low = (rdata1 | wdata1);
// 	dest_per = 1;
// 	// src_per=0;//i3c0_rx        
// 	rdata1 = DMA_CFG0_high;
// 	// REG32(0x30500) = 0x1;
// 	DMA_CFG0_high = (rdata1 | (dest_per << 11));
// 	// DMA_CFG0_high = (rdata1 | (src_per << 7));
// 	DMA_SAR0 = 0x28000; //源地址
// 	// printf("DMA_SAR0:%x\n",DMA_SAR0);

// 	// DMA_SAR0 = &TX_DATA_PORT; //源地址
// 	DMA_DAR0 = &TX_DATA_PORT;  //目标地址
// 	// DMA_DAR0 = 0x28000;  //目标地址
// 	DMA_DmaCfgReg = 0x1;//DMA使能
// 	wdata1 = (0x1 | (0x1<<8));
// 	DMA_ChEnReg  = wdata1;
// 	vDelayXms(10);
// 	printf("iram0 data 0x28000:%x\n",REG32(0x28000));
