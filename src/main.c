// Simple Time code that will toggle a pin four times per second

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_clock.h"
#include "nrf_timer.h"
#include "nrf_gpio.h"

// Autocompletion of nrf specific symbols and eldoc work much better when those
// files are included on top level
// This is not heeded if your code is using only SDK code and not accessing
// CMSIS stuff directly
#ifdef NRF51
#include "nrf51.h"
#include "core_cm0.h"
#endif

#ifdef NRF52
#include "nrf52.h"
#include "core_cm4.h"
#endif

#define PIN_LED 19 // LED1

void TIMER0_IRQHandler(void) {

    nrf_gpio_pin_toggle(PIN_LED);
    nrf_timer_event_clear(NRF_TIMER0, NRF_TIMER_EVENT_COMPARE0);
    nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_CLEAR);

}

int main(void)
{
    nrf_gpio_cfg_output(PIN_LED);
    nrf_gpio_pin_set(PIN_LED); // LED is off

    // Irq setup
    NVIC_SetPriority(TIMER0_IRQn, 15); // Lowes priority
    NVIC_ClearPendingIRQ(TIMER0_IRQn);
    NVIC_EnableIRQ(TIMER0_IRQn);

    // Set timer to 32-bit mode
    // using 1MHz clock
    // with compare value 250 000 (channel 0)
    // -> four  interrupts per second
    nrf_timer_mode_set(NRF_TIMER0,NRF_TIMER_MODE_TIMER);
    nrf_timer_bit_width_set(NRF_TIMER0, NRF_TIMER_BIT_WIDTH_32);
    nrf_timer_frequency_set(NRF_TIMER0, NRF_TIMER_FREQ_1MHz);
    nrf_timer_cc_write(NRF_TIMER0, NRF_TIMER_CC_CHANNEL0, 1000000/4);

    // Make a short between compare and clear
    // This does not work as expected
    // nrf_timer_shorts_enable(NRF_TIMER0, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK);

    nrf_timer_int_enable(NRF_TIMER0,NRF_TIMER_INT_COMPARE0_MASK );
    nrf_timer_task_trigger(NRF_TIMER0, NRF_TIMER_TASK_START);

    while(1) {
        __WFI();
    };
}
g
