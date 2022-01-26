#include "NuMicro.h" // Device header
#include "gpio.h"
#include "cmsis_vio.h"                  // ::CMSIS Driver:VIO

#define PLL_CLOCK 192000000

void SYS_Init(void)
{

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(PLL_CLOCK);

    /* Set PCLK0/PCLK1 to HCLK/2 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);

    /* Lock protected registers */
    SYS_LockReg();
}

static void delay_ms(uint16_t nDelay)
{
    uint16_t nIndex;
    for (nIndex = 0; nIndex < nDelay; nIndex++)
    {
        CLK_SysTickDelay(1000); //delay one ms
    }
}

/*----------------------------------------------------------------------------
  Main function
 *----------------------------------------------------------------------------*/
int main(void)
{

    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();
	vioInit();

	
#ifdef M487KMCAN
	// this is the blinky code for NuMaker-M487KMCAN
//    GPIO_SetMode(PH, BIT4, GPIO_MODE_OUTPUT);
//    GPIO_SetMode(PH, BIT5, GPIO_MODE_OUTPUT);
    while (1)
    {
      vioSetSignal(vioLED0, vioLEDoff);         // Switch LED0 off
      vioSetSignal(vioLED1, vioLEDon);          // Switch LED1 on
        delay_ms(1000);
      vioSetSignal(vioLED0, vioLEDon);          // Switch LED0 on
      vioSetSignal(vioLED1, vioLEDoff);         // Switch LED1 off
//        PH4 = 0;
//        PH5 = 1;
//        delay_ms(1000);
//        PH4 = 1;
//        PH5 = 0;
        delay_ms(1000);
    }
#else
	// this is the blinky code for NuMaker-PFM-M487
    GPIO_SetMode(PH, BIT0, GPIO_MODE_OUTPUT);
    GPIO_SetMode(PH, BIT2, GPIO_MODE_OUTPUT);
    while (1)
    {
        PH0 = 0;
        PH2 = 1;
        delay_ms(1000);
        PH0 = 1;
        PH2 = 0;
        delay_ms(1000);
    }
#endif
}
