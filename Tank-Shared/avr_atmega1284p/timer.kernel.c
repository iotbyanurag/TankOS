
// This module sets up timers.
// Two timers are provided for miscellaneous software-tasks.
// The timer 3 is used for this, the frequency is 1ms.
// Weak functions can be implemented to couple code to the timer-ISRs.

#include <platform/kernel_init.h>
#include <kernel/millisecond_clock.h>
#include <platform/platform_Avr/avr_atmega1284p/timer.h>
#include "timer.h"

// timer.c
extern Timer millisecond_timer_A;
extern Timer millisecond_timer_B;
extern Timer generic_timer_A;
extern Timer generic_timer_B;
extern void setTimerPairFrequency(Timer timerA, Timer timerB, uint16_t herz);

#define MILLISECOND_FREQUENCY 1000

static void init_millisecond_timers() {
    millisecond_timer_A = newTimer_m1284P(3, TIMER_A, FALSE); // Timer 3A
    millisecond_timer_B = newTimer_m1284P(3, TIMER_B, FALSE); // Timer 3B

    TCCR3A = 0; // No compare-match-output
    TCCR3B = _BV(WGM32) // Clear timer on compare match (max = OCCRA)
                         // No input-capture, noise-canceller
            | _BV(CS31); // clock-select: prescale 8
    TCCR3C = 0; // No forced output-compare

    setTimerPairFrequency(millisecond_timer_A, millisecond_timer_B, MILLISECOND_FREQUENCY);
}

static void init_generic_timers() {
    generic_timer_A = newTimer_m1284P(1, TIMER_A, FALSE); // Timer 1A
    generic_timer_B = newTimer_m1284P(1, TIMER_B, FALSE); // Timer 1B

    TCCR1A = 0; // No compare-match-output
    TCCR1B = _BV(WGM12) // Clear timer on compare match (max = OCCRA)
                         // No input-capture, noise-canceller
            | _BV(CS11); // clock-select: prescale 8
    TCCR1C = 0; // No forced output-compare

    setGenericTimerFrequency(MILLISECOND_FREQUENCY);
}

static void init_tank_timer() {
    init_millisecond_timers();
    init_generic_timers();
}
KERNEL_INIT(init_tank_timer)

void enableMillisecondTimerInterrupt_A() {
    TIMSK3 |= _BV(OCIE3A);
}

void enableMillisecondTimerInterrupt_B() {
    TIMSK3 |= _BV(OCIE3B);
}

void enableGenericTimerInterrupt_A() {
    TIMSK1 |= _BV(OCIE1A);
}

void enableGenericTimerInterrupt_B() {
    TIMSK1 |= _BV(OCIE1B);
}
