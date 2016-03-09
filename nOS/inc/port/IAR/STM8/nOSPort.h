/*
 * Copyright (c) 2014-2016 Jim Tremblay
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef NOSPORT_H
#define NOSPORT_H

#include <intrinsics.h>

extern void          __push_context           (void);
extern void          __pop_context            (void);
extern void          __set_cpu_sp             (int sp);
extern unsigned int  __get_cpu_sp             (void);
extern unsigned int  __get_cpu_x              (void);
extern unsigned int  __get_cpu_y              (void);
extern unsigned char __get_cpu_cc             (void);

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t                 nOS_Stack;
typedef uint8_t                 nOS_StatusReg;

#define NOS_UNUSED(v)           (void)v

#define NOS_MEM_ALIGNMENT       1

#ifdef NOS_CONFIG_ISR_STACK_SIZE
 #if (NOS_CONFIG_ISR_STACK_SIZE == 0)
  #error "nOSConfig.h: NOS_CONFIG_ISR_STACK_SIZE is set to invalid value: must be higher than 0."
 #endif
#endif

#define nOS_EnterCritical(sr)                                                   \
    do {                                                                        \
        sr = __get_interrupt_state();                                           \
        __disable_interrupt();                                                  \
    } while (0)

#define nOS_LeaveCritical(sr)                                                   \
    __set_interrupt_state(sr)

nOS_Stack*      nOS_EnterIsr        (nOS_Stack *sp);
nOS_Stack*      nOS_LeaveIsr        (nOS_Stack *sp);

#define NOS_ISR(vect)                                                           \
__task void vect##_ISR_L2(void);                                                \
void vect##_ISR_L3(void);                                                       \
_Pragma(_STRINGIFY(vector=vect))                                                \
__interrupt void vect##_ISR(void)                                               \
{                                                                               \
    vect##_ISR_L2();                                                            \
}                                                                               \
__task void vect##_ISR_L2(void)                                                 \
{                                                                               \
    __push_context();                                                           \
    __set_cpu_sp((int)nOS_EnterIsr((nOS_Stack*)__get_cpu_sp()));                \
    vect##_ISR_L3();                                                            \
    __disable_interrupt();                                                      \
    __set_cpu_sp((int)nOS_LeaveIsr((nOS_Stack*)__get_cpu_sp()));                \
    __pop_context();                                                            \
    asm("ret");                                                                 \
}                                                                               \
void vect##_ISR_L3(void)

#ifdef NOS_PRIVATE
 void   nOS_InitSpecific         (void);
 void   nOS_InitContext          (nOS_Thread *thread, nOS_Stack *stack, size_t ssize, nOS_ThreadEntry entry, void *arg);
 /* Absolutely need a naked function because function call push the return address on the stack */
 __task void   nOS_SwitchContext (void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NOSPORT_H */
