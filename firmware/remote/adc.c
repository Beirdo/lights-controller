#include "local.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <stdint.h>

uint8_t adcsra_val;

void adc_setup(void)
{
    /* Setup ADC reference (2.56V internal), channel mux to ADC3/ISRC */
    ADMUX = (1 << REFS1) | (1 << REFS0) | (1 << MUX1) | (1 << MUX0);
    adcsra_val = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0); /* 250kHz clock */
    ADCSRA = adcsra_val;
}

uint16_t adc_read_raw(void)
{
    uint16_t raw;

    /* Start conversion, clear old interrupt, enable interrupt */
    ADCSRA = adcsra_val | (1 << ADSC) | (1 << ADIF) | (1 << ADIE);

    while( ADCSRA & (1 << ADSC) )
    {
        sleep_mode();
    }

    raw = ADCL | (ADCH << 8);

    return( raw );
}

ISR(ADC_vect)
{
    /* Shut the interrupt back off */
    ADCSRA = adcsra_val;
}

/*
 * vim:ts=4:sw=4:ai:et:si:sts=4
 */

