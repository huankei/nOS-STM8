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

void ThreadA(void *arg)
{
    volatile uint32_t cntr = 0;

    (void)arg;

    while(1)
    {
        nOS_SemTake(&semA, NOS_WAIT_INFINITE);
        cntr++;
    }
}

void ThreadB(void *arg)
{
    volatile uint32_t cntr = 0;
    cntr |= 0x80;

    (void)arg;

    while(1)
    {
        nOS_SemTake(&semB, NOS_WAIT_INFINITE);
        nOS_SemGive(&semA);
        cntr++;
    }
}

void ThreadC(void *arg)
{
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

static void Timer4Init(void)
{
    CLK_CKDIVR = 0;         /* Default 16 MHz RC oscillator with no prescaler */

    TIM4_IER_UIE = 1;       /* Enable TIM4 interrupts */
    TIM4_PSCR_PSC = 6;      /* Prescaler of 64 from main clock */
    TIM4_ARR = 250;         /* Compare value for 1000 Hz tick */
    TIM4_CR1_CEN = 1;       /* Counter enable */

    asm("RIM");             /* Enable global interrupts */
}

int main (void)
{
    volatile uint32_t cntr = 0;

    nOS_Init();

    nOS_ThreadSetName(NULL, "main");

    nOS_SemCreate(&semA, 0, 1);
    nOS_SemCreate(&semB, 0, 1);
    nOS_SemCreate(&semC, 0, 1);

    nOS_ThreadCreate(&threadA, ThreadA, (void*)300, threadAStack, THREAD_STACK_SIZE, NOS_CONFIG_HIGHEST_THREAD_PRIO,   NOS_THREAD_READY, "ThreadA");
    nOS_ThreadCreate(&threadB, ThreadB, (void*)200, threadBStack, THREAD_STACK_SIZE, NOS_CONFIG_HIGHEST_THREAD_PRIO-1, NOS_THREAD_READY, "ThreadB");
    nOS_ThreadCreate(&threadC, ThreadC, (void*)100, threadCStack, THREAD_STACK_SIZE, NOS_CONFIG_HIGHEST_THREAD_PRIO-2, NOS_THREAD_READY, "ThreadC");

    nOS_Start(Timer4Init);

    while (1)
    {
        nOS_SemGive(&semC);
        cntr++;
    }
}