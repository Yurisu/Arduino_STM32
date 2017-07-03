/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Bryan Newbold.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file fsmc.c
 * @brief Flexible static memory controller support.
 */

#include "fsmc.h"
#include "gpio.h"

#if defined(STM32_HIGH_DENSITY) && defined(BOARD_generic_f407v) // pins not yet defined for disco F407

/**
 * Configure FSMC GPIOs for use with LCDs.
 */
<<<<<<< HEAD
void fsmc_sram_init_gpios(void) {
    /* Data lines... */
    gpio_set_mode(PD0, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PD1, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PD8, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PD9, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PD10, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PD14, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PD15, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PE7, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PE8, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PE9, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PE10, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PE11, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PE12, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PE13, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PE14, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PE15, GPIO_AF_OUTPUT_PP);

    /* Address lines... */
    gpio_set_mode(PD11, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PD12, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(PD13, GPIO_AF_OUTPUT_PP);
#if 0 // not available on LQFP package
    gpio_set_mode(GPIOF,  0, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOF,  1, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOF,  2, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOF,  3, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOF,  4, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOF,  5, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOF, 12, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOF, 13, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOF, 14, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOF, 15, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOG,  0, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOG,  1, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOG,  2, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOG,  3, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOG,  4, GPIO_AF_OUTPUT_PP);
    gpio_set_mode(GPIOG,  5, GPIO_AF_OUTPUT_PP);
#endif  // not available on LQFP package
    /* And control lines... */
    gpio_set_mode(PD4, GPIO_AF_OUTPUT_PP);   // NOE
    gpio_set_mode(PD5, GPIO_AF_OUTPUT_PP);   // NWE

    gpio_set_mode(PD7, GPIO_AF_OUTPUT_PP);   // NE1
#if 0 // not available on LQFP package
    gpio_set_mode(GPIOG,  9, GPIO_AF_OUTPUT_PP);   // NE2
    gpio_set_mode(GPIOG, 10, GPIO_AF_OUTPUT_PP);   // NE3
    gpio_set_mode(GPIOG, 12, GPIO_AF_OUTPUT_PP);   // NE4
#endif  // not available on LQFP package

    gpio_set_mode(PE0, GPIO_AF_OUTPUT_PP);   // NBL0
    gpio_set_mode(PE1, GPIO_AF_OUTPUT_PP);   // NBL1
=======

void fsmc_init(void) {
    rcc_clk_enable(RCC_FSMC);
    rcc_reset_dev(RCC_FSMC);
}

// used control, address and data lines
// NOE -> RD, NWE -> WR, A18 -> RS, NE1 -> CS
const uint8_t fsmc_pins[]= {FSMC_NOE, FSMC_NWE, FSMC_NE1, FSMC_A18,
							FSMC_D0, FSMC_D1, FSMC_D2, FSMC_D3,
							FSMC_D4, FSMC_D5, FSMC_D6, FSMC_D7,
							FSMC_D8, FSMC_D9, FSMC_D10, FSMC_D11,
							FSMC_D12, FSMC_D13, FSMC_D14, FSMC_D15};

void fsmc_lcd_init_gpios(void) {
	uint8_t i;
	for (i=0; i<sizeof(fsmc_pins); i++) {
		uint8_t pin = fsmc_pins[i];
		gpio_set_mode(pin, GPIO_AF_OUTPUT_PP);
		gpio_set_af_mode(pin, 12);
	}
}

volatile uint16_t * fsmcCommand;
volatile uint16_t * fsmcData;

void fsmc_lcd_init(void)
{
    fsmcCommand    =  FSMC_BANK1; // clears A18
    fsmcData       =  (fsmcCommand+(1<<18)); // sets A18

	fsmc_init();
	fsmc_lcd_init_gpios();
	// set access for asynchronous SRAM type, 16 bit wide data bus
	// see RM0090, 36.5.4, pages 1552-1554
	uint32_t val = (FSMC_BCR_MTYP_SRAM | FSMC_BCR_MWID_16BITS | FSMC_BCR_WREN);
	//Serial.print("BCR: "); Serial.println(val, HEX);
	fsmc_nor_psram_set_BCR(FSMC_NOR_PSRAM1_BASE, val);
	// set timing data Mode 1. Adjust DATAST according to the WR low period timing from the LCD specification
	val = (FSMC_BTR_ACCMOD_A | FSMC_BTR_DATLAT_(0) | FSMC_BTR_CLKDIV_(0) | FSMC_BTR_BUSTURN_(0) | FSMC_BTR_DATAST_(6) | FSMC_BTR_ADDHLD_(0) | FSMC_BTR_ADDSET_(2));
	//Serial.print("BTR: "); Serial.println(val, HEX);
	fsmc_nor_psram_set_BTR(FSMC_NOR_PSRAM1_BASE, val);
	// enable FSCM
	fsmc_nor_psram_bank_enable(FSMC_NOR_PSRAM1_BASE);
>>>>>>> refs/remotes/origin/master
}


#endif  /* STM32_HIGH_DENSITY */
