#include "drv_l1_system.h"

static INT32S iRAM_Delay(INT32U cnt)
{
    INT32U i;

    for (i = 0; i < cnt; ++i)
    {
        __asm {NOP};
    }
    return 0;
}

////////////////
// ICE  clock ㄓ方琌 SYSCLK
// ICE 璶铆﹚ SYSCLK / ICE_CLK > 7
// ㄓ SYSCLK=144MHz,  ICE LCK=4MHz
// 秈 SLOW mode SYSCLK=12MHz┮ ICE clk ぃ禬筁 1.7M Hz
// ICE 1.3M Hz ち繵奔
// ち场 XTAL 12M
void system_clk_ext_XLAT_12M(void)
{
    R_SYSTEM_MISC_CTRL1 |= 0x01;
    R_SYSTEM_CKGEN_CTRL |= 0x14C;

    {
        INT32U i;
        for (i = 0; i < 0xF000; ++i)
        {
            R_RANDOM0 = i;		// delay for XLAT clock stable
        }

    }

    R_SYSTEM_CLK_CTRL &= 0x3fff;		//enter SLOW mode

    while ( (R_SYSTEM_POWER_STATE & 0xF) != 1 )
    {
        //DBG_PRINT("wait goint to SLOW mode\r\n");
        ;
    }

    R_SYSTEM_CKGEN_CTRL |= 0x21;
    R_SYSTEM_CLK_CTRL |= 0x8000;		//enter FAST mode again


    while ( (R_SYSTEM_POWER_STATE & 0xF) != 2 )
    {
        //DBG_PRINT("wait coming back to FAST mode\r\n");
        ;
    }

    R_SYSTEM_CTRL |= 0x00000902;  // josephhsieh@140519 sensor 玡狠㏕﹚48MHz
    R_SYSTEM_CKGEN_CTRL |= 0x1F;
}

void setting_by_iRAM(void)
{
    __asm { NOP };
}

void system_clk_alter(INT32U SysClk, INT32U SDramPara)
{
    // switch system clock in TFT Vblanking  (ISR)
    SysClk |= (R_SYSTEM_PLLEN & (~0x1F));
    R_MEM_SDRAM_CTRL0 = SDramPara;
    R_SYSTEM_PLLEN =  SysClk;

    iRAM_Delay(16);
}

void system_clk_set(INT8U CPUCLK )
{

    // SDRAM Timing
    if(CPUCLK == 48)
        R_MEM_SDRAM_CTRL0 = 0x0011;	// SDRAM 把计
    else
        R_MEM_SDRAM_CTRL0 = 0x0711;	// SDRAM 把计
    R_MEM_SDRAM_CTRL1 = 0x2000;	// SDRAM 把计
    R_MEM_SDRAM_CBRCYC = 0x8CA;	// SDRAM 把计

    /*PLL setting*/
    while (R_SYSTEM_POWER_STATE == 0)
    {
        ;;   //waiting stable
    }

    R_SYSTEM_CLK_CTRL &= ~(0x8008); // Fast PLL disable, Entry Slow Mode

    (*((volatile INT32U *) 0xF8500000)) = (*((volatile INT32U *) 0xFF000001));//DUMMY OP
    (*((volatile INT32U *) 0xF8500000)) = (*((volatile INT32U *) 0xFF000001));//DUMMY OP

    while (R_SYSTEM_POWER_STATE == 0)
    {
        ;;   //waiting slow mode stable
    }

    {
        // Reset XCKGEN
        (*((volatile INT32U *) 0xD0000058)) &= ~0x1;
        iRAM_Delay(0);
        (*((volatile INT32U *) 0xD0000058)) |=  0x1;
        // Wait stable ...
        while ( ( (*((volatile INT32U *) 0xD0000058)) & 0x8000 ) == 0   )
        {
            __asm {NOP};
        }
        iRAM_Delay(100);
    }
    {
        // change clk
        if(CPUCLK == 48)
            R_SYSTEM_PLLEN = 0x02;		// CPU = 48MHz
        else if(CPUCLK == 96)
            R_SYSTEM_PLLEN = 0x0e;		// CPU = 96MHz
        else
            R_SYSTEM_PLLEN = 0x1a;		// CPU = 144MHz
        iRAM_Delay(50);
        R_SYSTEM_CLK_CTRL |= 0x8008; // Fast PLL ENABLE
    }

    (*((volatile INT32U *) 0xF8500000)) = (*((volatile INT32U *) 0xFF000001));//DUMMY OP
    (*((volatile INT32U *) 0xF8500000)) = (*((volatile INT32U *) 0xFF000001));//DUMMY OP
    while (R_SYSTEM_POWER_STATE == 0)
    {
        ;;   //waiting fast mode stable
    }

}

