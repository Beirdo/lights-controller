#include "local.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>

/* 38400 to the sensors over BLVDS */
#define BAUD 38400
#include <util/setbaud.h>

uint8_t u_rx_buf[MAX_BUF_LEN];
uint8_t u_rx_index;
uint8_t u_rx_size;
uint8_t u_tx_buf[MAX_BUF_LEN];
uint8_t u_tx_index;
uint8_t u_tx_size;
uint8_t ucsra_val;
uint8_t lin_id;
uint16_t lin_reading;

typedef struct {
    uint16_t min;
    uint16_t typ;
    uint16_t max;
} linIdReading_t;

linIdReading_t linIdTable[] = {
    { 38, 40, 45 },
    { 46, 48, 54 },
    { 57, 60, 68 },
    { 69, 72, 81 },
    { 84, 88, 99 },
    { 104, 108, 122 },
    { 127, 132, 149 },
    { 181, 188, 212 },
    { 262, 272, 306 },
    { 316, 328, 369 },
    { 386, 400, 450 },
    { 463, 480, 540 },
    { 579, 600, 675 },
    { 694, 720, 810 },
    { 849, 880, 989 },
    { 1023, 1023, 1023 }
};

void uart_setup(void)
{
    int i;

    /* Set LIN baud rate */
    LINBRRH = UBRRH_VALUE;
    LINBRRL = UBRRL_VALUE;

    /* Read LIN ID from ADC with current source on */
    lin_id = 32;
    lin_reading = 334;

    /* Look up the ADC reading in the id table */
    for (i = 0; i < 16; i++) {
        if (linIdTable[i].min <= lin_reading &&
            linIdTable[i].max >= lin_reading) {
            lin_id = i;
            break;
        }
    }

    if (lin_id == 32) {
        lin_id = 15;
    }

    /* Setup LIN ID */
    LINIDR = lin_id;

    /* Enable LIN 2.1 */
    LINCR = (1 << LENA);

    /* Forceably clear any pending interrupts */
    LINSIR = (1 << LERR) | (1 << LIDOK) | (1 << LTXOK) | (1 << LRXOK);

    /* Enable interrupts */
    LINENIR = (1 << LENERR) | (1 << LENIDOK) | (1 << LENTXOK) | (1 << LENRXOK);

    /* Disable the pullups on the TX and RX lines */
    PORTA &= ~((1 << PA1) | (1 << PA0));

    u_rx_index = 0;
    u_rx_size  = MAX_BUF_LEN;
    u_tx_index = 0;
}

/* LIN Transfer Complete interrupt */
ISR(LIN_TC_vect)
{
}

/* LIN Error interrupt */
ISR(LIN_ERR_vect)
{
}

void uart_transmit(void)
{
}

/*
 * vim:ts=4:sw=4:ai:et:si:sts=4
 */

