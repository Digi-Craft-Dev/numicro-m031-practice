/**************************************************************************//**
 * @file     main.c
 * @version  V1.00
 * $Revision: 10 $
 * $Date: 18/07/17 6:05p $
 * @brief
 *           Change duty cycle of output waveform to show different brightness of
                         Red LED.
 *
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "NuMicro.h"

/*---------------------------------------------------------------------------------------------------------*/
/* Macro, type and constant definitions                                                                    */
/*---------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/

void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Enable HIRC clock */
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    /* Waiting for HIRC clock ready */
    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    /* Switch HCLK clock source to HIRC and HCLK source divide 1 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    /* Enable PWM1 module clock */
    CLK_EnableModuleClock(PWM1_MODULE);

    /* Enable UART0 module clock for the default UART output */
    CLK_EnableModuleClock(UART0_MODULE);

    /* Use HIRC as UART0 clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /*---------------------------------------------------------------------------------------------------------*/
    /* PWM clock frequency configuration                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PLL clock as 96 MHz from HIRC/4 */
    CLK_EnablePLL(CLK_PLLCTL_PLLSRC_HIRC_DIV4, 96000000);

    /* Waiting for PLL clock ready */
    CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk);

    /* Select HCLK clock source as PLL and and HCLK clock divider as 2 */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL, CLK_CLKDIV0_HCLK(2));

    /* PWM clock frequency can be set equal or double to HCLK by choosing case 1 or case 2 */
    /* case 1.PWM clock frequency is set equal to HCLK: select PWM module clock source as PCLK */
//    CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL2_PWM1SEL_PCLK0, 0);

    /* case 2.PWM clock frequency is set double to HCLK: select PWM module clock source as PLL */
    CLK_SetModuleClock(PWM1_MODULE, CLK_CLKSEL2_PWM1SEL_PLL, (uint32_t)NULL);
    /*---------------------------------------------------------------------------------------------------------*/

    /* Reset PWM1 module */
    SYS_ResetModule(PWM1_RST);

    /* Update System Core Clock */
    SystemCoreClockUpdate();

    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set PB.14 multi-function pins for PWM1 Channel 1 for Red of LED */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & (~SYS_GPB_MFPH_PB14MFP_Msk)) |
                    SYS_GPB_MFPH_PB14MFP_PWM1_CH1;

    /* Set PB.12/PB.13 multi-function pins for default UART0 RXD/TXD */
    SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk |
                                       SYS_GPB_MFPH_PB13MFP_Msk)) |
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD |
                     SYS_GPB_MFPH_PB13MFP_UART0_TXD);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  UART0 Initialization                                                                                   */
/*---------------------------------------------------------------------------------------------------------*/
void UART0_Init(void)
{
    /* Reset UART0 and configure it for printf output at 115200 8-N-1. */
    SYS_ResetModule(UART0_RST);
    UART_Open(UART0, 115200);
}

static void UART0_PrintDuty(uint32_t duty)
{
    char message[32] = {0};
    uint32_t length = 0;
    const size_t size = sizeof(message);

    length = snprintf(message, size, "PWM1 CH1 duty = %lu%%\n", duty);
    if (length > 0)
        UART_Write(UART0, (uint8_t *)message, length);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int main(void)
{
    const uint32_t step = 5;
    bool direction = false;
    uint32_t duty = 30;

    /* Unlock protected registers */
    SYS_UnlockReg();
    /* Init System, IP clock and multi-function I/O */
    SYS_Init();

    /* Init default UART0 for printf output before locking protected registers */
    UART0_Init();
    UART_Write(UART0, (uint8_t *)"UART0 ready\n", 13);

    /* Lock protected registers */
    SYS_LockReg();

    /* Set frequency and duty of PWM1 Channel 1 to control Red LED */
    PWM_ConfigOutputChannel(PWM1, 1, 1000, duty);
    /* Enable PWM1 Output path for channel 1 */
    PWM_EnableOutput(PWM1, BIT1);
    /* Start PWM1 Counter */
    PWM_Start(PWM1, BIT1);

    UART0_PrintDuty(duty);

    /* Make the LED connected to PB.14 breathing on NuMaker-M031SE */
    while(1)
    {
        if (duty < 20 || duty >= 100)
            direction = !direction;
        if (direction)
            duty += step;
        else
            duty -= step;

        PWM_ConfigOutputChannel(PWM1, 1, 1000, duty);
        UART0_PrintDuty(duty);
        CLK_SysTickDelay(100000);
    }

    return 0;
}
