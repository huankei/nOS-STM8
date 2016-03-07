/*
 * Copyright (c) 2014-2016 Jim Tremblay
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef NOSPORT_H
#define NOSPORT_H

#include <iostm8s105c6.h>
#include <intrinsics.h>
   
extern void __push_context_from_task(void);
extern void __pop_context_from_task (void);
extern void __push_context_from_isr (void);
extern void __pop_context_from_isr  (void);

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
        sr = CPU_CCR;                                                           \
        __disable_interrupt();                                                             \
    } while (0)

#define nOS_LeaveCritical(sr)                                                   \
    CPU_CCR = sr

nOS_Stack*      nOS_EnterIsr        (nOS_Stack *sp);
nOS_Stack*      nOS_LeaveIsr        (nOS_Stack *sp);

#define NOS_ISR(vect) \
void vect##_ISR_L2(void); \
_Pragma(_STRINGIFY(vector=vect)) \
__interrupt void vect##_ISR(void) \
{ \
    __push_context_from_isr(); \
    vect##_ISR_L2(); \
    __disable_interrupt(); \
} \
void vect##_ISR_L2(void)

//#define NOS_ISR(vect)                                                           \
//void vect##_ISR(void) __attribute__ ( ( naked ) );                              \
//inline void vect##_ISR_L2(void) __attribute__( ( always_inline ) );             \
//ISR(vect, ISR_NAKED)                                                            \
//{                                                                               \
//    vect##_ISR();                                                               \
//    reti();                                                                     \
//}                                                                               \
//void vect##_ISR(void)                                                           \
//{                                                                               \
//    PUSH_CONTEXT();                                                             \
//    SP = (int)nOS_EnterIsr((nOS_Stack*)SP);                                     \
//    vect##_ISR_L2();                                                            \
//    cli();                                                                      \
//    SP = (int)nOS_LeaveIsr((nOS_Stack*)SP);                                     \
//    POP_CONTEXT();                                                              \
//    asm volatile ("ret");                                                       \
//}                                                                               \
//inline void vect##_ISR_L2(void)

#ifdef NOS_PRIVATE
 void   nOS_InitSpecific        (void);
 void   nOS_InitContext         (nOS_Thread *thread, nOS_Stack *stack, size_t ssize, nOS_ThreadEntry entry, void *arg);
 /* Absolutely need a naked function because function call push the return address on the stack */
 __task void   nOS_SwitchContext       (void);
#endif

#ifdef __cplusplus
}
#endif

#endif /* NOSPORT_H */
