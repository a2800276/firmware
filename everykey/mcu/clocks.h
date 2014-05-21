/* CGU (clock generation unit) and CCU (clock control unit)

The CGU controls the generation of base clocks from various internal and external
clock sources. It controls how PLLs, multipliers and dividers derive the clock rates for
base clocks.

The CCUs (CCU0, CCU1) enable and disable branch clocks. One or more branch clocks belong to one
base clock. Branch clocks can be individually enabled or disabled.
The base clock can be turned off if all corresponding branch clocks are disabled. */

#ifndef _CGU_CCU_
#define _CGU_CCU_

#include "../core/types.h"


typedef struct {
	HW_UU UNUSED1[5];
	HW_RW FREQ_MON;
	HW_RW XTAL_OSC_CTRL;
	HW_RO PLL0USB_STAT;
	HW_RW PLL0USB_CTRL;
	HW_RW PLL0USB_MDIV;
	HW_RW PLL0USB_NP_DIV;
	HW_RO PLL0AUDIO_STAT;
	HW_RW PLL0AUDIO_CTRL;
	HW_RW PLL0AUDIO_MDIV;
	HW_RW PLL0AUDIO_NP_DIV;
	HW_RW PLL0AUDIO_FRAC;
	HW_RO PLL1_STAT;
	HW_RW PLL1_CTRL;
	HW_RW IDIVA_CTRL;
	HW_RW IDIVB_CTRL;
	HW_RW IDIVC_CTRL;
	HW_RW IDIVD_CTRL;
	HW_RW IDIVE_CTRL;
	HW_RO BASE_SAFE_CLK;
	HW_RW BASE_USB0_CLK;
	HW_RW BASE_PERIPH_CLK;  //LPC43xx only
	HW_RW BASE_USB1_CLK;
	HW_RW BASE_Mx_CLK;      //BASE_M3_CLOCK on LPC18xx, BASE_M4_CLOCK on LPC43xx
	HW_RW BASE_SPIFI_CLK;
	HW_RW BASE_SPI_CLK;     //LPC43xx only
	HW_RW BASE_PHY_RX_CLK;
	HW_RW BASE_PHY_TX_CLK;
	HW_RW BASE_APB1_CLK;
	HW_RW BASE_APB3_CLK;
	HW_RW BASE_LCD_CLK;
	HW_RW BASE_ADCHS_CLK;   //LPC43xx only
	HW_RW BASE_SDIO_CLK;
	HW_RW BASE_SSP0_CLK;
	HW_RW BASE_SSP1_CLK;
	HW_RW BASE_UART0_CLK;
	HW_RW BASE_UART1_CLK;
	HW_RW BASE_UART2_CLK;
	HW_RW BASE_UART3_CLK;
	HW_RW BASE_OUT_CLK;
	HW_UU OUTCLK_21_CTRL;
	HW_UU OUTCLK_22_CTRL;
	HW_UU OUTCLK_23_CTRL;
	HW_UU OUTCLK_24_CTRL;
	HW_RW BASE_AUDIO_CLK;
	HW_RW BASE_CGU_OUT0_CLK;
	HW_RW BASE_CGU_OUT1_CLK;
} __attribute__((packed)) CGU_STRUCT;

#define CGU_HW ((CGU_STRUCT*)(0x40050000))

/** XTAL_OSC_CTRL values */
typedef enum {
	XTAL_OSC_CTRL_ENABLE = 0x01,	//0 = on, 1=off
	XTAL_OSC_CTRL_BYPASS = 0x02,	//0 = normal, 1 = ext clock source
	XTAL_OSC_CTRL_HF = 0x4 	        //0 = LF crystal (<20MHz), 1 = HF crystal (>15MHz), 15..20 don't care
} XTAL_OSC_CTRL_BITS;

typedef enum {
	BASE_SAFE_CLK = 0,      //12MHz max
	BASE_USB0_CLK = 1,      //480MHz max
	BASE_PERIPH_CLK = 2,    //MCU speed (180 or 204MHz) max
	BASE_USB1_CLK = 3,      //MCU speed (180 or 204MHz) max
	BASE_Mx_CLK = 4,        //MCU speed (180 or 204MHz) max
	BASE_SPIFI_CLK = 5,     //MCU speed (180 or 204MHz) max
	BASE_SPI_CLK = 6,       //MCU speed (180 or 204MHz) max
	BASE_PHY_RX_CLK = 7,    //75MHz max
	BASE_PHY_TX_CLK = 8,    //75MHz max
	BASE_APB1_CLK = 9,      //MCU speed (180 or 204MHz) max
	BASE_APB3_CLK = 10,     //MCU speed (180 or 204MHz) max
	BASE_LCD_CLK = 11,      //MCU speed (180 or 204MHz) max
	BASE_ADCHS_CLK = 12,    //80MHz max
	BASE_SDIO_CLK = 13,     //MCU speed (180 or 204MHz) max
	BASE_SSP0_CLK = 14,     //MCU speed (180 or 204MHz) max
	BASE_SSP1_CLK = 15,     //MCU speed (180 or 204MHz) max
	BASE_UART0_CLK = 16,    //MCU speed (180 or 204MHz) max
	BASE_UART1_CLK = 17,    //MCU speed (180 or 204MHz) max
	BASE_UART2_CLK = 18,    //MCU speed (180 or 204MHz) max
	BASE_UART3_CLK = 19,    //MCU speed (180 or 204MHz) max
	BASE_OUT_CLK = 20,      //MCU speed (180 or 204MHz) max
	BASE_AUDIO_CLK = 25,    //MCU speed (180 or 204MHz) max
	BASE_CGU_OUT0_CLK = 26, //MCU speed (180 or 204MHz) max
	BASE_CGU_OUT1_CLK = 27  //MCU speed (180 or 204MHz) max
} CGU_BASE_CLOCK;

/** Hint: Values*8 are offset to CCU1->CLK_APB3_BUS_CFG (first branch clock) */
typedef enum {
	CLK_APB3_BUS         = 0,
	CLK_APB3_I2C1        = 1,
	CLK_APB3_DAC         = 2,
	CLK_APB3_ADC0        = 3,
	CLK_APB3_ADC1        = 4,
	CLK_APB3_CAN0        = 5,
	CLK_APB1_BUS         = 32,
	CLK_APB1_MOTOCON_PWM = 33,
	CLK_APB1_I2C0        = 34,
	CLK_APB1_I2S         = 35,
	CLK_APB1_CAN1        = 36,
	CLK_SPIFI            = 64,
	CLK_Mx_BUS           = 96,
	CLK_Mx_SPIFI         = 97,
	CLK_Mx_GPIO          = 98,
	CLK_Mx_LCD           = 99,
	CLK_Mx_ETHERNET      = 100,
	CLK_Mx_USB0          = 101,
	CLK_Mx_EMC           = 102,
	CLK_Mx_SDIO          = 103,
	CLK_Mx_DMA           = 104,
	CLK_Mx_M3CORE        = 105,
	CLK_Mx_SCT           = 109,
	CLK_Mx_USB1          = 110,
	CLK_Mx_EMCDIV        = 111,
	CLK_Mx_FLASHA        = 112,
	CLK_Mx_FLASHB        = 113,
	CLK_Mx_EEPROM        = 116,
	CLK_Mx_WWDT          = 128,
	CLK_Mx_USART0        = 129,
	CLK_Mx_UART1         = 130,
	CLK_Mx_SSP0          = 131,
	CLK_Mx_TIMER0        = 132,
	CLK_Mx_TIMER1        = 133,
	CLK_Mx_SCU           = 134,
	CLK_Mx_CREG          = 135,
	CLK_Mx_RITIMER       = 160,
	CLK_Mx_USART2        = 161,
	CLK_Mx_USART3        = 162,
	CLK_Mx_TIMER2        = 163,
	CLK_Mx_TIMER3        = 164,
	CLK_Mx_SSP1          = 165,
	CLK_Mx_QEI           = 166,
	CLK_USB0             = 224,
	CLK_USB1             = 256,
	CLK_AUDIO            = 512,
	CLK_APB2_USART3      = 544,
	CLK_APB2_USART2      = 576,
	CLK_APB0_USART1      = 608,
	CLK_APB0_USART0      = 640,
	CLK_APB2_SSP1        = 672,
	CLK_APB0_SSP0        = 704,
	CLK_SDIO             = 736
} CCU_BRANCH_CLOCK;

/** sources for base clock output stages - not all sources are allowed for all clocks, see manual */
typedef enum {
	CLKSRC_32KOSC = 0,      //32 KHz oscillator
	CLKSRC_IRC = 1,         //Internal RC oscillator
	CLKSRC_ENET_RX_CLK = 2, //Ethernet RX clock
	CLKSRC_ENET_TX_CLK = 3, //Ethernet TX clock
	CLKSRC_GP_CLKIN = 4,    //General purpose clock input
	CLKSRC_XTAL = 6,        //HS Crystal oscillator (XTAL)
	CLKSRC_PLL0USB = 7,     //PLL 0 USB
	CLKSRC_PLL0AUDIO = 8,   //PLL 0 Audio
	CLKSRC_PLL1 = 9,        //PLL 1
	CLKSRC_IDIVA = 12,      //Integer divider A (max div/4, can be source for IDIVB..E)
	CLKSRC_IDIVB = 13,      //Integer divider B (max div/16)
	CLKSRC_IDIVC = 14,      //Integer divider C (max div/16)
	CLKSRC_IDIVD = 15,      //Integer divider D (max div/16)
	CLKSRC_IDIVE = 16,      //Integer divider E (max div/256)
} CGU_CLKSRC;

/** identifier for PLL */
typedef enum {
	CGU_PLL0USB = 0,
	CGU_PLL0AUDIO = 1,
	CGU_PLL1 = 2
} CGU_PLL_ID;

/** values for both PLL0USB_STAT and PLL0AUDIO_STAT */
typedef enum {
	PLL0_STAT_LOCK_MASK = 0x01,
	PLL0_STAT_LOCKED = 0x01,
	PLL0_STAT_FR_MASK = 0x02,
	PLL0_STAT_FREERUNNING = 0x02
} CGU_PLL0_STAT_VALUES;

/** values for PLL1_STAT */
typedef enum {
	PLL1_STAT_LOCK_MASK = 0x01,
	PLL1_STAT_LOCKED = 0x01
} CGU_PLL1_STAT_VALUES;


typedef enum {
	PLL1_PD           = 0x01,
	PLL1_BYPASS       = 0x02,
	PLL1_FBSEL        = 0x40,
	PLL1_DIRECT       = 0x80,
	PLL1_AUTOBLOCK    = 0x800
} CGU_PLL1_CTRL_VALUES;

typedef enum {
	IDIV_PD           = 0x01,
	IDIV_AUTOBLOCK    = 0x800
} CGU_IDIV_CTRL_VALUES;

typedef enum {
	CLKOUT_PD         = 0x01,
	CLKOUT_AUTOBLOCK  = 0x800
} CGU_CLKOUT_CTRL_VALUES;


typedef struct {
	HW_RW PM;
	HW_RO BASE_STAT;
	HW_UU UNUSED1[62];
	HW_RW CLK_APB3_BUS_CFG;         // 0
	HW_RO CLK_APB3_BUS_STAT;
	HW_RW CLK_APB3_I2C1_CFG;        // 1
	HW_RO CLK_APB3_I2C1_STAT;
	HW_RW CLK_APB3_DAC_CFG;         // 2
	HW_RO CLK_APB3_DAC_STAT;
	HW_RW CLK_APB3_ADC0_CFG;        // 3
	HW_RO CLK_APB3_ADC0_STAT;
	HW_RW CLK_APB3_ADC1_CFG;        // 4
	HW_RO CLK_APB3_ADC1_STAT;
	HW_RW CLK_APB3_CAN0_CFG;        // 5
	HW_RO CLK_APB3_CAN0_STAT;
	HW_UU UNUSED2[52];
	HW_RW CLK_APB1_BUS_CFG;         // 32
	HW_RO CLK_APB1_BUS_STAT;
	HW_RW CLK_APB1_MOTOCON_PWM_CFG; // 33
	HW_RO CLK_APB1_MOTOCON_PWM_STAT;
	HW_RW CLK_APB1_I2C0_CFG;        // 34
	HW_RO CLK_APB1_I2C0_STAT;
	HW_RW CLK_APB1_I2S_CFG;         // 35
	HW_RO CLK_APB1_I2S_STAT;
	HW_RW CLK_APB1_CAN1_CFG;        // 36
	HW_RO CLK_APB1_CAN1_STAT;
	HW_UU UNUSED3[54];
	HW_RW CLK_SPIFI_CFG;            // 64
	HW_RO CLK_SPIFI_STAT;
	HW_UU UNUSED4[62];
	HW_RW CLK_M3_BUS_CFG;           // 96
	HW_RO CLK_M3_BUS_STAT;
	HW_RW CLK_M3_SPIFI_CFG;         // 97
	HW_RO CLK_M3_SPIFI_STAT;
	HW_RW CLK_M3_GPIO_CFG;          // 98
	HW_RO CLK_M3_GPIO_STAT;
	HW_RW CLK_M3_LCD_CFG;           // 99
	HW_RO CLK_M3_LCD_STAT;
	HW_RW CLK_M3_ETHERNET_CFG;      // 100
	HW_RO CLK_M3_ETHERNET_STAT;
	HW_RW CLK_M3_USB0_CFG;          // 101
	HW_RO CLK_M3_USB0_STAT;
	HW_RW CLK_M3_EMC_CFG;           // 102
	HW_RO CLK_M3_EMC_STAT;
	HW_RW CLK_M3_SDIO_CFG;          // 103
	HW_RO CLK_M3_SDIO_STAT;
	HW_RW CLK_M3_DMA_CFG;           // 104
	HW_RO CLK_M3_DMA_STAT;
	HW_RW CLK_M3_M3CORE_CFG;        // 105
	HW_RO CLK_M3_M3CORE_STAT;
	HW_UU UNUSED5[6];
	HW_RW CLK_M3_SCT_CFG;           // 109
	HW_RO CLK_M3_SCT_STAT;
	HW_RW CLK_M3_USB1_CFG;          // 110
	HW_RO CLK_M3_USB1_STAT;
	HW_RW CLK_M3_EMCDIV_CFG;        // 111
	HW_RO CLK_M3_EMCDIV_STAT;
	HW_RW CLK_M3_FLASHA_CFG;        // 112
	HW_RO CLK_M3_FLASHA_STAT;
	HW_RW CLK_M3_FLASHB_CFG;        // 113
	HW_RO CLK_M3_FLASHB_STAT;
	HW_UU UNUSED6[4];
	HW_RW CLK_M3_EEPROM_CFG;        // 116
	HW_RO CLK_M3_EEPROM_STAT;
	HW_UU UNUSED7[22];
	HW_RW CLK_M3_WWDT_CFG;          // 128
	HW_RO CLK_M3_WWDT_STAT;
	HW_RW CLK_M3_USART0_CFG;        // 129
	HW_RO CLK_M3_USART0_STAT;
	HW_RW CLK_M3_UART1_CFG;         // 130
	HW_RO CLK_M3_UART1_STAT;
	HW_RW CLK_M3_SSP0_CFG;          // 131
	HW_RO CLK_M3_SSP0_STAT;
	HW_RW CLK_M3_TIMER0_CFG;        // 132
	HW_RO CLK_M3_TIMER0_STAT;
	HW_RW CLK_M3_TIMER1_CFG;        // 133
	HW_RO CLK_M3_TIMER1_STAT;
	HW_RW CLK_M3_SCU_CFG;           // 134
	HW_RO CLK_M3_SCU_STAT;
	HW_RW CLK_M3_CREG_CFG;          // 135
	HW_RO CLK_M3_CREG_STAT;
	HW_UU UNUSED8[48];
	HW_RW CLK_M3_RITIMER_CFG;       // 160
	HW_RO CLK_M3_RITIMER_STAT;
	HW_RW CLK_M3_USART2_CFG;        // 161
	HW_RO CLK_M3_USART2_STAT;
	HW_RW CLK_M3_USART3_CFG;        // 162
	HW_RO CLK_M3_USART3_STAT;
	HW_RW CLK_M3_TIMER2_CFG;        // 163
	HW_RO CLK_M3_TIMER2_STAT;
	HW_RW CLK_M3_TIMER3_CFG;        // 164
	HW_RO CLK_M3_TIMER3_STAT;
	HW_RW CLK_M3_SSP1_CFG;          // 165
	HW_RO CLK_M3_SSP1_STAT;
	HW_RW CLK_M3_QEI_CFG;           // 166
	HW_RO CLK_M3_QEI_STAT;
	HW_UU UNUSED9[114];
	HW_RW CLK_USB0_CFG;             // 224
	HW_RO CLK_USB0_STAT;
	HW_UU UNUSED10[62];
	HW_RW CLK_USB1_CFG;             // 256
	HW_RO CLK_USB1_STAT;
} __attribute__((packed)) CCU1_STRUCT;

#define CCU1_HW ((CCU1_STRUCT*)(0x40051000))

typedef struct {
	HW_RW PM;
	HW_RO BASE_STAT;
	HW_UU UNUSED1[62];
	HW_RW CLK_AUDIO_CFG;            // 512
	HW_RO CLK_AUDIO_STAT;
	HW_UU UNUSED2[62];
	HW_RW CLK_APB2_USART3_CFG;      // 544
	HW_RO CLK_APB2_USART3_STAT;
	HW_UU UNUSED3[62];
	HW_RW CLK_APB2_USART2_CFG;      // 576
	HW_RO CLK_APB2_USART2_STAT;
	HW_UU UNUSED4[62];
	HW_RW CLK_APB0_USART1_CFG;      // 608
	HW_RO CLK_APB0_USART1_STAT;
	HW_UU UNUSED5[62];
	HW_RW CLK_APB0_USART0_CFG;      // 640
	HW_RO CLK_APB0_USART0_STAT;
	HW_UU UNUSED6[62];
	HW_RW CLK_APB2_SSP1_CFG;        // 672
	HW_RO CLK_APB2_SSP1_STAT;
	HW_UU UNUSED7[62];
	HW_RW CLK_APB0_SSP0_CFG;        // 704
	HW_RO CLK_APB0_SSP0_STAT;
	HW_UU UNUSED8[62];
	HW_RW CLK_SDIO_CFG;             // 736
	HW_RO CLK_SDIO_STAT;
} __attribute__((packed)) CCU2_STRUCT;

#define CCU2_HW ((CCU2_STRUCT*)(0x40052000))

/* Bits for status and config registers */
typedef enum {
	CCU_RUN           = 0x01,
	CCU_AUTO          = 0x02,
	CCU_WAKEUP        = 0x04
} CCU_STAT_CFG_VALUES;

#ifdef __cplusplus
extern "C" {
#endif

/** enables or disables the external HS crystal and sets the crystal parameters according to the
platform settings (see platformsettings.h)
@param enable true to turn crystal on, false to turn it off  */
void clock_enable_xtal(bool enable);

/** returns whether the external HS crystal is enabled.
@return true if it is running, false if not. Returning true does not necessarily mean it is already stable.
bool clock_xtal_is_enabled();

/** returns whether a pll is locked
@param pll the pll to query
@return true if the pll is locked, false otherwise */
bool clock_pll_locked(CGU_PLL_ID pll);

/** enables or disables a PLL. Most detail settings are set to normal operation
@param pll pll to set
@param enable true to turn PLL on, false to turn it off
@param src clock source for PLL
@param preDiv predivider N (range is pll-dependent, set to 0 to disable predivider)
		For PLL1, allowed values are 1..4
@param fbDiv feedback divider M (range is pll-dependent, set to 0 to disable postdivider)
		For PLL1, allowed values are 1..256
@param postDiv postdivider P (range is pll-dependent, set to 0 to disable postdivider)
		For PLL1, allowed values are 1,2,4,8,16
@param fbsel if true, feedback is generated from the output clock.
		if false, feedback is generated from the cco directly, before postDiv operation (output frequency lower)
*/

/************ !!! THIS FUNCTION IS CURRENTLY ONLY IMPLEMENTED FOR PLL1 !!! **********************/
void clock_set_pll(CGU_PLL_ID pll, bool enable, CGU_CLKSRC src, uint32_t preDiv, uint32_t fbDiv, uint32_t postDiv, bool fbsel);
/************ !!! THIS FUNCTION IS CURRENTLY ONLY IMPLEMENTED FOR PLL1 !!! **********************/

/** enables or disables an idiv (integer divider register)
@param idivIdx idiv to set (0=A .. 4=E)
@param enable true to enable idiv, false to turn off
@param src clock source to use. Allowed values may vary, see manual
@param div divider value. Values are dependent on idiv. A:1..4, B-D:1..16, E:1..256 */
void clock_set_idiv(uint8_t idivIdx, bool enable, CGU_CLKSRC src, uint8_t div);

/** configures a base clock
@param clock clock to set. BASE_SAFE_CLK cannot be changed.
@param enable true to enable clock (always autoblock), false to turn off
@param src clock source to use */
void clock_set_source(CGU_BASE_CLOCK clock, bool enable, CGU_CLKSRC src);

/** sets PLL1 and core MCU speed (BASE_Mx_CLK) to a given multiple of the XTAL frequency.
Note that this might take some microseconds longer than absolutely needed
@param n nominator of multiplier
@param d denominator of multiplier */
void clock_set_xtal_core_freq(uint8_t n, uint8_t d);

/** turns a branch clock on or off
@param clock clock to modify
@param on on or off */
void clock_enable(CCU_BRANCH_CLOCK clock, bool on);

/** turns a branch clock auto-AHB-disable feature on or off
@param clock clock to modify
@param on on or off */
void clock_set_auto(CCU_BRANCH_CLOCK clock, bool on);

/** turns a branch clock auto-AHB-disable feature on or off
@param clock clock to modify
@param on on or off */
void clock_set_wakeup(CCU_BRANCH_CLOCK clock, bool on);

/** returns the clock's run state
@param clock clock to query
@return true if it is running, false otherwise */
bool clock_get_enable(CCU_BRANCH_CLOCK clock);

/** returns the clock's auto-AHB-disable state
@param clock clock to query
@return true if the feature is active, false otherwise */
bool clock_get_auto(CCU_BRANCH_CLOCK clock);

/** returns the clock's auto-AHB-disable state
@param clock clock to query
@return true if it is running, false otherwise */
bool clock_get_wakeup(CCU_BRANCH_CLOCK clock);

#ifdef __cplusplus
}
#endif

#endif
