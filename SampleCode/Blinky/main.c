/*---------------------------------------------------------------------------
 * Copyright (c) 2020 Arm Limited (or its affiliates). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *---------------------------------------------------------------------------*/

#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#ifdef RTE_VIO_BOARD
#include "cmsis_vio.h"
#endif
#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif

#include "gpio.h"                       // MDK-Packs.M80::Board Support:GPIO
#include "main.h"

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

int main (void) {

  /* Init System, peripheral clock and multi-function I/O */
  SYS_Init();

#ifdef RTE_VIO_BOARD
  vioInit();                            // Initialize Virtual I/O
#endif

#if defined(RTE_Compiler_EventRecorder) && \
    (defined(__MICROLIB) || \
    !(defined(RTE_CMSIS_RTOS2_RTX5) || defined(RTE_CMSIS_RTOS2_FreeRTOS)))
  EventRecorderInitialize(EventRecordAll, 1U);
#endif

  osKernelInitialize();                 // Initialize CMSIS-RTOS2
  app_initialize();                     // Initialize application
  osKernelStart();                      // Start thread execution

  for (;;) {}
}
