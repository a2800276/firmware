#include "../core/types.h"
#include "startup.h"
#include "vectortable.h"
#include "../utils/utils.h"

/* Global scope C++ constructors */
typedef void (*init_func)();

/* These variables are used to pass memory locations from the linker script to our code. */
extern uint8_t _LD_STACK_TOP;
extern uint8_t _LD_END_OF_TEXT;
extern uint8_t _LD_START_OF_DATA;
extern uint8_t _LD_END_OF_DATA;
extern uint8_t _LD_END_OF_BSS;
extern init_func _LD_INIT_ARRAY_START;
extern init_func _LD_INIT_ARRAY_END;

/* The vector table - contains the initial stack pointer and
 pointers to boot code as well as interrupt and fault handler pointers.
 The processor will expect this to be located at address 0x0, so
 we put it into a separate section for the linker. In addition, the vector
 is marked as used and annotated as "keep" in the linker script to prevent
 the linker garbage collection from removing it. */

const VECTOR_TABLE vtable __attribute__((used, section(".vectors"))) = {
	//Common for all Cortex M3/M4 MCUs
	&_LD_STACK_TOP,          //Stack top
	bootstrap,               //boot code
	nmi_handler,             //NMI
	hardfault_handler,       //Hard fault
	mpufault_handler,        //Memory protection unit fault handler
	busfault_handler,        //Bus fault handler
	usagefault_handler,      //Usage fault handler
	invalid_interrupt_handler,                 //Unused / reserved
	invalid_interrupt_handler,                 //Unused / reserved
	invalid_interrupt_handler,                 //Unused / reserved
	invalid_interrupt_handler,                 //Unused / reserved
	svcall_handler,          //SVCall handler
	debugmonitor_handler,    //Debug monitor handler
	invalid_interrupt_handler,                 //Unused / reserved
	pendsv_handler,          //PendSV handler
	systick,                 //The SysTick handler

	//MCU-specific from here
	dac_handler,             //DAC converter
	mapp_handler,            //Communication to Cortex M0 (LPC43xx only)
	dma_handler,             //DMA
	invalid_interrupt_handler,                 //Unused / reserved
	flash_eeprom_handler,    //Flash and EEPROM
	ethernet_handler,        //Ethernet
	sdio_handler,            //SDIO
	lcd_handler,             //LCD
	usb0_handler,            //USB 0
	usb1_handler,            //USB 1
	sct_handler,             //SCT (state configurable timer) combined interrupt
	ritimer_handler,         //RITIMER
	timer0_handler,          //Timer 0
	timer1_handler,          //Timer 1
	timer2_handler,          //Timer 2
	timer3_handler,          //Timer 3
	mcpwm_handler,           //Motor control PWM
	adc0_handler,            //ADC 0
	i2c0_handler,            //I2C 0
	i2c1_handler,            //I2C 1
	spi_handler,             //SPI (LPC43xx only, don't confuse with ssp0/ssp1)
	adc1_handler,            //ADC 1
	ssp0_handler,            //SSP 0
	ssp1_handler,            //SSP 1
	usart0_handler,          //USART 0
	uart1_handler,           //UART 1 (and modem)
	usart2_handler,          //USART 2
	usart3_handler,          //USART 3 (and IrDA)
	i2s0_handler,            //I2S 0
	i2s1_handler,            //I2S 1
	spifi_handler,           //SPIFI (LPC43xx only)
	sgpio_handler,           //SGPIO (LPC43xx only)
	gpio0_handler,           //GPIO 0 (PIN_INT0)
	gpio1_handler,           //GPIO 1 (PIN_INT1)
	gpio2_handler,           //GPIO 2 (PIN_INT2)
	gpio3_handler,           //GPIO 3 (PIN_INT3)
	gpio4_handler,           //GPIO 4 (PIN_INT4)
	gpio5_handler,           //GPIO 5 (PIN_INT5)
	gpio6_handler,           //GPIO 6 (PIN_INT6)
	gpio7_handler,           //GPIO 7 (PIN_INT7)
	gpiog0_handler,          //GPIO global interrupt 0
	gpiog1_handler,          //GPIO global interrupt 1
	event_router_handler,    //Event router combined interrupt
	can1_handler,            //CAN 1
	invalid_interrupt_handler,                 //Unused / reserved
	adchs_handler,           //ADCHS (LPC43xx only)
	atimer_handler,          //Alarm timer
	rtc_handler,             //RTX
	invalid_interrupt_handler,                 //Unused / reserved
	wwdt_handler,            //WWDT
	m0sub_handler,           //TXEV from Cortex M0 (Lpc43xx only)
	can0_handler,            //CAN 0
	qei_handler              //Quadrature encoder
};

void bootstrap(void) {
	earlysetup();

	//Set STKALIGN in NVIC. Not stritly necessary, but good to do. TODO: Make more readable, place definition at a proper place
#define NVIC_CCR ((volatile unsigned long *)(0xE000ED14))
//	*NVIC_CCR = *NVIC_CCR | 0x200;

	//copy initial values of variables (non-const globals and static variables) from FLASH to RAM
	uint8_t* mirror = &_LD_END_OF_TEXT; //copy from here
	uint8_t* ram = &_LD_START_OF_DATA;	//copy to here
	while (ram < (&_LD_END_OF_DATA)) *(ram++) = *(mirror++);

	//set uninitialized globals (and globals initialized to zero) to zero
	while (ram < (&_LD_END_OF_BSS)) *(ram++) = 0;

  //call global scope constructors
  init_func* i;
  for (i = &_LD_INIT_ARRAY_START; i != &_LD_INIT_ARRAY_END; i++) (*i)();

	//jump into main user code (which should setup needed timers and interrupts or not return at all)
	main();

	//after main, sleep until an interrupt occurs
	while (true) {
		waitForInterrupt();
	}
}
