/*
 * @Author: Iversu
 * @LastEditors: daweslinyu 
 * @LastEditTime: 2024-02-26 17:28:39
 * @Description: RC Table(s) for Scan Matix
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
#include "CUSTOM_SCANTABS.H"
#include "CUSTOM_POWER.H"
#include "KERNEL_MEMORY.H"
 // KBD_SN1211 Column ROW demo
 //     /* C\R    R0    R1    R2    R3    R4    R5    R6    R7 */
 //             /***********************************************/
 //     /* C0*/ {0x24, 0x04, 0x23, 0x0C, 0x21, 0x00, 0x06, 0x26},
 //     /* C0     E     F3    D     F4    C    K133   F2    3# */
 //             /***********************************************/
 //     /* C1*/ {0x2D, 0x2C, 0x2B, 0x34, 0x2A, 0x32, 0x2E, 0x25},
 //     /* C1     R     T     F     G     V     B     5%    4$ */
 //             /***********************************************/
 //     /* C2*/ {0x3C, 0x35, 0x3B, 0x33, 0x3A, 0x31, 0x36, 0x3D},
 //     /* C2     U     Y     J     H     M     N     6^    7& */
 //             /***********************************************/
 //     /* C3*/ {0x43, 0x5B, 0x42, 0x0B, 0x41, 0x00, 0x55, 0x3E},
 //     /* C3     I     ]}    K     F6    ,<    K56   =+    8* */
 //             /***********************************************/
 //     /* C4*/ {0x44, 0x80, 0x4B, 0x00, 0x49, 0x84, 0x0A, 0x46},
 //     /* C4     O     F7    L     H1    .>    APP   F8    9( */
 //             /***********************************************/
 //     /* C5*/ {0xA7, 0x00, 0x81, 0xE4, 0xEB, 0xE6, 0xE0, 0xE1},
 //     /* C5    +Pad  K107  ENTP   ↑    PPPS   ←    Home   End*/
 //             /***********************************************/
 //     /* C6*/ {0x9E, 0xA2, 0xA6, 0xAA, 0x9F, 0xA3, 0xE2, 0xE3},
 //     /* C6    9Pad  6Pad  3Pad  .Pad  *Pad  -Pad  PGUP  PGDN*/
 //             /***********************************************/
 //     /* C7*/ {0x9D, 0xA1, 0xA5, 0xA8, 0xAB, 0xE7, 0xFC, 0x00},
 //     /* C7    8Pad  5Pad  2Pad  0Pad  /Pad   →     Ins   H6 */
 //             /***********************************************/
 //     /* C8*/ {0x9C, 0xA0, 0xA4, 0x29, 0x90, 0xE5, 0xFA, 0x00},
 //     /* C8    7Pad  4Pad  1Pad  SPCE   Num   ↓     Del   H5 */
 //             /***********************************************/
 //     /* C9*/ {0x00, 0x88, 0x89, 0xEE, 0xED, 0xE8, 0xE9, 0xF9},
 //     /* C9     H7   SHFL  SHFR  VOL-  VOL+  NTTK  PRTK  MEDI*/
 //             /***********************************************/
 //     /*C10*/ {0x00, 0x66, 0x5D, 0x78, 0x5A, 0x07, 0x01, 0x09},
 //     /*C10     K14  BKSP   \|    F11   ENT   F12   F9    F10*/
 //             /***********************************************/
 //     /*C11*/ {0x4D, 0x54, 0x4C, 0x52, 0x00, 0x4A, 0x4E, 0x45},
 //     /*C11     P     [{    ;:    '"    K42   /?    -_    0) */
 //             /***********************************************/
 //     /*C12*/ {0x7E, 0x00, 0x8E, 0x8A, 0x00, 0x8B, 0x00, 0xFD},
 //     /*C12    SCRL   H2    Fn   AltL   H3   AltR   H4   PRTS*/
 //             /***********************************************/
 //     /*C13*/ {0x91, 0x85, 0xFE, 0x86, 0x8D, 0x87, 0x8C, 0x03},
 //     /*C13    PAUS   PWR  Euro   SLP  CtrR  WkUP  CtrL   F5 */
 //             /***********************************************/
 //     /*C14*/ {0xEF, 0x82, 0xF3, 0xF4, 0xF2, 0xF5, 0xEC, 0xF0},
 //     /*C14    Mail  WinL  WWFd  WWSp  WWBk  WWRH  Mute  WWSH*/
 //             /***********************************************/
 //     /*C15*/ {0x00, 0xF6, 0x27, 0xF8, 0xEA, 0xF7, 0xF1, 0x00},
 //     /*C15     KRL   fav  WinR  MyPC  Stop  Calc  WBHM   KRR*/
 //             /***********************************************/
 //     /*C16*/ {0x1D, 0x58, 0x1B, 0x00, 0x22, 0x00, 0x05, 0x1E},
 //     /*C16     W    Caps   S     K45   X    K132   F1    2@ */
 //             /***********************************************/
 //     /*C17*/ {0x15, 0x0D, 0x1C, 0x76, 0x1A, 0x00, 0x0E, 0x16},
 //     /*C17     Q     Tab   A     ESC   Z    K131   `~    1! */
 //             /***********************************************/
 //     /*C18*/ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x26},
 //     /*C18     H8    H9    H10   H11   H12   H13   H14   BL */
 //             /***********************************************/
 // Keyboard Printing Example
 // .-----..-----..-----..-----..-----..-----..-----..-----..-----..-----..-----..-----..-----..-----..-----..-----..------..-----..-----..-----.
 // |ESC  || F1  || F2  || F3  || F4  || F5  || F6  || F7  || F8  || F9  || F10 ||F11  || F12 ||PRINT||PAUSE|| Ins || Del  ||NUMLK||  /  ||  *  |
 // '-----''-----''-----''-----''-----''-----''-----''-----''-----''-----''-----''-----''-----''-----''-----''-----''------''-----''-----''-----'
 // .------..------..------..------..------..------..------..------..------..------..------..------..------..--------------..-----..-----..-----.
 // |  ~`  ||  1!  ||  2@  ||  3#  ||  4$  ||  5%  ||  6^  ||  7&  ||  8*  ||  9(  ||  0)  ||  -_  ||  +=  ||   Backspace  ||  .  ||  +  ||  -  |
 // |      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||              || Del ||     ||     |
 // '------''------''------''------''------''------''------''------''------''------''------''------''------''--------------''-----''-----''-----'
 // .----------..------..------..------..------..------..------..------..------..------..------..------..------..----------..-----..-----..-----.
 // |    Tab   ||  Q   ||  W   ||  E   ||  R   ||  T   ||  Y   ||  U   ||  I   ||  O   ||  P   ||  [{  ||  ]}  ||     \|   ||  7  ||  8  ||  9  |
 // |          ||      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||          || Home||  ↑  ||PG UP|
 // '----------''------''------''------''------''------''------''------''------''------''------''------''------''----------''-----''-----''-----'
 // .-------------..------..------..------..------..------..------..------..------..------..------..------..---------------..-----..-----..-----.
 // |     Cpas    ||  A   ||  S   ||  D   ||  F   ||  G   ||  H   ||  J   ||  K   ||  L   ||  ;:  ||  '"  ||     Enter     ||  4  ||  5  ||  6  |
 // |             ||      ||      ||      ||  __  ||      ||      ||  __  ||      ||      ||      ||      ||               ||  <- || __  ||  -> |
 // '-------------''------''------''------''------''------''------''------''------''------''------''------''---------------''-----''-----''-----'
 // .-----------------..------..------..------..------..------..------..------..------..------..------..-------------------..-----..-----..-----.
 // |      Shift      ||  Z   ||  X   ||  C   ||  V   ||  B   ||  N   ||  M   ||  ,<  ||  .>  ||  /?  ||       Shift       ||  1  ||  2  ||  3  |
 // |                 ||      ||      ||      ||      ||      ||      ||      ||      ||      ||      ||                   || End ||  ↓  ||PG DN|
 // '-----------------''------''------''------''------''------''------''------''------''------''------''-------------------''-----''-----''-----'
 // .------..------..------..------..--------------------------------------..------..------..------.        .------.        .-----..------------.
 // | Ctrl ||  Fn  ||  GUI ||  Alt ||                 SPACE                ||  Alt ||  GUI || Ctrl |.------.|__↑___|.------.|  0  ||    ENTER   |
 // |      ||      ||      ||      ||                                      ||      ||      ||      ||  <-  ||  ↓   ||  ->  || INS ||            |
 // '------''------''------''------''--------------------------------------''------''------''------''------''------''------''-----''------------'
 // 列行.（Column row）

const BYTE Cr_KBD_Tables[][8] = {
#if (defined(KBD_8_18)||defined(KBD_8_17)||defined(KBD_8_16))  // SN1211 16
    /* C\R    R0    R1    R2    R3    R4    R5    R6    R7 */
    /* C0*/ {0x24, 0x04, 0x23, 0x0C, 0x21, 0x00, 0x06, 0x26},
    /* C0     E     F3    D     F4    C    K133   F2    3# */
    /* C1*/ {0x2D, 0x2C, 0x2B, 0x34, 0x2A, 0x32, 0x2E, 0x25},
    /* C1     R     T     F     G     V     B     5%    4$ */
    /* C2*/ {0x3C, 0x35, 0x3B, 0x33, 0x3A, 0x31, 0x36, 0x3D},
    /* C2     U     Y     J     H     M     N     6^    7& */
    /* C3*/ {0x43, 0x5B, 0x42, 0x0B, 0x41, 0x00, 0x55, 0x3E},
    /* C3     I     ]}    K     F6    ,<    K56   =+    8* */
    /* C4*/ {0x44, 0x80, 0x4B, 0x00, 0x49, 0x84, 0x0A, 0x46},
    /* C4     O     F7    L     H1    .>    APP   F8    9( */
    /* C5*/ {0xA7, 0x00, 0x81, 0xE4, 0xEB, 0xE6, 0xE0, 0xE1},
    /* C5    +Pad  K107  ENTP   ↑    PPPS   ←    Home   End*/
    /* C6*/ {0x9E, 0xA2, 0xA6, 0xAA, 0x9F, 0xA3, 0xE2, 0xE3},
    /* C6    9Pad  6Pad  3Pad  .Pad  *Pad  -Pad  PGUP  PGDN*/
    /* C7*/ {0x9D, 0xA1, 0xA5, 0xA8, 0xAB, 0xE7, 0xFC, 0x00},
    /* C7    8Pad  5Pad  2Pad  0Pad  /Pad   →     Ins   H6 */
    /* C8*/ {0x9C, 0xA0, 0xA4, 0x29, 0x90, 0xE5, 0xFA, 0x00},
    /* C8    7Pad  4Pad  1Pad  SPCE   Num   ↓     Del   H5 */
    /* C9*/ {0x00, 0x88, 0x89, 0xEE, 0xED, 0xE8, 0xE9, 0xF9},
    /* C9     H7   SHFL  SHFR  VOL-  VOL+  NTTK  PRTK  MEDI*/
    /*C10*/ {0x00, 0x66, 0x5D, 0x78, 0x5A, 0x07, 0x01, 0x09},
    /*C10     K14  BKSP   \|    F11   ENT   F12   F9    F10*/
    /*C11*/ {0x4D, 0x54, 0x4C, 0x52, 0x00, 0x4A, 0x4E, 0x45},
    /*C11     P     [{    ;:    '"    K42   /?    -_    0) */
    /*C12*/ {0x7E, 0x00, 0x8E, 0x8A, 0x00, 0x8B, 0x00, 0xFD},
    /*C12    SCRL   H2    Fn   AltL   H3   AltR   H4   PRTS*/
    /*C13*/ {0x91, 0x85, 0xFE, 0x86, 0x8D, 0x87, 0x8C, 0x03},
    /*C13    PAUS   PWR  Euro   SLP  CtrR  WkUP  CtrL   F5 */
    /*C14*/ {0xEF, 0x82, 0xF3, 0xF4, 0xF2, 0xF5, 0xEC, 0xF0},
    /*C14    Mail  WinL  WWFd  WWSp  WWBk  WWRH  Mute  WWSH*/
    /*C15*/ {0x00, 0xF6, 0x27, 0xF8, 0xEA, 0xF7, 0xF1, 0x00},
    /*C15     KRL   fav  WinR  MyPC  Stop  Calc  WBHM   KRR*/
#endif
#if (defined(KBD_8_18)||defined(KBD_8_17))  // SN1211 17
    /*C16*/ {0x1D, 0x58, 0x1B, 0x00, 0x22, 0x00, 0x05, 0x1E},
    /*C16     W    Caps   S     K45   X    K132   F1    2@ */
#endif
#if (defined(KBD_8_18))  // SN1211 18
    /*C17*/ {0x15, 0x0D, 0x1C, 0x76, 0x1A, 0x00, 0x0E, 0x16},
    /*C17     Q     Tab   A     ESC   Z    K131   `~    1! */
#endif
};
// 行列式（Row Column）
const BYTE KB_S3WakeUP_Tables[][8] = {
#if (defined(KBD_8_18)||defined(KBD_8_17)||defined(KBD_8_16))  // SN1211 16/17/18
    /* C\R    R0    R1    R2    R3    R4    R5    R6    R7 */
    /* C0*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /* C0     E     F3    D     F4    C    K133   F2    3# */
    /* C1*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /* C1     R     T     F     G     V     B     5%    4$ */
    /* C2*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /* C2     U     Y     J     H     M     N     6^    7& */
    /* C3*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /* C3     I     ]}    K     F6    ,<    K56   =+    8* */
    /* C4*/ {0x55, 0x55, 0x55, 0x00, 0x55, 0x55, 0x55, 0x55},
    /* C4     O     F7    L     H1    .>    APP   F8    9( */
    /* C5*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /* C5    +Pad  K107  ENTP   ↑    PPPS   ←    Home   End*/
    /* C6*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /* C6    9Pad  6Pad  3Pad  .Pad  *Pad  -Pad  PGUP  PGDN*/
    /* C7*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00},
    /* C7    8Pad  5Pad  2Pad  0Pad  /Pad   →     Ins   H6 */
    /* C8*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x00},
    /* C8    7Pad  4Pad  1Pad  SPCE   Num   ↓     Del   H5 */
    /* C9*/ {0x00, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /* C9     H7   SHFL  SHFR  VOL-  VOL+  NTTK  PRTK  MEDI*/
    /*C10*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /*C10     K14  BKSP   \|    F11   ENT   F12   F9    F10*/
    /*C11*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /*C11     P     [{    ;:    '"    K42   /?    -_    0) */
    /*C12*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /*C12    SCRL   H2    Fn   AltL   H3   AltR   H4   PRTS*/
    /*C13*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /*C13    PAUS   PWR  Euro   SLP  CtrR  WkUP  CtrL   F5 */
    /*C14*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /*C14    Mail  WinL  WWFd  WWSp  WWBk  WWRH  Mute  WWSH*/
    /*C15*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /*C15     KRL   fav  WinR  MyPC  Stop  Calc  WBHM   KRR*/
#endif
#if (defined(KBD_8_18)||defined(KBD_8_17))  // SN1211 17/18
    /*C16*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /*        W    Caps   S     K45   X    K132   F1    2@ */
#endif
#if (defined(KBD_8_18))  // SN1211 18
    /*C17*/ {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55},
    /*        Q     Tab   A     ESC   Z    K131   `~    1! */
#endif
};
//----------------------------------------------------------------------------
// The function of hotkey Fn + F1
//----------------------------------------------------------------------------
void Do_HotKey_FnF1(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F2
//----------------------------------------------------------------------------
void Do_HotKey_FnF2(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F3
//----------------------------------------------------------------------------
void Do_HotKey_FnF3(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F4
//----------------------------------------------------------------------------
void Do_HotKey_FnF4(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F5
//----------------------------------------------------------------------------
void Do_HotKey_FnF5(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F6
//----------------------------------------------------------------------------
void Do_HotKey_FnF6(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F7
//----------------------------------------------------------------------------
void Do_HotKey_FnF7(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F8
//----------------------------------------------------------------------------
void Do_HotKey_FnF8(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F9
//----------------------------------------------------------------------------
void Do_HotKey_FnF9(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F10
//----------------------------------------------------------------------------
void Do_HotKey_FnF10(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F11
//----------------------------------------------------------------------------
void Do_HotKey_FnF11(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + F12
//----------------------------------------------------------------------------
void Do_HotKey_FnF12(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + ESC
//----------------------------------------------------------------------------
void Do_HotKey_FnESC(BYTE event)
{
    UNUSED_VAR(event);
}
//----------------------------------------------------------------------------
// The function of hotkey Fn + up arrow
//----------------------------------------------------------------------------
void Do_HotKey_FnUP(BYTE event) { UNUSED_VAR(event); }
//----------------------------------------------------------------------------
// The function of hotkey Fn + down arrow
//----------------------------------------------------------------------------
void Do_HotKey_FnDOWN(BYTE event) { UNUSED_VAR(event); }
//----------------------------------------------------------------------------
// The function of hotkey Fn + left arrow
//----------------------------------------------------------------------------
void Do_HotKey_FnLEFT(BYTE event) { UNUSED_VAR(event); }
//----------------------------------------------------------------------------
// The function of hotkey Fn + right arrow
//----------------------------------------------------------------------------
void Do_HotKey_FnRIGHT(BYTE event) { UNUSED_VAR(event); }
//----------------------------------------------------------------------------
// The function of hotkey Fn + Ins (Scr Lk)
//---------------------------------------------------------------------------
void Do_HotKey_FnIns(BYTE event) { UNUSED_VAR(event); }
//----------------------------------------------------------------------------
// The function of hotkey Fn + Del (NumLk)
//---------------------------------------------------------------------------
void Do_HotKey_FnDel(BYTE event) { UNUSED_VAR(event); }
const FUNCT_PTR_V_B HotKey_Fn_Fx[] = {
    Do_HotKey_FnF1,     // Fn + F1
    Do_HotKey_FnF2,     // Fn + F2
    Do_HotKey_FnF3,     // Fn + F3
    Do_HotKey_FnF4,     // Fn + F4
    Do_HotKey_FnF5,     // Fn + F5
    Do_HotKey_FnF6,     // Fn + F6
    Do_HotKey_FnF7,     // Fn + F7
    Do_HotKey_FnF8,     // Fn + F8
    Do_HotKey_FnF9,     // Fn + F9
    Do_HotKey_FnF10,    // Fn + F10
    Do_HotKey_FnF11,    // Fn + F11
    Do_HotKey_FnF12,    // Fn + F12
    Do_HotKey_FnESC,    // Fn + ESC
    Do_HotKey_FnUP,     // Fn + up arrow
    Do_HotKey_FnDOWN,   // Fn + down arrow
    Do_HotKey_FnLEFT,   // Fn + left arrow
    Do_HotKey_FnRIGHT,  // Fn + right arrow
    // Do_HotKey_FnIns,		// Fn + Ins (Scr Lk)
    // Do_HotKey_FnDel,		// Fn + Del (NumLk)
};
const BYTE ss2_9C[] =  // Numpad 7 or 7&
{
#ifdef KBD_8_16
    0x3D,  // Normal 		// 7
    0xB0,  // Numlock
    0xB0,  // Fn
    0xB0   // Fn+Numlock
#elif (defined(KBD_8_18))
    0x94,  // Normal 		// HOME
    0xB0,  // Numlock
    0xB0,  // Fn
    0xB0   // Fn+Numlock
#endif
};
const BYTE ss2_9D[] =  // Numpad 8 or 8*
{
#ifdef KBD_8_16
    0x3E,  // Normal 		// 8
    0xB1,  // Numlock
    0xB1,  // Fn
    0xB1   // Fn+Numlock
#elif (defined(KBD_8_18))
    0x98,  // Normal 		// UP arrow
    0xB1,  // Numlock
    0xB1,  // Fn
    0xB1   // Fn+Numlock
#endif
};
const BYTE ss2_9E[] =  // Numpad 9 or 9(
{
#ifdef KBD_8_16
    0x46,  // Normal 		// 9
    0xB2,  // Numlock
    0xB2,  // Fn
    0xB2   // Fn+Numlock
#elif (defined(KBD_8_18))
    0x96,  // Normal 		// PG UP
    0xB2,  // Numlock
    0xB2,  // Fn
    0xB2   // Fn+Numlock
#endif
};
const BYTE ss2_9F[] =  // NumPad * or 0)
{
#ifdef KBD_8_16
    0x45,  // Normal 		// 0
    0xB3,  // Numlock
    0xB3,  // Fn
    0xB3   // Fn+Numlock
#elif (defined(KBD_8_18))
    0xB3,  // Normal 		// *
    0xB3,  // Numlock
    0xB3,  // Fn
    0xB3   // Fn+Numlock
#endif
};
const BYTE ss2_A0[] =  // Numpad 4 or U
{
#ifdef KBD_8_16
    0x3C,  // Normal 		// U
    0xB4,  // Numlock
    0xB4,  // Fn
    0xB4   // Fn+Numlock
#elif (defined(KBD_8_18))
    0x9A,  // Normal 		// Left arrow
    0xB4,  // Numlock
    0xB4,  // Fn
    0xB4   // Fn+Numlock
#endif
};
const BYTE ss2_A1[] =  // Numpad 5 or I
{
#ifdef KBD_8_16
    0x43,  // Normal 		// I
    0xB5,  // Numlock
    0xB5,  // Fn
    0xB5   // Fn+Numlock
#elif (defined(KBD_8_18))
    0xB5,  // Normal 		// 5
    0xB5,  // Numlock
    0xB5,  // Fn
    0xB5   // Fn+Numlock
#endif
};
const BYTE ss2_A2[] =  // Numpad 6 or O
{
#ifdef KBD_8_16
    0x44,  // Normal 		// O
    0xB6,  // Numlock
    0xB6,  // Fn
    0xB6   // Fn+Numlock
#elif (defined(KBD_8_18))
    0x9B,  // Normal 		// Right arrow
    0xB6,  // Numlock
    0xB6,  // Fn
    0xB6   // Fn+Numlock
#endif
};
const BYTE ss2_A3[] =  // Numpad - or P
{
#ifdef KBD_8_16
    0x4D,  // Normal 		// P
    0xB7,  // Numlock
    0xB7,  // Fn
    0xB7   // Fn+Numlock
#elif (defined(KBD_8_18))
    0xB7,  // Normal 		// -
    0xB7,  // Numlock
    0xB7,  // Fn
    0xB7   // Fn+Numlock
#endif
};
const BYTE ss2_A4[] =  // Numpad 1 or J
{
#ifdef KBD_8_16
    0x3B,  // Normal 		// J
    0xB8,  // Numlock
    0xB8,  // Fn
    0xB8   // Fn+Numlock
#elif (defined(KBD_8_18))
    0x95,  // Normal 		// end
    0xB8,  // Numlock
    0xB8,  // Fn
    0xB8   // Fn+Numlock
#endif
};
const BYTE ss2_A5[] =  // Numpad 2 or K
{
#ifdef KBD_8_16
    0x42,  // Normal 		// K
    0xB9,  // Numlock
    0xB9,  // Fn
    0xB9   // Fn+Numlock
#elif (defined(KBD_8_18))
    0x99,  // Normal 		// Down arrow
    0xB9,  // Numlock
    0xB9,  // Fn
    0xB9   // Fn+Numlock
#endif
};
const BYTE ss2_A6[] =  // Numpad 3 or L
{
#ifdef KBD_8_16
    0x4B,  // Normal 		// L
    0xBA,  // Numlock
    0xBA,  // Fn
    0xBA   // Fn+Numlock
#elif (defined(KBD_8_18))
    0x97,  // Normal 		// Page down
    0xBA,  // Numlock
    0xBA,  // Fn
    0xBA   // Fn+Numlock
#endif
};
const BYTE ss2_A7[] =  // Numpad + or ;:
{
#ifdef KBD_8_16
    0x4C,  // Normal 		// ;:
    0xBB,  // Numlock
    0xBB,  // Fn
    0xBB   // Fn+Numlock
#elif (defined(KBD_8_18))
    0xBB,  // Normal 		// +
    0xBB,  // Numlock
    0xBB,  // Fn
    0xBB   // Fn+Numlock
#endif
};
const BYTE ss2_A8[] =  // Numpad 0 or M
{
#ifdef KBD_8_16
    0x3A,  // Normal 		// M
    0xBC,  // Numlock
    0xBC,  // Fn
    0xBC   // Fn+Numlock
#elif (defined(KBD_8_18))
    0xC2,  // Normal 		// Ins
    0xBC,  // Numlock
    0xBC,  // Fn
    0xBC   // Fn+Numlock
#endif
};
const BYTE ss2_A9[] =  // Numpad  or ,<
{
    0x41,  // Normal 		// ,<
    0x41,  // Numlock
    0x41,  // Fn
    0x41   // Fn+Numlock
};
const BYTE ss2_AA[] =  // Numpad . or .>
{
#ifdef KBD_8_16
    0x49,  // Normal 		// .>
    0xBD,  // Numlock
    0xBD,  // Fn
    0xBD   // Fn+Numlock
#elif (defined(KBD_8_18))
    0xC0,  // Normal 		// .
    0xBD,  // Numlock
    0xBD,  // Fn
    0xBD   // Fn+Numlock
#endif

};
const BYTE ss2_AB[] =  // Numpad / or /?
{
#ifdef KBD_8_16
    0x4A,  // Normal 		// /?
    0xBE,  // Numlock
    0xBE,  // Fn
    0xBE   // Fn+Numlock
#elif (defined(KBD_8_18))
    0xBE,  // Normal 		// /?
    0xBE,  // Numlock
    0xBE,  // Fn
    0xBE   // Fn+Numlock
#endif
};
const BYTE ss2_AC[] = { //Enter
    0x5A,  // Normal
    0x81,  // Fn
};
//----------------------------------------------------------------
// Function key F1 ~ F12
//----------------------------------------------------------------
const BYTE ss2_AD[] =  // F1
{
    0x05,  // Normal
    0xD0,  // Fn
};
const BYTE ss2_AE[] =  // F2
{
    0x06,  // Normal
    0xD1,  // Fn
};
const BYTE ss2_AF[] =  // F3
{
    0x04,  // Normal
    0xD2,  // Fn
};
const BYTE ss2_B0[] =  // F4
{
    0x0C,  // Normal
    0xD3,  // Fn
};
const BYTE ss2_B1[] =  // F5
{
    0x03,  // Normal
    0xD4,  // Fn
};
const BYTE ss2_B2[] =  // F6
{
    0x0B,  // Normal
    0xD5,  // Fn
};
const BYTE ss2_B3[] =  // F7
{
    0x80,  // Normal
    0xD6,  // Fn
};
const BYTE ss2_B4[] =  // F8
{
    0x0A,  // Normal
    0xD7,  // Fn
};
const BYTE ss2_B5[] =  // F9
{
    0x01,  // Normal
    0xD8,  // Fn
    // 0x01,   // Normal
};
const BYTE ss2_B6[] =  // F10
{
    0x09,  // Normal
    // 0x09,   // Fn
0xD9,  // Fn
// 0x09,   // Normal
};
const BYTE ss2_B7[] =  // F11
{
    0x78,  // Normal
    0xDA,  // Fn
    // 0x78,   // Normal
};
const BYTE ss2_B8[] =  // F12
{
    0x07,  // Normal
    0xDB,  // Fn
    // 0x07,
};
const BYTE ss2_B9[] =  // esc
{
    0x76,  // Normal
    0xDC,  // Fn
    // 0x76,
};
const BYTE ss2_BA[] =  // Up arrow
{
    0x75,  // Normal
    0xDD,  // Fn
    // 0x76,
};
const BYTE ss2_BB[] =  // Down arrow
{
    0x72,  // Normal
    0xDE,  // Fn
    // 0x76,
};
const BYTE ss2_BC[] =  // Left arrow
{
    0x6B,  // Normal
    0xDF,  // Fn
    // 0x76,
};
const BYTE ss2_BD[] =  // Right arrow
{
    0x74,  // Normal
    0xE0,  // Fn
    // 0x76,
};

// Reserved
const BYTE ss2_BE[] =  // Reserved
{
    0xBE,  // Normal
};
const BYTE ss2_BF[] =  // Reserved
{
    0xBF,  // Normal
};
const BYTE ss2_C0[] =  // Reserved
{
    0xC0,  // Normal
};
const BYTE ss2_C1[] =  // Reserved
{
    0xC1,  // Normal
};
const BYTE ss2_C2[] =  // Reserved
{
    0xC2,  // Normal
};
const BYTE ss2_C3[] =  // Reserved
{
    0xC3,  // Normal
};
const BYTE ss2_C4[] =  // Reserved
{
    0xC4,  // Normal
};
const BYTE ss2_C5[] =  // Reserved
{
    0xC5,  // Normal
};
const BYTE ss2_C6[] =  // Reserved
{
    0xC6,  // Normal
};
const BYTE ss2_C7[] =  // Reserved
{
    0xC7,  // Normal
};
const BYTE ss2_C8[] =  // Reserved
{
    0xC8,  // Normal
};
const BYTE ss2_C9[] =  // Reserved
{
    0xC9,  // Normal
};
const BYTE ss2_CA[] =  // Reserved
{
    0xCA,  // Normal
};
const BYTE ss2_CB[] =  // Reserved
{
    0xCB,  // Normal
};
const BYTE ss2_CC[] =  // Reserved
{
    0xCC,  // Normal
};
const BYTE ss2_CD[] =  // Reserved
{
    0xCD,  // Normal
};
const BYTE ss2_CE[] =  // Reserved
{
    0xCE,  // Normal
};
const BYTE ss2_CF[] =  // Reserved
{
    0xCF,  // Normal
};
const BYTE ss2_D0[] =  // Reserved
{
    0xD0,  // Normal
};
const BYTE ss2_D1[] =  // Reserved
{
    0xD1,  // Normal
};
const BYTE ss2_D2[] =  // Reserved
{
    0xD2,  // Normal
};
const BYTE ss2_D3[] =  // Reserved
{
    0xD3,  // Normal
};
const BYTE ss2_D4[] =  // Reserved
{
    0xD4,  // Normal
};
const BYTE ss2_D5[] =  // Reserved
{
    0xD5,  // Normal
};
const BYTE ss2_D6[] =  // Reserved
{
    0xD6,  // Normal
};
const BYTE ss2_D7[] =  // Reserved
{
    0xD7,  // Normal
};
const BYTE ss2_D8[] =  // Reserved
{
    0xD8,  // Normal
};
const BYTE ss2_D9[] =  // Reserved
{
    0xD9,  // Normal
};
const BYTE ss2_DA[] =  // Reserved
{
    0xDA,  // Normal
};
const BYTE ss2_DB[] =  // Reserved
{
    0xDB,  // Normal
};
const BYTE ss2_DC[] =  // Reserved
{
    0xDC,  // Normal
};
const BYTE ss2_DD[] =  // Reserved
{
    0xDD,  // Normal
};
const BYTE ss2_DE[] =  // Reserved
{
    0xDE,  // Normal
};
const BYTE ss2_DF[] =  // Reserved
{
    0xDF,  // Normal
};
/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_A2_table
 *
 * The A2_TABLE structure contains the following.
 *
 * BYTE  comb;  Bit   Setting Combination Key
 *            ---   -----------------------
 *             7    Scratch_Get_Index_Comb_BIT7
 *             6    Scratch_Get_Index_Comb_BIT6
 *             5    Fn
 *             4    Numlock
 *             3    Ctrl
 *             2    Alt
 *             1    Shift
 *             0    reserved
 *
 * BYTEP pntr; Pointer to array for key.
 * ------------------------------------------------------------------------- */
const A2_TABLE sskey2_A2_table[] = {
    // Keyboard matrix index
    // Keypad -->
    { 0x30, ss2_9C },
    /* index = 00h */  // 0x9C
    { 0x30, ss2_9D },
    /* index = 01h */  // 0x9D
    { 0x30, ss2_9E },
    /* index = 02h */  // 0x9E
    { 0x30, ss2_9F },
    /* index = 03h */  // 0x9F
    { 0x30, ss2_A0 },
    /* index = 04h */  // 0xA0
    { 0x30, ss2_A1 },
    /* index = 05h */  // 0xA1
    { 0x30, ss2_A2 },
    /* index = 06h */  // 0xA2
    { 0x30, ss2_A3 },
    /* index = 07h */  // 0xA3
    { 0x30, ss2_A4 },
    /* index = 08h */  // 0xA4
    { 0x30, ss2_A5 },
    /* index = 09h */  // 0xA5
    { 0x30, ss2_A6 },
    /* index = 0Ah */  // 0xA6
    { 0x30, ss2_A7 },
    /* index = 0Bh */  // 0xA7
    { 0x30, ss2_A8 },
    /* index = 0Ch */  // 0xA8
    { 0x30, ss2_A9 },
    /* index = 0Dh */  // 0xA9
    { 0x30, ss2_AA },
    /* index = 0Eh */  // 0xAA
    { 0x30, ss2_AB },
    /* index = 0Fh */  // 0xAB
    { 0x20, ss2_AC },
    /* index = 10h */  // 0xAC
                       // Keypad <--
    // F1~F12 hotkeys -->
    { 0x20, ss2_AD },
    /* index = 11h */  // 0xAD
    { 0x20, ss2_AE },
    /* index = 12h */  // 0xAE
    { 0x20, ss2_AF },
    /* index = 13h */  // 0xAF
    { 0x20, ss2_B0 },
    /* index = 14h */  // 0xB0
    { 0x20, ss2_B1 },
    /* index = 15h */  // 0xB1
    { 0x20, ss2_B2 },
    /* index = 16h */  // 0xB2
    { 0x20, ss2_B3 },
    /* index = 17h */  // 0xB3
    { 0x20, ss2_B4 },
    /* index = 18h */  // 0xB4
    { 0x20, ss2_B5 },
    /* index = 19h */  // 0xB5
    { 0x20, ss2_B6 },
    /* index = 1Ah */  // 0xB6
    { 0x20, ss2_B7 },
    /* index = 1Bh */  // 0xB7
    { 0x20, ss2_B8 },
    /* index = 1Ch */  // 0xB8
                        // F1~F12 keys <--
    { 0x20, ss2_B9 },   // ESC
    /* index = 1Dh */  // 0xB9

    // ARRORW hotkeys -->
    { 0x20, ss2_BA },
    /* index = 1Eh */  // 0xBA
    { 0x20, ss2_BB },
    /* index = 1Fh */  // 0xBB
    { 0x20, ss2_BC },
    /* index = 20h */  // 0xBC
    { 0x20, ss2_BD },
    /* index = 21h */  // 0xBD
                        // ARRORW  keys <--
// Reserved 0xBE ~ 0xDF
    { 0x00, ss2_BE },
    /* index = 22h */ // 0xBE
    { 0x00, ss2_BF },
    /* index = 23h */ // 0xBF
    { 0x00, ss2_C0 },
    /* index = 24h */ // 0xC0
    { 0x00, ss2_C1 },
    /* index = 25h */ // 0xC1
    { 0x00, ss2_C2 },
    /* index = 26h */ // 0xC2
    { 0x00, ss2_C3},
    /* index = 27h */ // 0xC3
    { 0x00, ss2_C4 },
    /* index = 28h */ // 0xC4
    { 0x00, ss2_C5 },
    /* index = 29h */ // 0xC5
    { 0x00, ss2_C6 },
    /* index = 2Ah */ // 0xC6
    { 0x00, ss2_C7 },
    /* index = 2Bh */ // 0xC7
    { 0x00, ss2_C8 },
    /* index = 2Ch */ // 0xC8
    { 0x00, ss2_C9 },
    /* index = 2Dh */ // 0xC9
    { 0x00, ss2_CA },
    /* index = 2Eh */ // 0xCA
    { 0x00, ss2_CB },
    /* index = 2Fh */ // 0xCB
    { 0x00, ss2_CC },
    /* index = 30h */ // 0xCC
    { 0x00, ss2_CD },
    /* index = 31h */ // 0xCD
    { 0x00, ss2_CE },
    /* index = 32h */ // 0xCE
    { 0x00, ss2_CF },
    /* index = 33h */ // 0xCF
    { 0x00, ss2_D0 },
    /* index = 34h */ // 0xD0
    { 0x00, ss2_D1 },
    /* index = 35h */ // 0xD1
    { 0x00, ss2_D2 },
    /* index = 36h */ // 0xD2
    { 0x00, ss2_D3},
    /* index = 37h */ // 0xD3
    { 0x00, ss2_D4 },
    /* index = 38h */ // 0xD4
    { 0x00, ss2_D5 },
    /* index = 39h */ // 0xD5
    { 0x00, ss2_D6 },
    /* index = 3Ah */ // 0xD6
    { 0x00, ss2_D7 },
    /* index = 3Bh */ // 0xD7
    { 0x00, ss2_D8 },
    /* index = 3Ch */ // 0xD8
    { 0x00, ss2_D9 },
    /* index = 3Dh */ // 0xD9
    { 0x00, ss2_DA },
    /* index = 3Eh */ // 0xDA
    { 0x00, ss2_DB },
    /* index = 3Fh */ // 0xDB
    { 0x00, ss2_DC },
    /* index = 40h */ // 0xDC
    { 0x00, ss2_DD },
    /* index = 41h */ // 0xDD
    { 0x00, ss2_DE },
    /* index = 42h */ // 0xDE
    { 0x00, ss2_DF },
    /* index = 43h */ // 0xDF

};
/* ----------------------------------------------------------------------------
 * FUNCTION: sskey2_overlay_table
 * ------------------------------------------------------------------------- */
const BYTE sskey2_overlay_table[] = {
    //	Normal  Fn   // Keyboard matrix index
        0x94, 0x94,  // 0xE0	Home	Home
        0x95, 0x95,  // 0xE1	End	    End
        0x96, 0x96,  // 0xE2	PGUP	PGUP
        0x97, 0x97,  // 0xE3	PGDN	PGDN

        0x98, 0x96,  // 0xE4	↑	PGUP
        0x99, 0x97,  // 0xE5	↓	PGDN
        0x9A, 0x94,  // 0xE6	←	Home
        0x9B, 0x95,  // 0xE7	→ 	END

        0x9C, 0x9C,  // 0xE8	Next	Next
        0x9D, 0x9D,  // 0xE9	Pre	    Pre
        0x9E, 0x9E,  // 0xEA	Stop	Stop
        0x9F, 0x9F,  // 0xEB	Play/Pause Play/Pause
        0xA0, 0xA0,  // 0xEC	Mute    Mute
        0xA1, 0xA1,  // 0xED	Volume Up   Volume Up
        0xA2, 0xA2,  // 0xEE    Volume Down Volume Down
        0xA3, 0xA3,  // 0xEF	Mail        Mail
        0xA4, 0xA4,  // 0xF0    Search      Search
        0xA5, 0xA5,  // 0xF1	Web/Home    Web/Home
        0xA6, 0xA6,  // 0xF2	Back        Back
        0xA7, 0xA7,  // 0xF3	Forward	    Forward
        0xA8, 0xA8,  // 0xF4	wwwStop	    wwwStop
        0xA9, 0xA9,  // 0xF5	Refresh	    Refresh
        0xAA, 0xAA,  // 0xF6	Favorites   Favorites
        0xAB, 0xAB,  // 0xF7	Caluator    Caluator
        0xAC, 0xAC,  // 0xF8	My Computer My Computer
        0xAD, 0xAD,  // 0xF9	Media       Media

        0xC0, 0xC0,  // 0xFA	Delete       Delete
        0xC1, 0x93,  // 0xFB	SysRq        (124) Alt-Case (SysRq)
        0xC2, 0xC2,  // 0xFC	Insert       Insert
        0xC3, 0xC3,  // 0xFD	Print Screen Print Screen
        0xC4, 0xC4,  // 0xFE	Euro Sign    Euro Sign
        0xC5, 0xC5,  // 0xFF	Dollar Sign  Dollar Sign

};
// Reserved 0xEE ~ 0xFF
/* ----------------------------------------------------------------------------
 * Sparkle Silicon Software Key Number 2
 *
 * 00h         Null
 * 01h - 7Fh   Same as IBM Scan Code, Set2
 * 80h - BFh   Pre-Index for generation Scan Code, Set2
 * C0h - DFh   Pre-Index for PPK function
 * C0h - DFh   Pre-Index for SMI function
 * ------------------------------------------------------------------------- */
 /* ----------------------------------------------------------------------------
  * FUNCTION: sskey3_80_table - Sparkle Silicon Software Key Number 2
  * 80h - BFh   Pre-Index for generation Scan Code, Set2
  * ------------------------------------------------------------------------- */
const BYTE sskey3_80_table[] = {
    //-----------------------------------------------------------------
    // Index from keyboard matrix
    //-----------------------------------------------------------------
    // Keyboard matrix index
    0x83, DO_SIMPLE_CODE,         // [80h] F7 (118)
    0x5A, DO_E0_PREFIX_CODE,      // [81h] Numpad Enter (108)
    0x1F, DO_E0_PREFIX_CODE,      // [82h] Windows Left
    0x27, DO_E0_PREFIX_CODE,      // [83h] Windows Right
    0x2F, DO_E0_PREFIX_CODE,      // [84h] Application
    0x37, DO_E0_PREFIX_CODE,      // [85h] Power event
    0x3F, DO_E0_PREFIX_CODE,      // [86h] Sleep event
    0x5E, DO_E0_PREFIX_CODE,      // [87h] Wake event
    LShift, DO_CONTROL_EFFECT,    // [88h] 12 Shift(L)
    RShift, DO_CONTROL_EFFECT,    // [89h] 59 Shift(R)
    LAlt, DO_CONTROL_EFFECT,      // [8Ah] 11 Alt(L)
    RAlt, DO_CONTROL_EFFECT_E0,   // [8Bh] E0 11 Alt(R)
    LCtrl, DO_CONTROL_EFFECT,     // [8Ch] 14 Crtl(L)
    RCtrl, DO_CONTROL_EFFECT_E0,  // [8Dh] E0 14 Ctrl(R)
    FN, DO_CONTROL_EFFECT_E0,     // [8Eh] Fn
    OVLAY, DO_CONTROL_EFFECT,     // [8Fh] Overlay
    NUM_LOCK, DO_CONTROL_EFFECT,  // [90h] NUM LOCK
    0x00, DO_SPECIAL_CODE,        // [91h] Pause (126)
    0x01, DO_SPECIAL_CODE,        // [92h] Break
    0x84, DO_SIMPLE_CODE,         // [93h] (124) Alt-Case (SysRq)
    0x6C, DO_E0_PREFIX_CODE,      // [94h] Home
    0x69, DO_E0_PREFIX_CODE,      // [95h] End
    0x7D, DO_E0_PREFIX_CODE,      // [96h] Page up
    0x7A, DO_E0_PREFIX_CODE,      // [97h] Page down
    0x75, DO_E0_PREFIX_CODE,      // [98h] UP arrow
    0x72, DO_E0_PREFIX_CODE,      // [99h] Down arrow
    0x6B, DO_E0_PREFIX_CODE,      // [9Ah] Left arrow
    0x74, DO_E0_PREFIX_CODE,      // [9Bh] Right arrow
    //-----------------------------------------------------------------
    // For MicroSoft enhance keyboard feature.
    //-----------------------------------------------------------------
    0x4D, DO_E0_PREFIX_CODE,  // [9Ch] Next Track event
    0x15, DO_E0_PREFIX_CODE,  // [9Dh] Prev Track event
    0x3B, DO_E0_PREFIX_CODE,  // [9Eh] Stop event
    0x34, DO_E0_PREFIX_CODE,  // [9Fh] Play/Pause event
    0x23, DO_E0_PREFIX_CODE,  // [A0h] Mute event
    0x32, DO_E0_PREFIX_CODE,  // [A1h] Volume Up event
    0x21, DO_E0_PREFIX_CODE,  // [A2h] Volume Down event
    0x48, DO_E0_PREFIX_CODE,  // [A3h] Mail event
    0x10, DO_E0_PREFIX_CODE,  // [A4h] Search event
    0x3A, DO_E0_PREFIX_CODE,  // [A5h] Web/Home event
    0x38, DO_E0_PREFIX_CODE,  // [A6h] Back event
    0x30, DO_E0_PREFIX_CODE,  // [A7h] Forward event
    0x28, DO_E0_PREFIX_CODE,  // [A8h] Stop event
    0x20, DO_E0_PREFIX_CODE,  // [A9h] Refresh event
    0x18, DO_E0_PREFIX_CODE,  // [AAh] Favorites event
    0x2B, DO_E0_PREFIX_CODE,  // [ABh] Caluator event
    0x40, DO_E0_PREFIX_CODE,  // [ACh] My Computer event
    0x50, DO_E0_PREFIX_CODE,  // [ADh] Media event
    0x00, DO_SIMPLE_CODE,     // [AEh] Reserved
    0x00, DO_SIMPLE_CODE,     // [AFh] Reserved
    //-----------------------------------------------------------------
    // Index from sskey2_A2_table
    //-----------------------------------------------------------------
    0x6C, DO_SIMPLE_CODE,     // Numpad 7 Home	[B0h]
    0x75, DO_SIMPLE_CODE,     // Numpad 8 UP  	[B1h]
    0x7D, DO_SIMPLE_CODE,     // Numpad 9 PGUp	[B2h]
    0x7C, DO_SIMPLE_CODE,     // Numpad *   		[B3h]
    0x6B, DO_SIMPLE_CODE,     // Numpad 4 Left	[B4h]
    0x73, DO_SIMPLE_CODE,     // Numpad 5 		[B5h]
    0x74, DO_SIMPLE_CODE,     // Numpad 6 Right	[B6h]
    0x7B, DO_SIMPLE_CODE,     // Numpad -  		[B7h]
    0x69, DO_SIMPLE_CODE,     // Numpad 1 End		[B8h]
    0x72, DO_SIMPLE_CODE,     // Numpad 2 Down	[B9h]
    0x7A, DO_SIMPLE_CODE,     // Numpad 3 PGDn	[BAh]
    0x79, DO_SIMPLE_CODE,     // Numpad + 		[BBh]
    0x70, DO_SIMPLE_CODE,     // Numpad 0 Ins		[BCh]
    0x71, DO_SIMPLE_CODE,     // Numpad . Del		[BDh]
    0x4A, DO_E0_PREFIX_CODE,  // Numpad /      	[BEh]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[BFh]
    0x71, DO_E0_PREFIX_CODE,  // Delete 			[C0h]
    0x84, DO_SIMPLE_CODE,     // SysRq			[C1h]
    0x70, DO_E0_PREFIX_CODE,  // Insert			[C2h]
#if 0
    0x02, DO_SPECIAL_CODE,    // Print Screen		[C3h]
#else
    0x7C, DO_E0_PREFIX_CODE,  // Print Screen		[C3h]
#endif
    0x41, DO_E0_PREFIX_CODE,  // Euro Sign 		[C4h]
    0x49, DO_E0_PREFIX_CODE,  // Dollar Sign 		[C5h]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[C6h]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[C7h]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[C8h]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[C9h]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[CAh]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[CBh]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[CCh]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[CDh]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[CEh]
    0x00, DO_SIMPLE_CODE,     // Reserved 		[CFh]
    // Function key F1 ~ F12
    0, DO_COSTOMER_FUNCTION,   // Function key F1	[D0h]
    1, DO_COSTOMER_FUNCTION,   // Function key F2	[D1h]
    2, DO_COSTOMER_FUNCTION,   // Function key F3	[D2h]
    3, DO_COSTOMER_FUNCTION,   // Function key F4	[D3h]
    4, DO_COSTOMER_FUNCTION,   // Function key F5	[D4h]
    5, DO_COSTOMER_FUNCTION,   // Function key F6	[D5h]
    6, DO_COSTOMER_FUNCTION,   // Function key F7	[D6h]
    7, DO_COSTOMER_FUNCTION,   // Function key F8	[D7h]
    8, DO_COSTOMER_FUNCTION,   // Function key F9	[D8h]
    9, DO_COSTOMER_FUNCTION,   // Function key F10	[D9h]
    10, DO_COSTOMER_FUNCTION,  // Function key F11	[DAh]
    11, DO_COSTOMER_FUNCTION,  // Function key F12	[DBh]
    12, DO_COSTOMER_FUNCTION,  // Function key Esc	[DCh]
    13, DO_COSTOMER_FUNCTION,  // Function key Up arrow	[DDh]
    14, DO_COSTOMER_FUNCTION,  // Function key Down arrow	[DEh]
    15, DO_COSTOMER_FUNCTION,  // Function key Left arrow	[DFh]
    16, DO_COSTOMER_FUNCTION,  // Function key Right arrow	[E0h]
};
// Reserved 0xE0 ~ 0xFF
//-----------------------------------------------------------------
// The function of checking boot hotkey
//-----------------------------------------------------------------
#define Crisiskey01 0x8E  // Fn
#define Crisiskey02 0xEC  // ESC
void Check_HotKey_01(BYTE event)
{
    if (event == BREAK_EVENT)
    {
        CRISIS_KEY_STATUS = 0x00;
    }
    else if (event == MAKE_EVENT)
    {
        CRISIS_KEY_STATUS++;
    }
}
void Check_HotKey_02(BYTE event)
{
    if (event == BREAK_EVENT)
    {
        CRISIS_KEY_STATUS = 0x00;
    }
    else if (event == MAKE_EVENT)
    {
        CRISIS_KEY_STATUS++;
    }
}
#if 1
const sBootHotKeyStruct asBootHotKeyStruct[] = {
    { Crisiskey01, Check_HotKey_01 },
    { Crisiskey02, Check_HotKey_02 },
};
#endif
void Check_HotKey_Boot(BYTE matrix, BYTE event)
{
    BYTE index;
    for (index = 0x00; index < (sizeof(asBootHotKeyStruct) / sizeof(sBootHotKeyStruct)); index++)
    {
        if (matrix == asBootHotKeyStruct[index].matrix)
        {
            (asBootHotKeyStruct[index].pfunction)(event);
            return;
        }
    }
    CRISIS_KEY_STATUS = 0x00;
}
//-----------------------------------------------------------------------------
// Keyboard wake up system from S3
//-----------------------------------------------------------------------------
void Check_KB_Wake_S3(BYTE KSO, BYTE KSI)
{
    if (KB_S3WakeUP_Tables[KSO][KSI] == 0x55)
    {
        PowerSequence_Step = 1;
        PowerSequence_Delay = 0x00;
        System_PowerState = SYSTEM_S3_S0;
    }
}
//----------------------------------------------------------------------------
// table_entry : value of keyboard matrix table. for example Cr_KBD_Tables[]
// event : key MAKE_EVENT or BREAK_EVENT or REPEAT_EVENT
//----------------------------------------------------------------------------
void Check_Send_Key(BYTE table_entry, BYTE event)
{
#if 0
    if (SystemNotS0)
    {
        Check_HotKey_Boot(table_entry, event);
    }
#else
    UNUSED_VAR(table_entry); UNUSED_VAR(event);
#endif
}
//----------------------------------------------------------------------------
// Return : 0xFF --> Transmit_Key function will be break; (no any KBD_SCAN code to host)
//----------------------------------------------------------------------------
BYTE Skip_Send_Key(void)
{
    return (0x00);
}
//-----------------------------------------------------------------------------
// The function of setting GPIO KBD_SCAN pin to high
//-----------------------------------------------------------------------------
void Set_GPIO_ScanPin_To_H(void) {}
//-----------------------------------------------------------------------------
// The function of setting GPIO KBD_SCAN pin to low
//-----------------------------------------------------------------------------
void Set_GPIO_ScanPin_To_L(void) {}
//-----------------------------------------------------------------------------
// The function of setting GPIO KBD_SCAN pin output
//-----------------------------------------------------------------------------
void Set_GPIO_ScanPin_Output(void) {}
//-----------------------------------------------------------------------------
// Fn key make
//-----------------------------------------------------------------------------
void Scratch_Fn_Key_Make(void) {}
//-----------------------------------------------------------------------------
// Fn key break
//-----------------------------------------------------------------------------
void Scratch_Fn_Key_Break(void) {}
//-----------------------------------------------------------------------------
// NUM LOCK  key make
//-----------------------------------------------------------------------------
void Scratch_NUMLK_Key_Make(void) {}
//-----------------------------------------------------------------------------
// NUM LOCK key break
//-----------------------------------------------------------------------------
void Scratch_NUMLK_Key_Break(void)
{
    NumLockKey = !NumLockKey;
}
//-----------------------------------------------------------------------------
// Return : 0xFF --> sskey2_A2_table contains bit6
//-----------------------------------------------------------------------------
BYTE Scratch_Get_Index_Comb_BIT6(void)
{
    return (0x00);
}
//-----------------------------------------------------------------------------
// Return : 0xFF --> sskey2_A2_table contains bit7
//-----------------------------------------------------------------------------
BYTE Scratch_Get_Index_Comb_BIT7(void)
{
    return (0x00);
}