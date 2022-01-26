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
 *
 *      Name:    Platform.c
 *      Purpose: Platform template
 *
 *---------------------------------------------------------------------------*/
#include "M480.h"                       // Device header
#include "cmsis_os2.h"

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

static const osThreadAttr_t app_main_attr = {
  .stack_size = 4096U
};

/*---------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
static void app_main (void *argument) {
  (void)argument;

  // Add user code here:
  for (;;) {}
}

/*---------------------------------------------------------------------------
 * Application initialization
 *---------------------------------------------------------------------------*/
void app_initialize (void) {
  osThreadNew(app_main, NULL, &app_main_attr);
}