//******************************************************************************//
// Module Name  : main.c
// Description  : 
// Designer     :
// Date         :
//******************************************************************************//
#include <stdint.h>

#include "../include/risc_v.h"
#include "../include/timer.h"
#include "../include/utils.h"
#include "../include/uart.h"
#include "../include/xprintf.h"

#define SFR(sfr, start, len, dat) (sfr = sfr & ~((~(0xffffffff<<len))<<start) | ((dat & (~(0xffffffff<<len)))<<start))
#define BIT(n)                    (1 << n)

static volatile unsigned int count;
static volatile unsigned int period[] = {0x1b8, 0x129, 0x134, 0x29, 0x143, 0x121, 0x93, 0x39};

//----------------------------------------------------------------------//
// function
//----------------------------------------------------------------------//
void delay(unsigned int timecycle)
{
	unsigned int rcnt = 0;
	while(rcnt < timecycle) rcnt ++;
}


//----------------------------------------------------------------------//
// interrupt
//----------------------------------------------------------------------//
void timer0_irq_handler()
{
    // clear int pending and start timer
	//RV_TIMER2->CON = BIT(8) | BIT(1) | BIT(0);
    RV_TIMER0->PERIOD = period[count%8];     // 10us period
	RV_TIMER0->CON = BIT(2) | BIT(1) | BIT(0);
    RV_TIMER2->CNT = 0x22;     // 10us period
    count++;
    RV_TIMER3->CNT = count;     // 10us period
}


//----------------------------------------------------------------------//
// main
//----------------------------------------------------------------------//
int main()
{
    count = 0;
	unsigned int rcnt = 0;

    RV_TIMER0->PERIOD = 200;     // 10us period
    RV_TIMER0->CON = 0x03 | BIT(2);     // enable interrupt and start timer

    RV_TIMER2->CNT = 0x11;     // 10us period
	rcnt = RV_TIMER0->PERIOD;
	RV_TIMER2->CNT = rcnt;     
    while (1) {
        if (count == 20) {
            RV_TIMER0->CON = 0x00;   // stop timer
            count = 0;
			RV_TIMER3->CNT = TIMER0_REG(TIMER0_VALUE);     // 10us period
        }
    }

    return 0;
}

//******************************************************************************//
// 
// END of Main 
//
//******************************************************************************//
