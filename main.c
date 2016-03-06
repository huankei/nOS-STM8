#include <iostm8s105c6.h>
#include "nOS.h"

int tick = 0;
// Automatic push of PC, X, Y, A and CCR on ISR enter
// Automatic global interrupt disabling on ISR enter
// Automatic pop of PC, X, Y, A and CCR on ISR exit
//  this has the effet of restauring previous interrupt mask state
//  IRET doesn't set anything special
NOS_ISR(TIM4_OVR_UIF_vector)
{
  TIM4_SR_UIF = 0; // Clear TIM4 interrupt flag
  tick++;
}
//
// Delay loop
//
// Actual delay depends on clock settings
// and compiler optimization settings.
//
void delay(unsigned int n)
{
    while (n-- > 0);
}

void my_function(void)
{
  int i = 1;
}

int main( void )
{
    // Clock config
    CLK_CKDIVR = 0; // Default 16Mhz RC clock with no divider
    // Timer4 init
    TIM4_IER_UIE = 1; // Enable interrupt
    TIM4_PSCR_PSC = 6; // Prescaler of 64
    TIM4_ARR = 250;  // Compare value
    TIM4_CR1_CEN = 1; // Counter enable
    asm("RIM"); // Enable global interrupts
  
    //
    // Data Direction Register
    //
    // 0: Input
    // 1: Output
    //
    PD_DDR_bit.DDR0 = 1;
    
    //
    // Control Register 1
    //
    // Input mode:
    //   0: Floating input
    //   1: Input with pull-up
    //
    // Output mode:
    //   0: Pseudo open drain
    //   1: Push-pull
    //
    PD_CR1_bit.C10 = 1;
    
    //
    // Control Register 2
    //
    // Input mode:
    //   0: External interrupt disabled
    //   1: External interrupt enabled
    //
    // Output mode:
    //   0: Output speed up to  2 MHz
    //   1: Output speed up to 10 MHz
    //
    PD_CR2_bit.C20 = 1;
    
    //
    // Output Data Register
    //
    // Output value
    //
    PD_ODR_bit.ODR0 = 0;
    
    //
    // Main loop
    //
    while (1)
    {
        PD_ODR_bit.ODR0 = !PD_ODR_bit.ODR0;
        delay(0xFFFF);
        delay(0xFFFF);
        delay(0xFFFF);
        delay(0xFFFF);
        delay(0xFFFF);
        delay(0xFFFF);
    }
}