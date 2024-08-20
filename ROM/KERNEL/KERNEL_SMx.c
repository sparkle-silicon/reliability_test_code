/*
 * @Author: Iversu
 * @LastEditors: daweslinyu daowes.ly@qq.com
 * @LastEditTime: 2024-08-19 18:52:28
 * @Description: This is about the  national crypto algorithm implementation
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
#if 1
#include <KERNEL_SMx.H>
#if (!GLE01)
#ifdef AE103
//SM2 elipse
#if 1
const char *SM2_ID = "L-silicon";
const char *SM2_DATA = "SPK32AE103";
const DWORD SM2_Elipse_P[8] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0x00000000,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE };
const DWORD SM2_Elipse_A[8] = {
    0xFFFFFFFC,
    0xFFFFFFFF,
    0x00000000,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE };
const DWORD SM2_Elipse_B[8] = {
    0x4D940E93,
    0xDDBCBD41,
    0x15AB8F92,
    0xF39789F5,
    0xCF6509A7,
    0x4D5A9E4B,
    0x9D9F5E34,
    0x28E9FA9E };
const DWORD SM2_Elipse_N[8] = {
    0x39D54123,
    0x53BBF409,
    0x21C6052B,
    0x7203DF6B,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE };
const spoint SM2_Elipse_G = {
    .x[0] = 0x334C74C7,
    .x[1] = 0x715A4589,
    .x[2] = 0xF2660BE1,
    .x[3] = 0x8FE30BBF,
    .x[4] = 0x6A39C994,
    .x[5] = 0x5F990446,
    .x[6] = 0x1F198119,
    .x[7] = 0x32C4AE2C,
    .y[0] = 0x2139F0A0,
    .y[1] = 0x02DF32E5,
    .y[2] = 0xC62A4740,
    .y[3] = 0xD0A9877C,
    .y[4] = 0x6B692153,
    .y[5] = 0x59BDCEE3,
    .y[6] = 0xF4F6779C,
    .y[7] = 0xBC3736A2 };
#endif
//SM4
static DWORD SMx_Change(DWORD data)
{
    return (((data & 0x000000ff) << 24) | ((data & 0x0000ff00) << 8) | ((data & 0x00ff0000) >> 8) | ((data & 0xff000000) >> 24));
}
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SM4
 * note :
 *          mode:0 ECB加密,1 ECB解码/2 CBC加密/3 CBC解码
 *          input: Plaintext password
 *          key:Extended secret key
 *          output: Ciphertext storage
 *          xordr :lv
 * ************************************************************************************/
/* void SM4_Run(BYTE mode,BYTEP input,BYTEP key,BYTEP output,BYTEP xordr)
{
        SM4_Key_Set((DWORDP)key);//set key
        SM4_Data_IN((DWORDP)input);//input data
        SM4_Mode_Set(mode);
        if(mode == 2 || mode == 3)
        {
            SM4_XORDR_IN((DWORDP)xordr);//Functionality not implemented
        }
        SM4_Data_Out((DWORDP)output);
} */
// void SM4_Run(DWORD bit, BYTE mode, BYTEP input, BYTEP key, BYTEP output, BYTEP xordr)
// {
//     if(bit % 128)
//     {
//         printf("SM4 data bit size ERROR\n");
//         return;
//     }
//     SM4_Key_Set((DWORDP)key);
//     SM4_Mode_Set(mode);
//     if(mode == SM4_CBC_ENCODE || mode == SM4_CBC_DECODE)
//     {
//         SM4_XORDR_IN((DWORDP)xordr);    //set xordr
//     }
//     for(int i = 0; i * 128 < bit; i++)
//     {
//         SM4_Data_IN((DWORDP)(input + i * 16));   //input data
//         SM4_Data_Out((DWORDP)(output + i * 16)); //output data
//     }
// }
//SM3
static BYTE data_offset = 0x0;
static void SM3_Auto_Write(DWORD data)
{
    SM3_REG(SM3_DATA0_OFFSET + data_offset) = data;
    data_offset = (data_offset + 0x4) % 0x40;
    while(!(data_offset || (SM3_INT & 0x1))) {}
    if(SM3_INT & 0x1)SM3_INT = 0x1;
}
static void SM3_Data_In(DWORD bit, DWORDP input)
{
    DWORD dword = bit / DEC32;//double word size
    DWORD rbyte = (bit / HEX08) % HEX04;//Remainder byte
    DWORD rbit = bit % DEC32 - rbyte * HEX08;//Remainder bit
    DWORD block = (bit % DEC512) / DEC32;//final block byte size
    uDword buffer;
    DWORD i = 0, j = 0;
    for(i = 0; i < dword; i++)
    {
        buffer.dword = (*(input + i));
        SM3_Auto_Write(buffer.dword);
        //SM3_IN=buffer.dword;
    }
    for(j = 0; j < rbyte; j++)
    {
        buffer.byte[j] = *((BYTEP)input + i * 8 + j);
    }
    rbit %= HEX08;
    if(rbit)buffer.byte[j] = *((BYTEP)input + i * 8 + j);
    else buffer.byte[j] = 0x0;
    for(i = 0; i < rbit; i++)
    {
        buffer.byte[j] &= (~(0x80 >> i));
    }
    if(rbit || rbyte)
    {
        buffer.byte[j] |= (0x80 >> i);
        SM3_Auto_Write(buffer.dword);
        block++;
    }
    else
    {
        buffer.dword = 0x80;
        SM3_Auto_Write(buffer.dword);
        block++;
    }
    if(block >= 16)block %= 16;
    while(block < 15)
    {
        buffer.dword = 0;
        SM3_Auto_Write(buffer.dword);
        block++;
    }
    buffer.dword = SMx_Change(bit);
    SM3_Auto_Write(buffer.dword);
    data_offset = 0x0;
}
static void SM3_Data_Out(DWORDP output)
{
    *(output + 0) = SMx_Change(SM3_OUT7);
    *(output + 1) = SMx_Change(SM3_OUT6);
    *(output + 2) = SMx_Change(SM3_OUT5);
    *(output + 3) = SMx_Change(SM3_OUT4);
    *(output + 4) = SMx_Change(SM3_OUT3);
    *(output + 5) = SMx_Change(SM3_OUT2);
    *(output + 6) = SMx_Change(SM3_OUT1);
    *(output + 7) = SMx_Change(SM3_OUT0);
}
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SM3
 * note :
 *          bit:data bit size
 *          input: Plaintext password
 *          output: Ciphertext storage
 * ************************************************************************************/
void SM3_Run(DWORD bit, BYTEP input, BYTEP output)
{
    SM3_CONIFG |= SM3_CONFIG_ENABLE;//使能sm3   
    SM3_CONIFG &= ~SM3_CONFIG_NEW_DATA;//data in
    SM3_Data_In(bit, (DWORDP)input);//data in
    SM3_CONIFG |= SM3_CONFIG_ENABLE | SM3_CONFIG_NEW_DATA;//config run
    while(!(SM3_STATE & SM3_STATE_FINISH)) {}
    SM3_Data_Out((DWORDP)output);
}
//SM2
static void SM2_IO(VDWORDP addr1, VDWORDP addr2)
{
    VBYTE i = 0;
    while(i < 8) { *(addr1 + i) = *(addr2 + i); i++; }
}
static void SM2_Data_IN(DWORDP X1, DWORDP X2, DWORDP Y1, DWORDP Y2, DWORDP K)
{
    if(X1 != NULL)SM2_IO(&SM2_P1X0, (VDWORDP)X1);
    if(X2 != NULL)SM2_IO(&SM2_P2X0, (VDWORDP)X2);
    if(Y1 != NULL)SM2_IO(&SM2_P1Y0, (VDWORDP)Y1);
    if(Y2 != NULL)SM2_IO(&SM2_P2Y0, (VDWORDP)Y2);
    if(K != NULL)SM2_IO(&SM2_K0, (VDWORDP)K);
}
static void SM2_Data_Out(DWORDP X3, DWORDP Y3)
{
    if(X3 != NULL)SM2_IO((VDWORDP)X3, &SM2_P3X0);
    if(Y3 != NULL)SM2_IO((VDWORDP)Y3, &SM2_P3Y0);
}
static void SM2_ALU(DWORDP X1, DWORDP X2, DWORDP Y1, DWORDP Y2, DWORDP K, DWORD control, DWORDP X3, DWORDP Y3)
{
    SM2_Data_IN(X1, X2, Y1, Y2, K);//input
    SM2_CONTROL = SM2_CONTROL_EN | control;//run
    while(SM2_STATUS != SM2_STATUS_DONE)//wait
    {
        if(SM2_STATUS == SM2_STATUS_ERROR)
        {
            SM2_Error();
        }
    }
    SM2_Data_Out(X3, Y3);//out
    SM2_CONTROL = SM2_CONTROL_FREE;//free
}
void SM2_N_ADD(DWORDP input1, DWORDP input2, DWORDP output)
{
    SM2_ALU(input1, input2, NULL, NULL, NULL, SM2_CONTROL_MODN_ADD, NULL, output);
}
void SM2_DOT_ADD(spoint input1, spoint input2, spoint *output)
{
    SM2_ALU(input1.x, input2.x, input1.y, input2.y, NULL, SM2_CONTROL_DOT_ADD, output->x, output->y);
}
void SM2_DOT_DBL(spoint input, spoint *output)
{
    SM2_ALU(input.x, NULL, input.y, NULL, NULL, SM2_CONTROL_DOT_DBL, output->x, output->y);
}
void SM2_DOT_MUL(spoint input, DWORDP k, spoint *output)
{
    SM2_ALU(input.x, NULL, input.y, NULL, k, SM2_CONTROL_DOT_MUL, output->x, output->y);
}
#if 0
void SM2_SIGN(spoint g, DWORDP k, DWORDP e, DWORDP key, ssign ouutput)
{
    BYTE i = 0;
    DWORD s[8];
    DWORD s0[8];
    DWORD s1[8];
    spoint P0;
    SM2_DOT_MUL(g, k, &P0);//p0 =[k']G
    SM2_N_ADD(e, P0.x, ouutput.r);//r=(e+p0.x)modn
    for(i = 0; i < 8; i++)
    {
        s0[i] = key[i];
    }
    for(i = 0; i < 8; i++)
    {
        if(s0[i] != 0xFFFFFFFF)
        {
            s0[i]++;
            break;
        }
        else if(i == 7)return;
        else s0[i] = 0x0;
    }
    SM2_ALU(s0, NULL, NULL, NULL, NULL, SM2_CONTROL_MODN_INV, NULL, s0);
    SM2_ALU(s0, s1, NULL, NULL, NULL, SM2_CONTROL_MODN_MUL, NULL, s);//s=(1+dA)-1(k-rdA)modn
}
#endif 
BYTE SM2_VERIFY(DWORDP r, DWORDP s, DWORDP e, spoint g, spoint PA)
{
    VBYTE i = 0;
    DWORD T[8];
    DWORD R[8];
    spoint P0; spoint P1; spoint P2;
    SM2_N_ADD(r, s, T);//t=(r'+s')modn
    SM2_DOT_MUL(g, s, &P0);//p0 =[s']G
    SM2_DOT_MUL(PA, T, &P1);//p1 = [t]PA
    SM2_DOT_ADD(P0, P1, &P2);//P2=P1+P0
    SM2_N_ADD(e, P2.x, R);//R=(e'+P2.x')modn
    for(i = 0; i < 8; i++) { if(r[i] != R[i])return 1;/*printf("r[%d]=%x,R[%d]=%x\n",i,r[i],i,R[i]);*/ }
    return 0;
}
WEAK void SM2_Error(void)
{
    printf("SM2 error of %#x\n", SM2_STATUS);
}
extern spoint SM2Pulib_Key;
extern ssign SM2_signer;
extern DWORD SM2_E[8];
DWORD SM2_K[8] = {
    0xEAC1BC21,
    0x6D54B80D,
    0x3CDBE4CE,
    0xEF3CC1FA,
    0xD9C02DCC,
    0x16680F3A,
    0xD506861A,
    0x59276E27 };
DWORD SM2Private_Key[8] = {
    0x4DF7C5B8,
    0x42FB81EF,
    0x2860B51A,
    0x88939369,
    0xC6D39F95,
    0x3F36E38A,
    0x7B2144B1,
    0x3945208F };//da
#if 0
spoint SM2Pulib_Key = {
   .x[0] = 0x56F35020,
   .x[1] = 0x6BB08FF3,
   .x[2] = 0x1833FC07,
   .x[3] = 0x72179FAD,
   .x[4] = 0x1E4BC5C6,
   .x[5] = 0x50DD7D16,
   .x[6] = 0x1E5421A1,
   .x[7] = 0x09F9DF31,
   .y[0] = 0x2DA9AD13,
   .y[1] = 0x6632F607,
   .y[2] = 0xF35E084A,
   .y[3] = 0x0AED05FB,
   .y[4] = 0x8CC1AA60,
   .y[5] = 0x2DC6EA71,
   .y[6] = 0xE26775A5,
   .y[7] = 0xCCEA490C };
ssign SM2_signer = {
   .r[0] = 0xEEE720B3,
   .r[1] = 0x43AC7EAC,
   .r[2] = 0x27D5B741,
   .r[3] = 0x5944DA38,
   .r[4] = 0xE1BB81A1,
   .r[5] = 0x0EEAC513,
   .r[6] = 0x48D2C463,
   .r[7] = 0xF5A03B06,
   .s[0] = 0x85BBC1AA,
   .s[1] = 0x840B69C4,
   .s[2] = 0x1F7F42D4,
   .s[3] = 0xBB9038FD,
   .s[4] = 0x0D421CA1,
   .s[5] = 0x763182BC,
   .s[6] = 0xDF212FD8,
   .s[7] = 0xB1B6AA29 };
DWORD SM2_E[8] = {
   0xC0D28640,
   0xF4486FDF,
   0x19CE7B31,
   0x17E6AB5A,
   0x534382EB,
   0xACE692ED,
   0xBA45ACCA,
   0xF0B43E94
};
#endif
BYTE SM2_RUN(void)
{
    //SM2_Key_Set(SM2_Elipse_G,SM2_Elipse_K,SM2Private_Key,&SM2Pulib_Key);//生成公钥
    //SM2_SIGN(SM2_Elipse_G,SM2_K,SM2_E,SM2Private_Key,SM2_signer);  
    return SM2_VERIFY(SM2_signer.r, SM2_signer.s, SM2_E, SM2_Elipse_G, SM2Pulib_Key);
}
//TRNG
// u32 *TRNG_SET_SEED(u32 *seed)
// {

//     if((TRNG_STAT & TRNG_STAT_MISSION_MODE) && (!(TRNG_STAT & TRNG_STAT_NONCE_MODE)))
//     {
//         return NULL;
//     }
//     TRNG_SEED0 = *(seed + 0);
//     TRNG_SEED1 = *(seed + 1);
//     TRNG_SEED2 = *(seed + 2);
//     TRNG_SEED3 = *(seed + 3);
//     if((TRNG_STAT & TRNG_STAT_R256))//r256==1
//     {
//         TRNG_SEED4 = *(seed + 4);
//         TRNG_SEED5 = *(seed + 5);
//         TRNG_SEED6 = *(seed + 6);
//         TRNG_SEED7 = *(seed + 7);
//     }
//     return seed;
// }
// u32 *TRNG_GET_RAND(u32 *rand)
// {
//     *(rand + 0) = TRNG_RAND0;
//     *(rand + 1) = TRNG_RAND1;
//     *(rand + 2) = TRNG_RAND2;
//     *(rand + 3) = TRNG_RAND3;

//     if((TRNG_STAT & TRNG_STAT_R256))//r256==1
//     {
//         *(rand + 4) = TRNG_RAND4;
//         *(rand + 5) = TRNG_RAND5;
//         *(rand + 6) = TRNG_RAND6;
//         *(rand + 7) = TRNG_RAND7;

//     }
//     return rand;
// }

// void TRNG_Init(u8 ie, u16 rqsts, u16 age)
// {
//     TRNG_MODE = TRNG_MODE_R256;//1:256bit
//     //TRNG_MODE = ~TRNG_MODE_R256;//0:128bit
//     TRNG_SMODE |= TRNG_SMODE_MISSION_MODE;//任务模式，不开nonce
//     if(ie)
//         TRNG_IE = TRNG_IE_GLBL_EN | (ie & 0b11111);
//     else
//         TRNG_IE = ~(TRNG_IE_GLBL_EN | (0b11111));
//     TRNG_AUTO_RQSTS = rqsts & 0xffff;//自动请求寄存器每产生x*16个随机数更新随机种子，0关闭，否则再重新生成随机种子后作为重载值（数量），rqsts中断
//     TRNG_AUTO_AGE = age & 0xffff;//自动过期计数器x*2^26 个AHB周期（24M约3x sec一次），0关闭，否则再重新生成随机种子后作为重载值（时间），age中断
// }
// u32 *TRNG_RUN(u32 *seed, u32 *rand)
// {
//     // while(TRNG_STAT & TRNG_STAT_SRVC_RQST);//存在未处理的命令
//     if(seed == NULL)
//     {
//         if(((TRNG_STAT >> 16) & 0x7) == 0x7)//未生成随机种子
//         {
//             TRNG_CTRL = 0x2;//auto seed
//         }
//         else if(((TRNG_STAT >> 16) & 0x7) == 0x3)//通过NONCE作为随机种子
//         {
//             TRNG_CTRL = 0x2;//auto seed
//         }
//         else if(((TRNG_STAT >> 16) & 0x7) == 0x4)//自动生成
//         {
//         }
//         else if(((TRNG_STAT >> 16) & 0x7) == 0x0)//通过命令生成
//         {
//         }
//     }
//     else
//     {
//         //再开NONCE或测试模式下执行
//         if((TRNG_STAT & TRNG_STAT_NONCE_MODE) || (!(TRNG_STAT & TRNG_STAT_MISSION_MODE)))
//         {
//             TRNG_SET_SEED(seed);
//             TRNG_CTRL = 0x3;//set seed
//         }
//         else
//         {
//             TRNG_CTRL = 0x2;//auto seed
//         }
//     }
//     while(TRNG_STAT & TRNG_STAT_RAND_RESEEDING);//正在产生随机种子
//     if(!(TRNG_STAT & TRNG_STAT_SEEDED))return NULL;//未生成随机种子，失效
//     TRNG_CTRL = 0x1;//set rand
//     while(TRNG_STAT & TRNG_STAT_RAND_GENERATING);//正在产生随机数
//     TRNG_GET_RAND(rand);
//     return rand;
// }
//eg:
#if 1
BYTE Smx_input[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
BYTE Smx_key[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
BYTE Smx_lv[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
BYTE Smx_output[32];
BYTE Sm3_input[] = { 0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64 };
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SMx test code
 * note :
 * ************************************************************************************/
void SM_Test_main(void)
{
    BYTE i = 0;
    //SM2 debug
    memset(Smx_output, 0, 32);
    i = SM2_RUN();
    if(i)printf("SM2 RUN ERROR\n");
    else printf("SM2 RUN SUCCEED\n");
    //sm3 debug
    printf("SM3 \n");
    memset(Smx_output, 0, 32);
    SM3_Run(512, Sm3_input, Smx_output);
    printf("encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    SM3_Run(128, Sm3_input, Smx_output);
    printf("encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    SM3_Run(32, Sm3_input, Smx_output);
    printf("encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    SM3_Run(24, Sm3_input, Smx_output);
    printf("encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    SM3_Run(16, Sm3_input, Smx_output);
    printf("encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    SM3_Run(8, Sm3_input, Smx_output);
    printf("encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    printf("--------------------------------------------------------------\n");
    //sm4_debug
    printf("SM4 encryption and decryption:\n");
    memset(Smx_output, 0, 32);
    SM4_Run(256, SM4_ECB_ENCODE, Smx_input, Smx_key, Smx_output, NULL);
    printf("ECB encryption:");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    SM4_Run(256, SM4_ECB_DECODE, Smx_output, Smx_key, Smx_output, NULL);
    printf("ECB decryption:");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    SM4_Run(256, SM4_CBC_ENCODE, Smx_input, Smx_key, Smx_output, Smx_lv);
    printf("CBC encryption:");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    SM4_Run(256, SM4_CBC_DECODE, Smx_output, Smx_key, Smx_output, Smx_lv);
    printf("CBC decryption:");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
}
#endif
#endif
spoint SM2Pulib_Key = {
    .x[0] = 0x80b876a1,
    .x[1] = 0xbfd216e4,
    .x[2] = 0xfafc4a40,
    .x[3] = 0x7aa35d31,
    .x[4] = 0x97610239,
    .x[5] = 0xc6d44810,
    .x[6] = 0x0b593495,
    .x[7] = 0x7aa28ca3,
    .y[0] = 0x1b044541,
    .y[1] = 0x81788221,
    .y[2] = 0x23ca3752,
    .y[3] = 0xe9e0ceb7,
    .y[4] = 0x7d446169,
    .y[5] = 0xd28b0d28,
    .y[6] = 0xe2a98a76,
    .y[7] = 0xb5ed19c9,
};
DWORD SM2_E[8] = {
    0xb0920555,
    0x5d237c29,
    0xcfba9077,
    0x14cd1550,
    0xb6e73f74,
    0x33b99034,
    0xb90cd5df,
    0x7ee06a90,
};
ssign SM2_signer = {
    .r[0] = 0x50227612,
    .r[1] = 0xaef7aa60,
    .r[2] = 0xb903baba,
    .r[3] = 0x02784804,
    .r[4] = 0x87286dd2,
    .r[5] = 0xc3c71ea2,
    .r[6] = 0xdc4c6c99,
    .r[7] = 0x6da4c4ed,
    .s[0] = 0xe5231de0,
    .s[1] = 0x9cfced64,
    .s[2] = 0xf23d34b4,
    .s[3] = 0xf98c0c4d,
    .s[4] = 0x4f537fa6,
    .s[5] = 0xa3a3f3d8,
    .s[6] = 0x024143a4,
    .s[7] = 0xae94050c,
};

#endif
#else
#include <KERNEL_SMx.H>
#ifdef AE103
static DWORD SMx_Change(DWORD data)
{
    return (((data & 0x000000ff) << 24) | ((data & 0x0000ff00) << 8) | ((data & 0x00ff0000) >> 8) | ((data & 0xff000000) >> 24));
}
ssign_string *SMx_sign2string(ssign *sign, ssign_string *sign_string)
{
    int i, j;
    uDword r_temp, s_temp;
    u_int8_t temp;
    sign_string->space1 = sign_string->space2 = 0;
    sign_string->head[0] = '0';
    sign_string->head[1] = '4';
    if(sign == NULL || sign_string == NULL)return NULL;
    for(i = 0; i < 8; i++)
    {
        r_temp.dword = sign->r[7 - i];
        s_temp.dword = sign->s[7 - i];
        for(j = 0; j < 4; j++)
        {
            temp = r_temp.byte[3 - j] & 0xf;
            if(temp < 10)temp += '0';
            else temp = temp - 10 + 'A';
            sign_string->r[(i << 3) + (j << 1) + 1] = temp;
            temp = (r_temp.byte[3 - j] >> 4) & 0xf;
            if(temp < 10)temp += '0';
            else temp = temp - 10 + 'A';
            sign_string->r[(i << 3) + (j << 1)] = temp;
            temp = s_temp.byte[3 - j] & 0xf;
            if(temp < 10)temp += '0';
            else temp = temp - 10 + 'A';
            sign_string->s[(i << 3) + (j << 1) + 1] = temp;
            temp = (s_temp.byte[3 - j] >> 4) & 0xf;
            if(temp < 10)temp += '0';
            else temp = temp - 10 + 'A';
            sign_string->s[(i << 3) + (j << 1)] = temp;
        }
    }
    return sign_string;
}
ssign *SMx_string2sign(ssign_string *sign_string, ssign *sign)
{
    int i, j;
    uDword r_temp, s_temp;
    u_int8_t temp;
    if(sign_string == NULL || sign_string == NULL)return NULL;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 4; j++)
        {
            temp = sign_string->r[(i << 3) + (j << 1) + 1];
            if(is_digit(temp))temp -= '0';
            else temp = temp - 'A' + 10;
            r_temp.byte[3 - j] = temp;
            temp = sign_string->r[(i << 3) + (j << 1)];
            if(is_digit(temp))temp -= '0';
            else temp = temp - 'A' + 10;
            r_temp.byte[3 - j] = (r_temp.byte[3 - j]) + (temp << 4);

            temp = sign_string->s[(i << 3) + (j << 1) + 1];
            if(is_digit(temp))temp -= '0';
            else temp = temp - 'A' + 10;
            s_temp.byte[3 - j] = temp;
            temp = sign_string->s[(i << 3) + (j << 1)];
            if(is_digit(temp))temp -= '0';
            else temp = temp - 'A' + 10;
            s_temp.byte[3 - j] = (s_temp.byte[3 - j]) + (temp << 4);
        }
        sign->r[7 - i] = r_temp.dword;
        sign->s[7 - i] = s_temp.dword;
    }
    return sign;
}
//TRNG
u32 *TRNG_SET_SEED(u32 *seed)
{

    if((TRNG_STAT & TRNG_STAT_MISSION_MODE) && (!(TRNG_STAT & TRNG_STAT_NONCE_MODE)))
    {
        return NULL;
    }
    TRNG_SEED0 = *(seed + 0);
    TRNG_SEED1 = *(seed + 1);
    TRNG_SEED2 = *(seed + 2);
    TRNG_SEED3 = *(seed + 3);
    if((TRNG_STAT & TRNG_STAT_R256))//r256==1
    {
        TRNG_SEED4 = *(seed + 4);
        TRNG_SEED5 = *(seed + 5);
        TRNG_SEED6 = *(seed + 6);
        TRNG_SEED7 = *(seed + 7);
    }
    return seed;
}
u32 *TRNG_GET_SEED(u32 *seed)
{
    *(seed + 0) = TRNG_SEED0;
    *(seed + 1) = TRNG_SEED1;
    *(seed + 2) = TRNG_SEED2;
    *(seed + 3) = TRNG_SEED3;

    if((TRNG_STAT & TRNG_STAT_R256))//r256==1
    {
        *(seed + 4) = TRNG_SEED4;
        *(seed + 5) = TRNG_SEED5;
        *(seed + 6) = TRNG_SEED6;
        *(seed + 7) = TRNG_SEED7;

    }
    return seed;
}
u32 *TRNG_GET_RAND(u32 *rand)
{
    *(rand + 0) = TRNG_RAND0;
    *(rand + 1) = TRNG_RAND1;
    *(rand + 2) = TRNG_RAND2;
    *(rand + 3) = TRNG_RAND3;

    if((TRNG_STAT & TRNG_STAT_R256))//r256==1
    {
        *(rand + 4) = TRNG_RAND4;
        *(rand + 5) = TRNG_RAND5;
        *(rand + 6) = TRNG_RAND6;
        *(rand + 7) = TRNG_RAND7;

    }
    return rand;
}
#define TRNG_TEST 0
void TRNG_Init(u8 ie, u16 rqsts, u16 age)
{
    TRNG_MODE = TRNG_MODE_R256;//1:256bit
    //TRNG_MODE = ~TRNG_MODE_R256;//0:128bit
#if TRNG_TEST
    TRNG_SMODE &= ~TRNG_SMODE_MISSION_MODE;//測試模式
#else
    TRNG_SMODE |= TRNG_SMODE_MISSION_MODE;//任务模式，不开nonce
#endif
    if(ie)
        TRNG_IE = TRNG_IE_GLBL_EN | (ie & 0b11111);
    else
        TRNG_IE = ~(TRNG_IE_GLBL_EN | (0b11111));
    TRNG_AUTO_RQSTS = rqsts & 0xffff;//自动请求寄存器每产生x*16个随机数更新随机种子，0关闭，否则再重新生成随机种子后作为重载值（数量），rqsts中断
    TRNG_AUTO_AGE = age & 0xffff;//自动过期计数器x*2^26 个AHB周期（24M约3x sec一次），0关闭，否则再重新生成随机种子后作为重载值（时间），age中断
}
u32 *TRNG_RUN(u32 *seed, u32 *rand)
{
    // while(TRNG_STAT & TRNG_STAT_SRVC_RQST);//存在未处理的命令
    // GPIO0_DR0 = 0xff;
#if TRNG_TEST
    TRNG_CTRL = 0x2;//auto seed
#else
    if(seed == NULL)
    {
        if(((TRNG_STAT >> 16) & 0x7) == 0x7)//未生成随机种子
        {
            TRNG_CTRL = 0x2;//auto seed
        }
        else if(((TRNG_STAT >> 16) & 0x7) == 0x3)//通过NONCE作为随机种子
        {
            TRNG_CTRL = 0x2;//auto seed
        }
        else if(((TRNG_STAT >> 16) & 0x7) == 0x4)//自动生成
        {
        }
        else if(((TRNG_STAT >> 16) & 0x7) == 0x0)//通过命令生成
        {
            TRNG_CTRL = 0x2;//auto seed
        }
    }
    else
    {
        //再开NONCE或测试模式下执行
        if((TRNG_STAT & TRNG_STAT_NONCE_MODE) || (!(TRNG_STAT & TRNG_STAT_MISSION_MODE)))
        {
            TRNG_SET_SEED(seed);
            TRNG_CTRL = 0x3;//set seed
        }
        else
        {
            TRNG_CTRL = 0x2;//auto seed
        }
    }
#endif
    // GPIO0_DR0 = 0x00;
    while(TRNG_STAT & TRNG_STAT_RAND_RESEEDING);//正在产生随机种子
    // GPIO0_DR0 = 0xff;
    if(!(TRNG_STAT & TRNG_STAT_SEEDED))return NULL;//未生成随机种子，失效
    // GPIO0_DR0 = 0x00;

    // GPIO0_DR0 = 0xff;
    TRNG_CTRL = 0x1;//set rand
    // GPIO0_DR0 = 0x00;
    while(TRNG_STAT & TRNG_STAT_RAND_GENERATING);//正在产生随机数
    // GPIO0_DR0 = 0x00;
    TRNG_GET_RAND(rand);
    return rand;
}

//SM4
static void SM4_Key_Set(DWORDP key)
{
    SM4_MODE = SM4_MODE_KEYEX;
    SM4_KEY0 = SMx_Change(*(key + 0));
    SM4_KEY1 = SMx_Change(*(key + 1));
    SM4_KEY2 = SMx_Change(*(key + 2));
    SM4_KEY3 = SMx_Change(*(key + 3));
    SM4_STATUS |= SM4_STATUS_DONE;
    SM4_CTRL &= ~SM4_CTRL_INTEN;
    SM4_CTRL |= SM4_CTRL_START;
    while(!(SM4_STATUS & SM4_STATUS_DONE)) {}
}
static void SM4_Data_IN(DWORDP input)
{
    SM4_DATA0 = SMx_Change(*(input + 0));
    SM4_DATA1 = SMx_Change(*(input + 1));
    SM4_DATA2 = SMx_Change(*(input + 2));
    SM4_DATA3 = SMx_Change(*(input + 3));
}
static void SM4_XORDR_IN(DWORDP xordr)
{
    SM4_XORD0 = SMx_Change(*(xordr + 0));
    SM4_XORD1 = SMx_Change(*(xordr + 1));
    SM4_XORD2 = SMx_Change(*(xordr + 2));
    SM4_XORD3 = SMx_Change(*(xordr + 3));
}
static void SM4_Mode_Set(BYTE mode)
{
    if(mode == SM4_ECB_ENCODE)
        SM4_MODE = SM4_MODE_ENDE1;
    else if(mode == SM4_ECB_DECODE)
        SM4_MODE = SM4_MODE_ENDE0;
    else if(mode == SM4_CBC_ENCODE)
        SM4_MODE = SM4_MODE_XOR | SM4_MODE_ENDE1;
    else if(mode == SM4_CBC_DECODE)
        SM4_MODE = SM4_MODE_XOR | SM4_MODE_ENDE0;
}
static void SM4_Data_Out(DWORDP output)
{
    SM4_STATUS |= SM4_STATUS_DONE;
    SM4_CTRL &= ~SM4_CTRL_INTEN;
    SM4_CTRL |= SM4_CTRL_START;
    while(!(SM4_STATUS & SM4_STATUS_DONE)) {}
    *(output + 0) = SMx_Change(SM4_DATA0);
    *(output + 1) = SMx_Change(SM4_DATA1);
    *(output + 2) = SMx_Change(SM4_DATA2);
    *(output + 3) = SMx_Change(SM4_DATA3);
}
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SM4
 * note :
 *          mode:0 ECB加密,1 ECB解码/2 CBC加密/3 CBC解码
 *          input: Plaintext password
 *          key:Extended secret key
 *          output: Ciphertext storage
 *          xordr :lv
 * ************************************************************************************/
/* void SM4_Run(BYTE mode,BYTEP input,BYTEP key,BYTEP output,BYTEP xordr)
{
        SM4_Key_Set((DWORDP)key);//set key
        SM4_Data_IN((DWORDP)input);//input data
        SM4_Mode_Set(mode);
        if(mode == 2 || mode == 3)
        {
            SM4_XORDR_IN((DWORDP)xordr);//Functionality not implemented
        }
        SM4_Data_Out((DWORDP)output);
} */
void SM4_Run(DWORD bit, BYTE mode, BYTEP input, BYTEP key, BYTEP output, BYTEP xordr)
{
    if(bit & 127)
    {
        printf("SM4 data bit size ERROR\n");
        return;
    }
    SM4_Key_Set((DWORDP)key);
    SM4_Mode_Set(mode);
    if(mode == SM4_CBC_ENCODE || mode == SM4_CBC_DECODE)
    {
        SM4_XORDR_IN((DWORDP)xordr);    //set xordr
    }
    for(int i = 0; i * 128 < bit; i++)
    {
        SM4_Data_IN((DWORDP)(input + i * 16));   //input data
        SM4_Data_Out((DWORDP)(output + i * 16)); //output data
    }
}
//SM3
static BYTE data_offset = 0x0;
static void SM3_Auto_Write(DWORD data)
{
    SM3_REG(SM3_DATA0_OFFSET + data_offset) = data;
    data_offset = (data_offset + 0x4) & 63;
    while(!(data_offset || (SM3_INT & 0x1))) {}
    if(SM3_INT & 0x1)SM3_INT = 0x1;
}
static void SM3_Data_In(DWORD bit, DWORDP input)
{
    DWORD dword = bit >> 5;//double word size
    DWORD rbyte = (bit >> 3) & 3;//Remainder byte
    DWORD rbit = (bit & 31) - (rbyte << 3);//Remainder bit
    DWORD block = (bit & 511) >> 5;//final block byte size
    uDword buffer;
    DWORD i = 0, j = 0;
    for(i = 0; i < dword; i++)
    {
        buffer.dword = (*(input + i));
        SM3_Auto_Write(buffer.dword);
        //SM3_IN=buffer.dword;
    }
    buffer.dword = 0;
    for(j = 0; j < rbyte; j++)
    {
        buffer.byte[j] = *(((BYTEP)input) + (i << 2) + j);
    }
    rbit &= 7;
    if(rbit)buffer.byte[j] = *((BYTEP)input + (i << 2) + j);
    else buffer.byte[j] = 0x0;
    for(i = 0; i < rbit; i++)
    {
        buffer.byte[j] &= (~(0x80 >> i));
    }
    if(rbit || rbyte)
    {
        buffer.byte[j] |= (0x80 >> i);
        SM3_Auto_Write(buffer.dword);
        block++;
    }
    else
    {
        buffer.dword = 0x80;
        SM3_Auto_Write(buffer.dword);
        block++;
    }
    if(block >= 16)block &= 15;
    while(block < 15)
    {
        buffer.dword = 0;
        SM3_Auto_Write(buffer.dword);
        block++;
    }
    buffer.dword = SMx_Change(bit);
    SM3_Auto_Write(buffer.dword);
    data_offset = 0x0;
}
static void SM3_Data_Out(DWORDP output)
{
    *(output + 0) = SMx_Change(SM3_OUT7);
    *(output + 1) = SMx_Change(SM3_OUT6);
    *(output + 2) = SMx_Change(SM3_OUT5);
    *(output + 3) = SMx_Change(SM3_OUT4);
    *(output + 4) = SMx_Change(SM3_OUT3);
    *(output + 5) = SMx_Change(SM3_OUT2);
    *(output + 6) = SMx_Change(SM3_OUT1);
    *(output + 7) = SMx_Change(SM3_OUT0);
}
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SM3
 * note :
 *          bit:data bit size
 *          input: Plaintext password
 *          output: Ciphertext storage
 * ************************************************************************************/
void SM3_Run(DWORD bit, BYTEP input, BYTEP output)
{
    SM3_CONIFG |= SM3_CONFIG_ENABLE;//使能sm3   
    SM3_CONIFG &= ~SM3_CONFIG_NEW_DATA;//data in
    SM3_Data_In(bit, (DWORDP)input);//data in
    SM3_CONIFG |= SM3_CONFIG_ENABLE | SM3_CONFIG_NEW_DATA;//config run
    while(!(SM3_STATE & SM3_STATE_FINISH)) {}
    SM3_Data_Out((DWORDP)output);
}
//SM2
static void SM2_IO(VDWORDP addr1, VDWORDP addr2)
{
    VBYTE i = 0;
    while(i < 8) { *(addr1 + i) = *(addr2 + i); i++; }
}
static void SM2_Data_IN(DWORDP X1, DWORDP X2, DWORDP Y1, DWORDP Y2, DWORDP K)
{
    if(X1 != NULL)SM2_IO(&SM2_P1X0, (VDWORDP)X1);
    if(X2 != NULL)SM2_IO(&SM2_P2X0, (VDWORDP)X2);
    if(Y1 != NULL)SM2_IO(&SM2_P1Y0, (VDWORDP)Y1);
    if(Y2 != NULL)SM2_IO(&SM2_P2Y0, (VDWORDP)Y2);
    if(K != NULL)SM2_IO(&SM2_K0, (VDWORDP)K);
}
static void SM2_Data_Out(DWORDP X3, DWORDP Y3)
{
    if(X3 != NULL)SM2_IO((VDWORDP)X3, &SM2_P3X0);
    if(Y3 != NULL)SM2_IO((VDWORDP)Y3, &SM2_P3Y0);
}
static void SM2_ALU(DWORDP X1, DWORDP X2, DWORDP Y1, DWORDP Y2, DWORDP K, DWORD control, DWORDP X3, DWORDP Y3)
{
    SM2_Data_IN(X1, X2, Y1, Y2, K);//input
    SM2_CONTROL = SM2_CONTROL_EN | control;//run
    while(SM2_STATUS != SM2_STATUS_DONE)//wait
    {
        if(SM2_STATUS == SM2_STATUS_ERROR)
        {
            SM2_Error();
        }
    }
    SM2_Data_Out(X3, Y3);//out
    SM2_CONTROL = SM2_CONTROL_FREE;//free
}
//模N反
void SM2_N_INV(DWORDP input, DWORDP output)
{
    SM2_ALU(input, NULL, NULL, NULL, NULL, SM2_CONTROL_MODN_INV, NULL, output);
}
//模N乘
void SM2_N_MUL(DWORDP input1, DWORDP input2, DWORDP output)
{
    SM2_ALU(input1, input2, NULL, NULL, NULL, SM2_CONTROL_MODN_MUL, NULL, output);
}
//模N加
void SM2_N_ADD(DWORDP input1, DWORDP input2, DWORDP output)
{
    SM2_ALU(input1, input2, NULL, NULL, NULL, SM2_CONTROL_MODN_ADD, NULL, output);
}
//模N减
void SM2_N_SUB(DWORDP input1, DWORDP input2, DWORDP output)
{
    SM2_ALU(input1, input2, NULL, NULL, NULL, SM2_CONTROL_MODN_SUB, NULL, output);
}
//点加
void SM2_DOT_ADD(spoint *input1, spoint *input2, spoint *output)
{
    SM2_ALU(input1->x, input2->x, input1->y, input2->y, NULL, SM2_CONTROL_DOT_ADD, output->x, output->y);
}
//倍点
void SM2_DOT_DBL(spoint *input, spoint *output)
{
    SM2_ALU(input->x, NULL, input->y, NULL, NULL, SM2_CONTROL_DOT_DBL, output->x, output->y);
}
//点乘
void SM2_DOT_MUL(spoint *input, DWORDP k, spoint *output)
{
    SM2_ALU(input->x, NULL, input->y, NULL, k, SM2_CONTROL_DOT_MUL, output->x, output->y);
}
//模P反
void SM2_P_INV(DWORDP input, DWORDP output)
{
    SM2_ALU(input, NULL, NULL, NULL, NULL, SM2_CONTROL_MODP_INV, NULL, output);
}
//模P乘
void SM2_P_MUL(DWORDP input1, DWORDP input2, DWORDP output)
{
    SM2_ALU(input1, input2, NULL, NULL, NULL, SM2_CONTROL_MODP_MUL, NULL, output);
}
//模P加
void SM2_P_ADD(DWORDP input1, DWORDP input2, DWORDP output)
{
    SM2_ALU(input1, input2, NULL, NULL, NULL, SM2_CONTROL_MODP_ADD, NULL, output);
}
//模P减
void SM2_P_SUB(DWORDP input1, DWORDP input2, DWORDP output)
{
    SM2_ALU(input1, input2, NULL, NULL, NULL, SM2_CONTROL_MODP_SUB, NULL, output);
}
// VBYTE SM2_ID[] = "L-silicon";
// VBYTE SM2_DATA[] = "SPK32AE103";
//SM2 elipse 土建使用素数域256bit椭圆曲线，方程y平方＝x三次方+ax+b
const DWORD SM2_Elipse_P[8] = {
    0xFFFFFFFF,
    0xFFFFFFFF,
    0x00000000,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE };
const DWORD SM2_Elipse_A[8] = {
    0xFFFFFFFC,
    0xFFFFFFFF,
    0x00000000,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE };
const DWORD SM2_Elipse_B[8] = {
    0x4D940E93,
    0xDDBCBD41,
    0x15AB8F92,
    0xF39789F5,
    0xCF6509A7,
    0x4D5A9E4B,
    0x9D9F5E34,
    0x28E9FA9E };
const DWORD SM2_Elipse_N[8] = {
    0x39D54123,
    0x53BBF409,
    0x21C6052B,
    0x7203DF6B,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFF,
    0xFFFFFFFE };
const spoint SM2_Elipse_G = {
    .x[0] = 0x334C74C7,
    .x[1] = 0x715A4589,
    .x[2] = 0xF2660BE1,
    .x[3] = 0x8FE30BBF,
    .x[4] = 0x6A39C994,
    .x[5] = 0x5F990446,
    .x[6] = 0x1F198119,
    .x[7] = 0x32C4AE2C,
    .y[0] = 0x2139F0A0,
    .y[1] = 0x02DF32E5,
    .y[2] = 0xC62A4740,
    .y[3] = 0xD0A9877C,
    .y[4] = 0x6B692153,
    .y[5] = 0x59BDCEE3,
    .y[6] = 0xF4F6779C,
    .y[7] = 0xBC3736A2 };
BYTEP SM2_Z_Hashing(u_int16_t EntlenA, BYTEP ID, spoint *PA, BYTEP output)//杂凑值生成//EntlenA:ID的bit
{
    //ZA=H256(ENTLA||IDA||a||b||xG||yG||xA||yA)
    register u_int32_t i = 0, j = 0;

    DWORDP dword_input = NULL;
    BYTEP byte_input = NULL;
//        bit=ENTLA16bit+IDEntlen+a_bit+b_bit+xGbit+yGbit+xAbit+yAbit;//2+ENtlenA/8 + 32*6 = 194byte+ENtlenA/8
    DWORD bit = (2 * 8) + EntlenA + 256 + 256 + 256 + 256 + 256 + 256;//統計bit數

    DWORDP a = (DWORDP)SM2_Elipse_A;
    DWORDP b = (DWORDP)SM2_Elipse_B;
    DWORDP gx = (DWORDP)SM2_Elipse_G.x;
    DWORDP gy = (DWORDP)SM2_Elipse_G.y;
    DWORDP ax = (DWORDP)PA->x;
    DWORDP ay = (DWORDP)PA->y;

    BYTEP input = malloc((sizeof(BYTE) * ((bit >> 3) + ((bit & 0x7) ? 1 : 0) + 1)));//申請空間
    if(input == NULL)return NULL;//申請失敗

    //ENTLA||IDA||a||b||xG||yG||xA||yA
    {
        byte_input = input;
        //ENTLA
        *byte_input++ = ((EntlenA >> 8) & 0xff);
        *byte_input++ = (EntlenA & 0xff);
        //IDA
        i = EntlenA & 7;
        EntlenA = (EntlenA >> 3) + (i ? 1 : 0);
        for(j = 0; j < EntlenA; j++)
        {
            *byte_input++ = ID[j];
        }
        dword_input = (DWORDP)byte_input;
        //a
        for(j = 0; j < 8; j++) { *dword_input++ = SMx_Change(a[7 - j]); }
        //b
        for(j = 0; j < 8; j++) { *dword_input++ = SMx_Change(b[7 - j]); }
        //xG
        for(j = 0; j < 8; j++) { *dword_input++ = SMx_Change(gx[7 - j]); }
        //yG
        for(j = 0; j < 8; j++) { *dword_input++ = SMx_Change(gy[7 - j]); }
        //xA
        for(j = 0; j < 8; j++) { *dword_input++ = SMx_Change(ax[7 - j]); }
        //yA
        for(j = 0; j < 8; j++) { *dword_input++ = SMx_Change(ay[7 - j]); }
        //offfset
        if(i)//默認最高位開始計算
        {
            while(byte_input < ((BYTEP)(dword_input)-1))
            {
                *(byte_input - 1) = (*(byte_input - 1)) | ((*(byte_input)) >> i);
                *byte_input <<= (8 - i);
                byte_input++;
            }
        }
    }
    SM3_Run(bit, input, (BYTEP)output);//Hash256
    free(input);
    return output;
}

DWORD SM2_CREATE_KEY_PAIR(DWORDP dA, spoint *PA)
{
    DWORD dA_temp;
    DWORD error = 0;
    if(dA == NULL || PA == NULL)return SM2_KEY_PAIR_FALSE;
    // DWORDP p = (DWORDP)SM2_Elipse_P;
    // DWORDP a = (DWORDP)SM2_Elipse_A;
    // DWORDP b = (DWORDP)SM2_Elipse_B;
    DWORDP n = (DWORDP)SM2_Elipse_N;
    spoint *G = (spoint *)&SM2_Elipse_G;

    TRNG_Init(0, 1, 1);
    //rand range dA
    do
    {
        if(error++ >= 100)return SM2_KEY_PAIR_FALSE;

        TRNG_RUN(NULL, dA);
        dA_temp = dA[0] | dA[1] | dA[2] | dA[3] | dA[4] | dA[5] | dA[6] | dA[7];
        for(int i = 0; i < 8; i++)
        {
            if(dA[7 - i] > n[7 - i]) { continue; }//重新來
            else if(dA[7 - i] < n[7 - i]) { break; }//n更大
            else { if(i == 7)continue; }//完全等於則重新生成(因為＜n)
        }
    }
    while(!dA_temp);
    //PA=[dA]G
    SM2_DOT_MUL(G, dA, PA);
    return 0;
}
ssign *SM2_SIGN_PROCESS(DWORDP Z_Hash, DWORD message_bit, BYTE *message, spoint *PA, DWORDP dA, ssign *output)
{
    DWORD e[8];
    DWORD k[8];
    spoint P;
    DWORD da1[8];
    DWORD rda[8];

    register u_int32_t i = 0, j = 0, error = 0;
    BYTEP byte_input = NULL;
    DWORDP dword_input = NULL;

    spoint *G = (spoint *)&SM2_Elipse_G;
    DWORDP n = (DWORDP)SM2_Elipse_N;
    DWORD bit = 256 + message_bit;

    if(Z_Hash == NULL || message == NULL || PA == NULL || dA == NULL || output == NULL)return NULL;

    BYTEP input = malloc((sizeof(BYTE) * ((bit >> 3) + ((bit & 0x7) ? 1 : 0) + 1)));//申請空間
    if(input == NULL)return NULL;//申請失敗
    message_bit = ((message_bit >> 3) + ((message_bit & 0x7) ? 1 : 0));

    //第1步 M = Za|Message
    {
        //ZA
        dword_input = (DWORDP)input;
        for(j = 0; j < 8; j++) { *dword_input++ = (Z_Hash[j]); }
        //M
        byte_input = (BYTEP)dword_input;
        for(j = 0; j < message_bit; j++) { *byte_input++ = message[j]; }
    }
    //第2步 e =H256(M')
    {
        SM3_Run(bit, input, (BYTEP)e);
    }
    //第3步 生成隨機數k∈[1,n-1]
sing_process_randk:
    {
        TRNG_Init(0, 1, 1);
        if(error++ >= 100)return NULL;//100次還沒生成，認命退出
        TRNG_RUN(NULL, (u32 *)k);
        u_int32_t k_temp = k[0] | k[1] | k[2] | k[3] | k[4] | k[5] | k[6] | k[7];//判斷是否有1
        if(!k_temp)goto sing_process_randk;//非0即為＞＝1，0則為無1
        for(i = 0; i < 8; i++)
        {
            if(k[7 - i] > n[7 - i]) { goto sing_process_randk; }//重新來
            else if(k[7 - i] < n[7 - i]) { break; }//n更大
            else { if(i == 7)goto sing_process_randk; }//完全等於則重新生成(因為＜n)
        };
    }
    //第4步 (x1,y1) = [k]G
    {
        SM2_DOT_MUL(G, k, &P);//P =[s']G
    }
    //第5步 r = (e+x1)modn
    {
        SM2_N_ADD(e, P.x, output->r);
        //r=0||r+k=n?
        DWORD r_temp = output->r[0] | output->r[1] | output->r[2] | output->r[3] | output->r[4] | output->r[5] | output->r[6] | output->r[7];
        if(!r_temp)goto sing_process_randk;
        r_temp = 0;
        for(i = 0; i < 8; i++)
        {
            //相加
            r_temp = (output->r[i] + k[i] + r_temp);
            //比較
            if(r_temp != n[i])break;//不等於直接退出
            if(i == 7) { goto sing_process_randk; }//都相等那就重新生成
            //進位
            if(r_temp < k[i])r_temp = 1;
            else r_temp = 0;
        }
    }
    //第6步 s=((1+dA)−1·(k−r·dA))modn
    {
        //da1=1+dA
        u_int32_t da1_temp = 1;
        for(i = 0; i < 8; i++)
        {
            //相加
            da1[i] = dA[i] + da1_temp;
            if(da1[i] < dA[i]) { da1_temp = 1; }
            else da1_temp = 0;
        }
        //da1 = (da1)-1
        SM2_N_INV(da1, da1);
        //rda=r*dA
        {
            DWORD r[8];
            DWORD d[8];

            for(i = 0; i < 8; i++)
            {
                r[i] = output->r[i];
                d[i] = dA[i];
                rda[i] = 0;
            }

            for(j = 0; j < 256; j++)//256bit
            {
                u_int32_t r_temp = r[0] | r[1] | r[2] | r[3] | r[4] | r[5] | r[6] | r[7];//判斷是否有1,没有则结束（因为后续相加没用）
                u_int32_t d_temp = d[0] | d[1] | d[2] | d[3] | d[4] | d[5] | d[6] | d[7];//判斷是否有1
                if(!(r_temp | d_temp))
                {
                    break;
                }//有0则为0
                //相加
                if(r[0] & 1)
                {
                    r_temp = 0;
                    for(i = 0; i < 8; i++)
                    {
                        rda[i] += d[i] + r_temp;
                        if(rda[i] < d[i])r_temp = 1;
                        else r_temp = 0;
                    }

                }
                r_temp = 0;
                d_temp = 0;
                for(i = 0; i < 8; i++)
                {
                    d_temp = r[7 - i] & 1;
                    r[7 - i] >>= 1;
                    r[7 - i] |= r_temp;
                    r_temp = d_temp << 31;
                }
                r_temp = 0;
                d_temp = 0;
                for(i = 0; i < 8; i++)
                {
                    r_temp = d[i] & (1 << 31);
                    d[i] <<= 1;
                    d[i] |= d_temp;
                    d_temp = r_temp >> 31;
                }
            }
        }
        //rda=(k-rda)
        SM2_N_SUB(k, rda, rda);
        //(da1·rda)modn
        SM2_N_MUL(da1, rda, output->s);
        //S=0?
        DWORD s_temp = output->s[0] | output->s[1] | output->s[2] | output->s[3] | output->s[4] | output->s[5] | output->s[6] | output->s[7];
        if(!s_temp)goto sing_process_randk;
    }
    //第7步 ernter Digital Signature(確定數字簽名(r,s))//轉為字符串
    {
        printf("Digital Signature\n");
        printf("r:");
        for(i = 0; i < 8; i++) { printf("%08x", output->r[7 - i]); }printf("\n");
        printf("s:");
        for(i = 0; i < 8; i++) { printf("%08x", output->s[7 - i]); }printf("\n");

    }

    free(input);
    return output;
}
BYTE SM2_VERIFY_SIGN(DWORDP Z_Hash, DWORD message_bit, BYTE *message, spoint *PA, ssign *sign)
{
    DWORD R[8];
    DWORD e[8];
    DWORD t[8];

    spoint P0; spoint P1; spoint P2;
    spoint *G = (spoint *)&SM2_Elipse_G;
    DWORDP n = (DWORDP)SM2_Elipse_N;

    register u_int32_t i = 0, j = 0;

    BYTEP byte_input = NULL;
    DWORDP dword_input = NULL;

    DWORD bit = 256 + message_bit;

    if(Z_Hash == NULL || message == NULL || PA == NULL || sign == NULL)return SM2_VERIFY_SIGN_FALSE;


    BYTEP input = malloc((sizeof(BYTE) * ((bit >> 3) + ((bit & 0x7) ? 1 : 0) + 1)));//申請空間
    if(input == NULL)return SM2_VERIFY_SIGN_FALSE;//申請失敗
    message_bit = ((message_bit >> 3) + ((message_bit & 0x7) ? 1 : 0));


    //第1步 检验r'∈[1,n-1]
    {
        u_int32_t r_temp = sign->r[0] | sign->r[1] | sign->r[2] | sign->r[3] | sign->r[4] | sign->r[5] | sign->r[6] | sign->r[7];//判斷是否有1
        if(!r_temp)return SM2_VERIFY_SIGN_FALSE;//非0即為＞＝1，0則為無1
        for(i = 0; i < 8; i++)
        {
            if(sign->r[7 - i] < n[7 - i])//n更大
            {
                break;
            }
            else//r>=n
            {
                if((sign->r[7 - i] > n[7 - i]) || (i == 7))return SM2_VERIFY_SIGN_FALSE;
            }
        };
    }
    //第2步 检验s'∈[1,n-1]
    {
        u_int32_t s_temp = sign->s[0] | sign->s[1] | sign->s[2] | sign->s[3] | sign->s[4] | sign->s[5] | sign->s[6] | sign->s[7];//判斷是否有1
        if(!s_temp)return SM2_VERIFY_SIGN_FALSE;//非0即為＞＝1，0則為無1
        for(i = 0; i < 8; i++)
        {
            if(sign->s[7 - i] < n[7 - i])//n更大
            {
                break;
            }
            else//s>=n
            {
                if((sign->s[7 - i] > n[7 - i]) || (i == 7))return SM2_VERIFY_SIGN_FALSE;
            }
        };
    }
    //第3步 M'=ZA||M
    {
        //ZA
        dword_input = (DWORDP)input;
        for(j = 0; j < 8; j++) { *dword_input++ = (Z_Hash[j]); }
        //M
        byte_input = (BYTEP)dword_input;
        for(j = 0; j < message_bit; j++) { *byte_input++ = message[j]; }
    }
    //第4步 e'=SM3(M')
    {
        SM3_Run(bit, input, (BYTEP)e);
    }
    //第5步 t=(r'+s')modn
    {
        SM2_N_ADD(sign->r, sign->s, t);//t=(r'+s')modn
        //t=0?
        {
            u_int32_t t_temp = t[0] | t[1] | t[2] | t[3] | t[4] | t[5] | t[6] | t[7];//判斷是否有1
            if(!t_temp)return SM2_VERIFY_SIGN_FALSE;//t==0返回錯誤
        }
    }
    //第6步 (x1',y1')=[s']G+[t]Pa
    {
        SM2_DOT_MUL(G, sign->s, &P0);//p0 =[s']G
        SM2_DOT_MUL(PA, t, &P1);//p1 = [t]PA
        SM2_DOT_ADD(&P0, &P1, &P2);//P2=P1+P0
    }
        //第7步 R=(e'+P2.x')mod n
    {
        SM2_N_ADD(e, P2.x, R);//R=(e'+P2.x')modn
    }
    //R=r'
    {
        for(i = 0; i < 8; i++)//R=r'?
        {
            printf("%08x \n", R[i]);
            if(sign->r[i] != R[i])
                return SM2_VERIFY_SIGN_FALSE;
        }
    }

    free(input);
    return SM2_VERIFY_SIGN_TRUE;
}
WEAK void SM2_Error(void)
{
    printf("SM2 error of %#x\n", SM2_STATUS);
}
extern spoint SM2Pulib_Key;
extern ssign SM2_signer;
extern DWORD SM2_E[8];
DWORD SM2_K[8] = {
    0xEAC1BC21,
    0x6D54B80D,
    0x3CDBE4CE,
    0xEF3CC1FA,
    0xD9C02DCC,
    0x16680F3A,
    0xD506861A,
    0x59276E27
};
DWORD SM2Private_Key[8] = {
    0x4DF7C5B8,
    0x42FB81EF,
    0x2860B51A,
    0x88939369,
    0xC6D39F95,
    0x3F36E38A,
    0x7B2144B1,
    0x3945208F };//da
#if 0
spoint SM2Pulib_Key = {
   .x[0] = 0x56F35020,
   .x[1] = 0x6BB08FF3,
   .x[2] = 0x1833FC07,
   .x[3] = 0x72179FAD,
   .x[4] = 0x1E4BC5C6,
   .x[5] = 0x50DD7D16,
   .x[6] = 0x1E5421A1,
   .x[7] = 0x09F9DF31,
   .y[0] = 0x2DA9AD13,
   .y[1] = 0x6632F607,
   .y[2] = 0xF35E084A,
   .y[3] = 0x0AED05FB,
   .y[4] = 0x8CC1AA60,
   .y[5] = 0x2DC6EA71,
   .y[6] = 0xE26775A5,
   .y[7] = 0xCCEA490C
};
ssign SM2_signer = {
   .r[0] = 0xEEE720B3,
   .r[1] = 0x43AC7EAC,
   .r[2] = 0x27D5B741,
   .r[3] = 0x5944DA38,
   .r[4] = 0xE1BB81A1,
   .r[5] = 0x0EEAC513,
   .r[6] = 0x48D2C463,
   .r[7] = 0xF5A03B06,
   .s[0] = 0x85BBC1AA,
   .s[1] = 0x840B69C4,
   .s[2] = 0x1F7F42D4,
   .s[3] = 0xBB9038FD,
   .s[4] = 0x0D421CA1,
   .s[5] = 0x763182BC,
   .s[6] = 0xDF212FD8,
   .s[7] = 0xB1B6AA29
};
DWORD SM2_E[8] = {
   0xC0D28640,
   0xF4486FDF,
   0x19CE7B31,
   0x17E6AB5A,
   0x534382EB,
   0xACE692ED,
   0xBA45ACCA,
   0xF0B43E94
};
#endif
BYTE SM2_RUN(void)
{
    // spoint *PA = &SM2Pulib_Key;
    // DWORD *dA = SM2Private_Key;
    spoint PA;
    DWORD dA[8];
    ssign SM2_signer;
    ssign_string SM2_signString;
    char *ID = "SPK32AE103@sparkle-silicon";
    u_int16_t ID_bit = 8 * strlen(ID);
    u_int32_t Z_Hash[8];
    char *message = "message digest";
    DWORD message_bit = 8 * strlen(message);

        // GPIO0_DR0 = 0xff;
        // GPIO0_DR0 = 0x00;
    //鑰匙
        // if(SM2_CREATE_KEY_PAIR(dA, &PA))return SM2_KEY_PAIR_FALSE;//生成公钥
        // GPIO0_DR0 = 0xff;
    printf("create key pair\n");
    printf("private key:");
    for(int i = 0; i < 8; i++) { printf("%08x", dA[7 - i]); }printf("\n");
    printf("public key:\n");
    printf("x:");
    for(int i = 0; i < 8; i++) { printf("%08x", PA.x[7 - i]); }printf("\n");
    printf("y:");
    for(int i = 0; i < 8; i++) { printf("%08x", PA.y[7 - i]); }printf("\n");

    // GPIO0_DR0 = 0x00;
//前置步驟ZA=H256(ENTLA||IDA||a||b||xG||yG||xA||yA)
    // if(SM2_Z_Hashing(ID_bit, (BYTEP)ID, &PA, (BYTEP)Z_Hash) == NULL)
    //     return SM2_Hash_Value_Z_FALSE;//生成Za數據
// GPIO0_DR0 = 0xff;
    printf("Hash Value\nZ:");
    for(int i = 0; i < 8; i++) { printf("%08x", SMx_Change(Z_Hash[7 - i])); }printf("\n");
// GPIO0_DR0 = 0x00;
//簽名
    // if(SM2_SIGN_PROCESS((DWORDP)Z_Hash, message_bit, (BYTEP)message, &PA, dA, &SM2_signer) == NULL)
    //     return SM2_SIGN_PROCESS_FALSE;
    // // GPIO0_DR0 = 0xff;
    SMx_sign2string(&SM2_signer, &SM2_signString);
    printf("Digital Signature string\n");
    printf("%s||%s||%s\n", SM2_signString.head, SM2_signString.r, SM2_signString.s);
    SMx_string2sign(&SM2_signString, &SM2_signer);
    printf("Digital Signature\n");
    printf("r:");
    for(int i = 0; i < 8; i++) { printf("%08x", SM2_signer.r[7 - i]); }printf("\n");
    printf("s:");
    for(int i = 0; i < 8; i++) { printf("%08x", SM2_signer.s[7 - i]); }printf("\n");
// GPIO0_DR0 = 0x00;
//驗簽
    if(SM2_VERIFY_SIGN((DWORDP)Z_Hash, message_bit, (BYTEP)message, &PA, &SM2_signer) == SM2_VERIFY_SIGN_FALSE)
        return SM2_VERIFY_SIGN_FALSE;
    // GPIO0_DR0 = 0xff;
    // GPIO0_DR0 = 0x00;
    return SM2_VERIFY_SIGN_TRUE;
}
//eg:
#if 1
BYTE Smx_input[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
BYTE Smx_key[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
BYTE Smx_lv[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
BYTE Smx_output[32];
BYTE Sm3_input[] = { 0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64,0x61,0x62,0x63,0x64 };
/**************************************************************************************
 * Author :Linyu
 * date :23.2.8
 * function:run SMx test code
 * note :
 * ************************************************************************************/
void SM_Test_main(void)
{
    // nop;
    // GPIO0_INTEN0 = 0;
    // SYSCTL_PIO0_CFG &= ~0xffff;
    // GPIO0_DR0 = 0xff;
    // GPIO0_DDR0 = 0xff;
    // nop;
    // GPIO0_DR0 = 0x00;
    // GPIO0_DR0 = 0xff;
    // nop;
    BYTE i = 0;
    //TRNG debug
    printf("TRNG \n");
    TRNG_Init(0, 1, 1);
    printf("Init: Mode 256byte,IRQ Disable,Seed Auto\n");
    for(int j = 8; j; j--)
    {
        memset(Smx_output, 0, 32);
        // GPIO0_DR0 = 0x00;
        TRNG_RUN(NULL, (u32 *)Smx_output);
        // GPIO0_DR0 = 0xff;
        printf("RAND %02d:", (9 - j));
        for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    #if TRNG_TEST
        TRNG_GET_SEED((u32 *)Smx_output);
        printf("SEED %02d:", (9 - j));
        for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    #endif
    }
    //SM2 debug
    memset(Smx_output, 0, 32);
    // GPIO0_DR0 = 0x00;
    i = SM2_RUN();
    // GPIO0_DR0 = 0xff;
    if(i == SM2_VERIFY_SIGN_FALSE)printf("SM2 VERIFY SIGN ERROR\n");
    else if(i == SM2_SIGN_PROCESS_FALSE)printf("SM2 SIGN PROCESS ERROR\n");
    else if(i == SM2_KEY_PAIR_FALSE)printf("SM2 KEY PAIR ERROR\n");
    else if(i == SM2_Hash_Value_Z_FALSE)printf("SM2 Hash Value ZA ERROR\n");
    else printf("SM2 RUN SUCCEED\n");
#if 0
    //sm3 debug
    printf("SM3 \n");
    memset(Smx_output, 0, 32);
    GPIO0_DR0 = 0x00;
    SM3_Run(512, Sm3_input, Smx_output);
    GPIO0_DR0 = 0xff;
    printf("512 bit encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    GPIO0_DR0 = 0x00;
    SM3_Run(256, Sm3_input, Smx_output);
    GPIO0_DR0 = 0xff;
    printf("256 bit encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    GPIO0_DR0 = 0x00;
    SM3_Run(128, Sm3_input, Smx_output);
    GPIO0_DR0 = 0xff;
    printf("128 bit encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    // GPIO0_DR0 = 0x00;
    SM3_Run(32, Sm3_input, Smx_output);
    // GPIO0_DR0 = 0xff;
    printf("32 bit encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    // GPIO0_DR0 = 0x00;
    SM3_Run(24, Sm3_input, Smx_output);
    // GPIO0_DR0 = 0xff;
    printf("24 bit encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    // GPIO0_DR0 = 0x00;
    SM3_Run(16, Sm3_input, Smx_output);
    // GPIO0_DR0 = 0xff;
    printf("16 bit encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    // GPIO0_DR0 = 0x00;
    SM3_Run(8, Sm3_input, Smx_output);
    // GPIO0_DR0 = 0xff;
    printf("8 bit encryption:\n");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    printf("--------------------------------------------------------------\n");
    //sm4_debug
    printf("SM4 encryption and decryption:\n");
    memset(Smx_output, 0, 32);
    // GPIO0_DR0 = 0x00;
    SM4_Run(256, SM4_ECB_ENCODE, Smx_input, Smx_key, Smx_output, NULL);
    // GPIO0_DR0 = 0xff;
    printf("ECB encryption:");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    // GPIO0_DR0 = 0x00;
    SM4_Run(256, SM4_ECB_DECODE, Smx_output, Smx_key, Smx_output, NULL);
    // GPIO0_DR0 = 0xff;
    printf("ECB decryption:");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    memset(Smx_output, 0, 32);
    // GPIO0_DR0 = 0x00;
    SM4_Run(256, SM4_CBC_ENCODE, Smx_input, Smx_key, Smx_output, Smx_lv);
    // GPIO0_DR0 = 0xff;
    printf("CBC encryption:");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
    // GPIO0_DR0 = 0x00;
    SM4_Run(256, SM4_CBC_DECODE, Smx_output, Smx_key, Smx_output, Smx_lv);
    // GPIO0_DR0 = 0xff;
    printf("CBC decryption:");
    for(i = 0; i < 32; i++) { printf("%02x ", Smx_output[i]); }printf("\n");
#endif
}
#endif
#endif
spoint SM2Pulib_Key = {
    .x[0] = 0x80b876a1,
    .x[1] = 0xbfd216e4,
    .x[2] = 0xfafc4a40,
    .x[3] = 0x7aa35d31,
    .x[4] = 0x97610239,
    .x[5] = 0xc6d44810,
    .x[6] = 0x0b593495,
    .x[7] = 0x7aa28ca3,
    .y[0] = 0x1b044541,
    .y[1] = 0x81788221,
    .y[2] = 0x23ca3752,
    .y[3] = 0xe9e0ceb7,
    .y[4] = 0x7d446169,
    .y[5] = 0xd28b0d28,
    .y[6] = 0xe2a98a76,
    .y[7] = 0xb5ed19c9,
};
DWORD SM2_E[8] = {
    0xb0920555,
    0x5d237c29,
    0xcfba9077,
    0x14cd1550,
    0xb6e73f74,
    0x33b99034,
    0xb90cd5df,
    0x7ee06a90,
};
ssign SM2_signer = {
    .r[0] = 0x50227612,
    .r[1] = 0xaef7aa60,
    .r[2] = 0xb903baba,
    .r[3] = 0x02784804,
    .r[4] = 0x87286dd2,
    .r[5] = 0xc3c71ea2,
    .r[6] = 0xdc4c6c99,
    .r[7] = 0x6da4c4ed,
    .s[0] = 0xe5231de0,
    .s[1] = 0x9cfced64,
    .s[2] = 0xf23d34b4,
    .s[3] = 0xf98c0c4d,
    .s[4] = 0x4f537fa6,
    .s[5] = 0xa3a3f3d8,
    .s[6] = 0x024143a4,
    .s[7] = 0xae94050c,
};
#endif