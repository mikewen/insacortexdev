/*----------------------------------------------------------------------------
 *      R T L   K e r n e l   E x a m p l e
 *----------------------------------------------------------------------------
 * Reprise de RT_blinky pour tester can et RTX
 * ACCO JUIN2009
  *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2008 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>
#include <stm32f10x_lib.h>              /* STM32F10x Library Definitions     */
#include "STM32_Reg.h"   
#include "STM32_Init.h"                 /* STM32 Initialization              */
#include "LCD.h"
#include "LCD.h"                                  // LCD function prototypes
#include "CAN.h"                                  // STM32 CAN adaption layer

OS_TID t_emmit;                        /* assigned task id of task: phase_a */
OS_TID t_receiv;                        /* assigned task id of task: phase_b */
OS_TID t_clock;                         /* assigned task id of task: clock   */
OS_TID t_lcd;                           /* assigned task id of task: lcd     */

#define LED_A   0x40
#define LED_B   0x20

#define LED_CLK 0x80

#define LED_On(led)     GPIOB->ODR |=  (led << 8)
#define LED_Off(led)    GPIOB->ODR &= ~(led << 8)



unsigned int val_Tx = 0xFC, val_Rx = 0xFC;              // Global variables used for display

char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


/*------------------------------------------------------------------------------
  convert one byte to string in hexadecimal notation
 *------------------------------------------------------------------------------*/
void Hex_Str (unsigned char hex, char *str) {
  *str++ = '0';
  *str++ = 'x';
  *str++ = hex_chars[hex >>  4];
  *str++ = hex_chars[hex & 0xF];
}


/*----------------------------------------------------------------------------
  insert a delay time.
 *----------------------------------------------------------------------------*/
void delay(unsigned int nCount)	{

  for(; nCount != 0; nCount--);
}


/*------------------------------------------------------------------------------
  Initialises the Analog/Digital converter
  PA1 (ADC Channel1) is used as analog input
 *------------------------------------------------------------------------------*/
void adc_Init (void) {

  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;             // enable periperal clock for ADC1

  ADC1->SQR1  = 0x00000000;                       // only one conversion
  ADC1->SMPR2 = 0x00000028;                       // set sample time channel1 (55,5 cycles)
  ADC1->SQR3  = 0x00000001;                       // set channel1 as 1st conversion

  ADC1->CR1   = 0x00000100;                       // use independant mode, SCAN mode
  ADC1->CR2   = 0x000E0003;                       // data align right, cont conversion
                                                  // EXTSEL = SWSTART
                                                  // enable ADC, no external Trigger
  ADC1->CR2  |=  0x00500000;                      // start SW conversion
}

/*------------------------------------------------------------------------------
  read a converted value from the Analog/Digital converter
 *------------------------------------------------------------------------------*/
unsigned char adc_Get (void) {
  unsigned short val;

  while(!(ADC1->SR & ADC_SR_EOC));                // whait til end of conversion
  val = ADC1->DR & ADC_DR_DATA;                   // Get conversion result
  val = (val >> 4) & 0xFF;                        // Scale analog value to 8 bits
	
  return (val);
}

/*----------------------------------------------------------------------------
  display transmit and receieve values
 *---------------------------------------------------------------------------*/
void val_display (void) {
  static char disp_buf[] = "Xx:    ";    // display string

  set_cursor (0, 0);
  Hex_Str(val_Tx, &disp_buf[3]);                 // display Tx and Rx values to LCD disp
  lcd_print  ((char *)disp_buf);                  // print string to LCD
  set_cursor (7, 0);
  lcd_bargraph (val_Tx, 8);
  
  set_cursor (0, 1);
  Hex_Str(val_Rx, &disp_buf[3]);
  lcd_print  ((char *)disp_buf);                  // print string to LCD
  set_cursor (7, 1);
  lcd_bargraph (val_Rx, 8);
}


/*----------------------------------------------------------------------------
  initialize CAN interface
 *----------------------------------------------------------------------------*/
void can_Init (void) {

  CAN_setup ();                                   // setup CAN interface
  CAN_wrFilter (33, STANDARD_FORMAT);             // Enable reception of messages

  /* COMMENT THE LINE BELOW TO ENABLE DEVICE TO PARTICIPATE IN CAN NETWORK   */
  CAN_testmode(CAN_BTR_SILM | CAN_BTR_LBKM);      // Loopback, Silent Mode (self-test)

  CAN_start ();                                   // leave init mode

  CAN_waitReady ();                               // wait til mbx is empty
}


/*----------------------------------------------------------------------------
 *        Function 'signal_func' called from multiple tasks
 *---------------------------------------------------------------------------*/
void signal_func (OS_TID task)  {
  os_evt_set (0x0100, t_clock);          /* send event signal to clock task  */
  os_dly_wait (20);                      /* delay 50 clock ticks             */
  os_evt_set (0x0100, t_clock);          /* send event signal to clock task  */
  os_dly_wait (20);                      /* delay 50 clock ticks             */
  os_evt_set (0x0001, task);             /* send event to task 'task'        */
}

/*----------------------------------------------------------------------------
 *        Task 1 'phaseA': Phase A output
 *---------------------------------------------------------------------------*/
__task void emmit (void) {
	int i;

	 CAN_TxMsg.id = 33;                              // initialise message to send
  	for (i = 0; i < 8; i++) CAN_TxMsg.data[i] = 0;
  	CAN_TxMsg.len = 1;
  	CAN_TxMsg.format = STANDARD_FORMAT;
  	CAN_TxMsg.type = DATA_FRAME;

  while (1) 
  {  
    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
	LED_On (LED_A);
	os_dly_wait (50);                      /* delay 50 clock ticks             */
    

	if (CAN_TxRdy) 
	{
      CAN_TxRdy = 0;

      CAN_TxMsg.data[0] = adc_Get ();             // data[0] field = ADC value
      CAN_wrMsg (&CAN_TxMsg);                     // transmit message
      val_Tx = CAN_TxMsg.data[0];
    }
	LED_Off(LED_A);
 
    signal_func (t_receiv);              /* call common signal function      */
   }
}
/*----------------------------------------------------------------------------
 *        Task 2 'phaseB': Phase B output
 *---------------------------------------------------------------------------*/
__task void receiv (void) 
{
  int i; 

  CAN_TxMsg.id = 33;                              // initialise message to send
  for (i = 0; i < 8; i++) CAN_TxMsg.data[i] = 0;
  CAN_TxMsg.len = 1;
  CAN_TxMsg.format = STANDARD_FORMAT;
  CAN_TxMsg.type = DATA_FRAME;

  while (1) 
  {  

    os_evt_wait_and (0x0001, 0xffff);    /* wait for an event flag 0x0001    */
	LED_On (LED_B);
 	os_dly_wait (50);                      /* delay 50 clock ticks             */
    

  
    if (CAN_RxRdy) {
      CAN_RxRdy = 0;

      val_Rx = CAN_RxMsg.data[0];
    }
    LED_Off(LED_B);
	 
    signal_func (t_emmit);              /* call common signal function      */
  }
 }
/*----------------------------------------------------------------------------
 *        Task 5 'clock': Signal Clock
 *---------------------------------------------------------------------------*/
__task void clock (void) {
  for (;;) {
    os_evt_wait_and (0x0100, 0xffff);    /* wait for an event flag 0x0100    */
    LED_On (LED_CLK);
    os_dly_wait (10);                     /* delay 8 clock ticks              */
    LED_Off(LED_CLK);
  }
}

/*----------------------------------------------------------------------------
 *        Task 6 'lcd': LCD Control task
 *---------------------------------------------------------------------------*/
__task void lcd (void) {
  int i;
  lcd_init ();                           /* Initialize LCD display module    */
  lcd_clear ();

  for (;;) {
    lcd_print ("RTX with CAN ");
    set_cursor(0, 1);
    lcd_print (" Kernel example by Acco");
    os_dly_wait (40);

    for(i=0;i<100;i++)
	{
		lcd_clear ();

    	val_display ();                               // display TX and RX values

  
    	os_dly_wait (50);

	}
  }
}

/*----------------------------------------------------------------------------
 *        Task 8 'init': Initialize
 *---------------------------------------------------------------------------*/
__task void init (void) {

  t_emmit = os_tsk_create (emmit, 0);  /* start task phaseA                */
  t_receiv = os_tsk_create (receiv, 0);  /* start task phaseB                */
  t_clock  = os_tsk_create (clock, 0);   /* start task clock                 */
  t_lcd    = os_tsk_create (lcd, 0);     /* start task lcd                   */
  os_evt_set (0x0001, t_emmit);         /* send signal event to task phaseA */
  os_tsk_delete_self ();
}

/*----------------------------------------------------------------------------
 *        Main: Initialize and start RTX Kernel
 *---------------------------------------------------------------------------*/
int main (void) {

  stm32_Init ();                         /* STM32 setup (must be called first*/
  adc_Init ();                                    // initialise A/D converter
  can_Init ();                                    // initialise CAN interface

  lcd_init  ();                                   // initialise LCD
  lcd_clear ();

  os_sys_init (init);                    /* Initialize RTX and start init    */
}

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

