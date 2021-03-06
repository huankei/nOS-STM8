/*
 * nOS v0.1
 * Copyright (c) 2014 Jim Tremblay
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <stdint.h>
#include <stdio.h>

#include <iostm8s105c6.h>

#include "nOS.h"

#define THREAD_STACK_SIZE       128

static void Timer4Init(void);

void ThreadA(void *arg);
void ThreadB(void *arg);
void ThreadC(void *arg);

nOS_Sem semA;
nOS_Sem semB;
nOS_Sem semC;
nOS_Thread threadA;
nOS_Thread threadB;
nOS_Thread threadC;
nOS_Stack threadAStack[THREAD_STACK_SIZE];
nOS_Stack threadBStack[THREAD_STACK_SIZE];
nOS_Stack threadCStack[THREAD_STACK_SIZE];

static void Timer4Init(void)
{
    CLK_CKDIVR = 0;         /* Default 16 MHz RC oscillator with no prescaler */

    TIM4_IER_UIE = 1;       /* Enable TIM4 interrupts */
    TIM4_PSCR_PSC = 6;      /* Prescaler of 64 from main clock */
    TIM4_ARR = 250;         /* Compare value for 1000 Hz tick */
    TIM4_CR1_CEN = 1;       /* Counter enable */
}

static void LEDBlinkInit(void)
{
    PD_DDR_bit.DDR0 = 1;
    PD_CR1_bit.C10 = 1;
    PD_CR2_bit.C20 = 1;
    PD_ODR_bit.ODR0 = 0;
}

void delay(unsigned int n)
{
    while (n-- > 0);
}

void ThreadA(void *arg)
{
    int arg1 = (int)arg;
    int var1 = 0x01;
    int var2 = 0x02;
    int var3 = 0x03;

    while(1)
    {
        if (arg1 == 0x0110 && var1 == 0x01 && var2 == 0x02 && var3 == 0x03) {
            nOS_SemTake(&semB, NOS_WAIT_INFINITE);
            PD_ODR_bit.ODR0 = 1;
            nOS_SleepMs(500);
            nOS_SemGive(&semA);
        }
    }
}

void ThreadB(void *arg)
{
    int arg2 = (int)arg;
    int var4 = 0x04;
    int var5 = 0x05;
    int var6 = 0x06;

    while(1)
    {
      if (arg2 == 0x0220 && var4 == 0x04 && var5 == 0x05 && var6 == 0x06) {
          nOS_SemTake(&semA, NOS_WAIT_INFINITE);
          PD_ODR_bit.ODR0 = 0;
          nOS_SleepMs(500);
          nOS_SemGive(&semB);
      }
    }
}

void ThreadC(void *arg)
{
    int arg3 = (int)arg;
    volatile uint32_t cntr = 0;

    (void)arg;

    while(1)
    {
        nOS_SemTake(&semC, NOS_WAIT_INFINITE);
        nOS_SemGive(&semB);
        cntr++;
    }
}

NOS_ISR(TIM4_OVR_UIF_vector)
{
    TIM4_SR_UIF = 0;        /* Clear TIM4 interrupt flag */
    nOS_Tick();
}

int main (void)
{
    volatile uint32_t cntr = 0;
    int i;
    int arg1 = 0x0110;
    int arg2 = 0x0220;
    int arg3 = 0x0330;

    CLK_CKDIVR = 0;
    
    LEDBlinkInit();

    nOS_Init();

    nOS_ThreadSetName(NULL, "main");

    nOS_SemCreate(&semA, 1, 1);
    nOS_SemCreate(&semB, 0, 1);
    //nOS_SemCreate(&semC, 0, 1);

    nOS_ThreadCreate(&threadA, ThreadA, (void*)arg1, threadAStack, THREAD_STACK_SIZE, NOS_CONFIG_HIGHEST_THREAD_PRIO,   NOS_THREAD_READY, "ThreadA");
    nOS_ThreadCreate(&threadB, ThreadB, (void*)arg2, threadBStack, THREAD_STACK_SIZE, NOS_CONFIG_HIGHEST_THREAD_PRIO-1, NOS_THREAD_READY, "ThreadB");
    //nOS_ThreadCreate(&threadC, ThreadC, (void*)arg3, threadCStack, THREAD_STACK_SIZE, NOS_CONFIG_HIGHEST_THREAD_PRIO-2, NOS_THREAD_READY, "ThreadC");

    nOS_Start(Timer4Init);
    
    asm("RIM");             /* Enable global interrupts */

    while (1)
    {
        //nOS_SemGive(&semC);
        //asm("RIM");
        cntr++;
        //PD_ODR_bit.ODR0 = !PD_ODR_bit.ODR0;
        //for (i = 0; i < 6; i++)
            //delay(0xFFFF);
    }
}