#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "spi_lib.h"
#include "radio_lib.h"

typedef unsigned char uchar;
typedef signed char schar;

// PORTC   PORTF
// PORTA   PORTD

#define SR_DATA 2
#define SR_CLK 3
#define SR_CLR 4

#define ADDR_SIZE 5
#define POINT_NUMBER 2

typedef struct
{
    unsigned angle : 7;
    signed   x     : 5;
    unsigned y     : 4;
}Point;

static const uchar payloadSize = POINT_NUMBER * sizeof(Point);
static uchar address[ADDR_SIZE] = {0x11, 0x12, 0x12, 0x12, 0x12};

Point points[POINT_NUMBER];

volatile uchar pos;
volatile bool flag = false;
volatile uchar j;

ISR(INT7_vect)
{
    sendToNrf(R, R_RX_PAYLOAD, NULL, payloadSize, (uchar*)points);
    resetIRQ();
}

ISR(INT5_vect)
{
    if(++pos > 127)
    {
        pos = 0;
        ++j;
    }
    flag = true;
    EIFR=0x20;
}

void setPorts(schar height, uchar side)
{
    if(side == 0)
    {
        if(height > 7)
        {
            PORTC |= _BV(height - 8);
        }
        else if((height >= 0) && (height <= 7))
        {
            PORTA |= _BV(height);
        }
    }
    else
    {
        if(height > 7)
        {
            PORTF |= _BV(height - 8);
        }
        else if((height >= 0) && (height <= 7))
        {
            PORTD |= _BV(height);
        }
    }
}

int main(void)
{
    // External Interrupt(s) initialization
    // INT5: On
    // INT5 Mode: Any change
    // INT7: On
    // INT7 Mode: Falling edge
    EICRB = _BV(ISC71) | _BV(ISC50);
    EIMSK = _BV(INT5) | _BV(INT7);
    EIFR = _BV(INT5) | _BV(INT7);
    sei();
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;
    DDRF = 0xFF;
    DDRE = _BV(SR_DATA) | _BV(SR_CLK) | _BV(SR_CLR);
    PORTE |= _BV(SR_CLR);
    initAsReceiver(address, ADDR_SIZE, payloadSize);
    startListengForPayload();

    while(1)
    {
        // Draw Points
        while(flag == false);
        flag = false;
        PORTE |= _BV(SR_DATA);
        for(uchar i = 0; i < 16; ++i)
        {
            PORTA = 0;
            PORTD = 0;
            PORTC = 0;
            PORTF = 0;

            for(uchar k = 0; k < POINT_NUMBER; ++k)
            {
                uchar oppositePos = (pos + 64) & 0x7F;
                if(points[k].angle == pos)
                {
                    if(points[k].x == i)
                    {
                        setPorts(points[k].y, 0);
                    }
                    else if(-(points[k].x + 1) == i)
                    {
                        setPorts(points[k].y, 1);
                    }
                }
                else if(points[k].angle == oppositePos)
                {
                    if(points[k].x == i)
                    {
                        setPorts(points[k].y, 1);
                    }
                    else if(-(points[k].x + 1) == i)
                    {
                        setPorts(points[k].y, 0);
                    }
                }
            }

            PORTE |= _BV(SR_CLK);
            PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
        }
    }
}
