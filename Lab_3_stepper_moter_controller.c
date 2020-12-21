#include <SI_EFM8BB1_Register_Enums.h>

sbit SW = P0 ^ 1; //Counter-Clockwise Switch
sbit SW2 = P0 ^ 0; //ClockWise Switch

void InitDevice(void) {
  //Disable Watchdog Timer
  //Disable Watchdog with key sequence
  WDTCN = 0xDE; //First key
  WDTCN = 0xAD; //Second key
  // [WDTCN � Watchdog Timer Control]$

  //Setup Defaults for P1
  P1MDOUT = P1MDOUT_B0__OPEN_DRAIN | P1MDOUT_B1__OPEN_DRAIN |
    P1MDOUT_B2__OPEN_DRAIN | P1MDOUT_B3__OPEN_DRAIN |
    P1MDOUT_B4__PUSH_PULL | P1MDOUT_B5__OPEN_DRAIN |
    P1MDOUT_B6__OPEN_DRAIN | P1MDOUT_B7__OPEN_DRAIN;

  //Enable Weak Pullups
  XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;

  //Disable Interrupts
  IE = IE_EA__DISABLED | IE_EX0__DISABLED | IE_EX1__DISABLED |
    IE_ESPI0__DISABLED | IE_ET0__DISABLED | IE_ET1__DISABLED |
    IE_ET2__ENABLED | IE_ES0__DISABLED;

}
void delay();

void MSDelay(unsigned int value) //IMPORTANT DELAY FUCTION NOT TO BE MIXED WITH delay(); OK!
{
  unsigned int x, y;
  for (x = 0; x < 350; x++) //determines the speed of the motor LARGER value SLOWER SMALLER value FASTER for x
    for (y = 0; y < value; y++);
}

main(void) {
  unsigned int i;
  P0 = 0xFF; //error checking 
  P1 = 0x00; // error checking

  SW = 1; //Switch set to high
  InitDevice();

  while (1) {
    if (SW == 0) { //Counter Clockwise Switch
      {
        for (i = 0; i < 65; i++) { // Determines how far the motor goes out in degrees (this is closest to 45 degrees) 2 missisipi seconds
          P1 = 0x66;
          MSDelay(1);
          P1 = 0xCC;
          MSDelay(1);
          P1 = 0x99;
          MSDelay(1);
          P1 = 0x33;
          MSDelay(1);
        }
        delay(); //calls the delay time function USING TCON_etc..........
      }
    } else if (SW2 == 0) { //Clock wise Switch
      {
        for (i = 0; i < 130; i++) { // Determines how far the motor goes out in degrees (this is closest to 90 degrees)
          P1 = 0x66;
          MSDelay(1);
          P1 = 0x33;
          MSDelay(1);
          P1 = 0x99;
          MSDelay(1);
          P1 = 0xCC;
          MSDelay(1);
        }
        delay();
      }
    }
  }
}

void delay() // Delay generation using Timer 0 mode 1
{
  TMOD = 0x01; // Mode1 of Timer0
  TH0 = 0xFC; // FC66 evaluated hex value for 1millisecond delay
  TL0 = 0x66;
  TCON_TR0 = 1; // Start Timer
  while (TCON_TF0 == 0); // Using polling method
  TCON_TR0 = 0; // Stop Timer
  TCON_TF0 = 0; // Clear flag
} //probably most important part for the timer