#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>

#include "ram.h"
#include "random.h"
#include "uart.h"
#include "z80.h"
#include "post.h"

volatile uint8_t buffer[512] = { 0 };
volatile uint8_t seed;

static void initialize_fortuna();
static void run_post_tests();
static void check_mcucsr();

void initialize_fortuna();

int main()
{
    initialize_fortuna();
    post_run();
    
    uart_puthex(seed);

    for(;;);
}

static void initialize_fortuna()
{
    // initialize serial
    _delay_ms(50);
    uart_init();
    
    // check last status
    check_mcucsr();
    seed = rnd_seed();
    
    // initialize devices
    ram_init();
    z80_init();    // the Z80 is now on a reset state (addr/data lines in high impedance)
}

ISR(BADISR_vect)   // catch all interrupts
{
    uart_putchar('!');
    uart_putchar('!');
    uart_putchar('!');
    for(;;);
}

static void check_mcucsr()
{
    if (MCUCSR == 0)
        return;

    uart_putchar('!');

    if (bit_is_set(MCUCSR, PORF))
        uart_putchar('#');
    if (bit_is_set(MCUCSR, EXTRF))
        uart_putchar('%');
    if (bit_is_set(MCUCSR, BORF))
        uart_putchar('~');
    if (bit_is_set(MCUCSR, WDRF))
        uart_putchar('W');
    if (bit_is_set(MCUCSR, JTRF))
        uart_putchar('J');

    uart_putchar(' ');
    MCUCSR = 0;
}
