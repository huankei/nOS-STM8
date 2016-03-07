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

void my_func2(void)
{
  unsigned char a = 5;
  unsigned char b = 5;
  unsigned char c = 5;
  unsigned char d = 5;
unsigned char e = 5;
unsigned char f = 5;
  unsigned char g = 5;
  unsigned char h = 5;
  unsigned char i = 5;
  unsigned char j = 5;
  unsigned char k = 5;
  unsigned char l = 5;
  unsigned char m = 5;
  unsigned char n = 5;
  unsigned char o = 5;
  unsigned char p = 5;
  
  a = b;
  b = c;
  c = d;
d = e;
e=f;
  f=g;
  g=h;
  h=i;
  i=j;
  k=l;
  m=n;
  o=p;
}
void my_func(void)
{
  unsigned char a = 5;
  unsigned char b = 5;
  unsigned char c = 5;
  unsigned char d = 5;
  unsigned char e = 5;
unsigned char f = 5;
unsigned char g = 5;
//unsigned char h = 5;
//unsigned char i = 5;
//unsigned char j = 5;
//unsigned char k = 5;
//unsigned char l = 5;
//unsigned char m = 5;
//unsigned char n = 5;
//unsigned char o = 5;
//unsigned char p = 5;
  
  //my_func2();

  a = b;
  b = c;
  c = d;
d = e;
e=f;
  f=g;
//  g=h;
//  h=i;
//  i=j;
//  k=l;
//  m=n;
//  o=p;
}

int main( void )
{
    unsigned char intstate;
    unsigned int sp;
    unsigned int x;
    unsigned int y;
    unsigned char cc;

    // Clock config
    CLK_CKDIVR = 0; // Default 16Mhz RC clock with no divider
    // Timer4 init
    TIM4_IER_UIE = 1; // Enable interrupt
    TIM4_PSCR_PSC = 6; // Prescaler of 64
    TIM4_ARR = 250;  // Compare value
    TIM4_CR1_CEN = 1; // Counter enable
    //asm("RIM"); // Enable global interrupts
  
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
        intstate = __get_interrupt_state();
        __set_cpu_sp(0xFFFF);
        sp = __get_cpu_sp();
        x = __get_cpu_x();
        y = __get_cpu_y();
        cc = __get_cpu_cc();
        my_func();
        PD_ODR_bit.ODR0 = !PD_ODR_bit.ODR0;
        //__push_context();
        delay(0xFFFF);
        delay(0xFFFF);
        delay(0xFFFF);
        delay(0xFFFF);
        delay(0xFFFF);
        delay(0xFFFF);
    }
}