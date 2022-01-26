/**************************************************************************//**
 * @file     main.c
 * @version  V3.0
 * @brief    Configure QSPI0 as Slave 3 wire mode and demonstrate how to
 *           communicate with an off-chip SPI Master device with FIFO mode.
 *           This sample code needs to work with SPI_MasterFifoMode sample code.
 *
 *
 * @copyright (C) 2019 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include "NuMicro.h"

#define TEST_COUNT 16
#define PLL_CLOCK   192000000

uint32_t g_au32SourceData[TEST_COUNT];
uint32_t g_au32DestinationData[TEST_COUNT];
volatile uint32_t g_u32TxDataCount;
volatile uint32_t g_u32RxDataCount;

void SYS_Init(void)
{
    /* Set XT1_OUT(PF.2) and XT1_IN(PF.3) to input mode */
    PF->MODE &= ~(GPIO_MODE_MODE2_Msk | GPIO_MODE_MODE3_Msk);

    /* Enable HXT clock (external XTAL 12MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Wait for HXT clock ready */
    CLK_WaitClockReady(CLK_STATUS_HXTSTB_Msk);

    /* Set core clock as PLL_CLOCK from PLL */
    CLK_SetCoreClock(FREQ_192MHZ);

    /* Set both PCLK0 and PCLK1 as HCLK/2 */
    CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2;

    /* Select UART module clock source as HXT and UART module clock divider as 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HXT, CLK_CLKDIV0_UART0(1));

    /* Select PCLK0 as the clock source of QSPI0 */
    CLK_SetModuleClock(QSPI0_MODULE, CLK_CLKSEL2_QSPI0SEL_PCLK0, MODULE_NoMsk);

    /* Enable UART peripheral clock */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Enable QSPI0 peripheral clock */
    CLK_EnableModuleClock(QSPI0_MODULE);

    /* Set GPB multi-function pins for UART0 RXD and TXD */
    SYS->GPB_MFPH &= ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk);
    SYS->GPB_MFPH |= (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    /* Setup QSPI0 multi-function pins */
    SYS->GPA_MFPL |= SYS_GPA_MFPL_PA0MFP_QSPI0_MOSI0 | SYS_GPA_MFPL_PA1MFP_QSPI0_MISO0 | SYS_GPA_MFPL_PA2MFP_QSPI0_CLK;

    /* Enable QSPI0 clock pin (PA2) schmitt trigger */
    PA->SMTEN |= GPIO_SMTEN_SMTEN2_Msk;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and CyclesPerUs automatically. */
    SystemCoreClockUpdate();
}

void QSPI_Init(void)
{

    /* Configure as a slave, clock idle low, 32-bit transaction, drive output on falling clock edge and latch input on rising edge. */
    /* Configure QSPI0 as a low level active device. */
    QSPI_Open(QSPI0, SPI_SLAVE, SPI_MODE_0, 32, (uint32_t)NULL);

    /* Enable slave 3 wire mode */
    QSPI0->SSCTL |= QSPI_SSCTL_SLV3WIRE_Msk;
}

int main(void)
{
    volatile uint32_t u32TxDataCount, u32RxDataCount;

    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Init System, IP clock and multi-function I/O. */
    SYS_Init();

    /* Configure UART0: 115200, 8-bit word, no parity bit, 1 stop bit. */
    UART_Open(UART0, 115200);

    /* Init QSPI */
    QSPI_Init();

    printf("\n\n");
    printf("+----------------------------------------------------------------------+\n");
    printf("|           QSPI0 Slave 3 Wire Mode Sample Code                        |\n");
    printf("+----------------------------------------------------------------------+\n");
    printf("\n");
    printf("Configure QSPI0 as a slave.\n");
    printf("Bit length of a transaction: 32\n");
    printf("The I/O connection for QSPI0:\n");
    printf("    QSPI0_CLK(PA2)\n    QSPI0_MISO(PA1)\n    QSPI0_MOSI(PA0)\n\n");
    printf("SPI controller will enable FIFO mode and transfer %d data to a off-chip master device.\n", TEST_COUNT);
    printf("In the meanwhile the SPI controller will receive %d data from the off-chip master device.\n", TEST_COUNT);
    printf("After the transfer is done, the %d received data will be printed out.\n", TEST_COUNT);

    for(u32TxDataCount = 0; u32TxDataCount < TEST_COUNT; u32TxDataCount++)
    {
        /* Write the initial value to source buffer */
        g_au32SourceData[u32TxDataCount] = 0x00AA0000 + u32TxDataCount;
        /* Clear destination buffer */
        g_au32DestinationData[u32TxDataCount] = 0;
    }

    u32TxDataCount = 0;
    u32RxDataCount = 0;
    printf("Press any key if the master device configuration is ready.\n");
    getchar();
    printf("\n");

    /* Set TX FIFO threshold and enable FIFO mode. */
    QSPI_SetFIFO(QSPI0, 4, 4);

    /* Access TX and RX FIFO */
    while(u32RxDataCount < TEST_COUNT)
    {
        /* Check TX FULL flag and TX data count */
        if((QSPI_GET_TX_FIFO_FULL_FLAG(QSPI0) == 0) && (u32TxDataCount < TEST_COUNT))
            QSPI_WRITE_TX(QSPI0, g_au32SourceData[u32TxDataCount++]); /* Write to TX FIFO */
        /* Check RX EMPTY flag */
        if(QSPI_GET_RX_FIFO_EMPTY_FLAG(QSPI0) == 0)
            g_au32DestinationData[u32RxDataCount++] = QSPI_READ_RX(QSPI0); /* Read RX FIFO */
    }

    /* Print the received data */
    printf("Received data:\n");
    for(u32RxDataCount = 0; u32RxDataCount < TEST_COUNT; u32RxDataCount++)
    {
        printf("%d:\t0x%X\n", u32RxDataCount, g_au32DestinationData[u32RxDataCount]);
    }
    printf("The data transfer was done.\n");

    printf("\n\nExit QSPI driver sample code.\n");

    /* Reset QSPI0 */
    QSPI_Close(QSPI0);
    while(1);
}

/*** (C) COPYRIGHT 2019 Nuvoton Technology Corp. ***/
