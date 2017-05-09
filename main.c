#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

typedef unsigned char uchar;
typedef signed char schar;

// PORTC   PORTF
// PORTA   PORTD

#define SR_DATA 2
#define SR_CLK 3
#define SR_CLR 4

volatile uchar pos;
volatile bool flag = false;
volatile uchar j;
const uchar sinusF[] = {5, 6, 7, 8, 9, 10, 11, 12, 12, 11, 10, 9, 8, 7, 6, 5};
const uchar widthSq[] = {10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13};
uchar sinus[] = {12, 12, 11, 11, 10, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5,
                 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12};

const uchar sputnik[] = {12, 12, 11, 11, 10, 10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5,
                 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12};

const uchar cosinus[] = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5,
                   5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 10,
                   10, 10, 10, 10, 10, 9, 9, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5,
                   5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0};
const uchar cosinus2[] = {10, 10, 10, 10, 10, 9, 9, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5,
                    5, 4, 4, 3, 3, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0,
                    0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 5,
                    5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 10};
const uchar circleUp[] = {0x20, 0x20, 0x10, 0x10, 0x0C, 0x03};
const uchar circleDown[] = {0x04, 0x04, 0x08, 0x08, 0x30, 0xC0};

const uchar papaA[] =
{
    0xFF,
    0x80,
    0x80,
    0x80,
    0xFF,
    0x0,
    0x3,
    0xC,
    0x38,
    0xC8,
    0xC8,
    0x38,
    0xC,
    0x3,
    0x0,
    0xFF,
    0x80,
    0x80,
    0x80,
    0xFF,
    0x0,
    0x3,
    0xC,
    0x18,
    0x68,
    0xC8
};

const uchar papaD[] =
{
    0xC8,
    0x38,
    0xC,
    0x3,
    0x0,
    0xF0,
    0x10,
    0x10,
    0xFF,
    0x0,
    0xFF,
    0x18,
    0x24,
    0x42,
    0x81,
    0x0,
    0x3,
    0xC,
    0x38,
    0xC8,
    0xC8,
    0x38,
    0xC,
    0x3
};

const unsigned char leti[] =
{
  0x80,
  0x80,
  0x7E,
  0x1,
  0x1,
  0x1,
  0xFF,
  0x0,
  0x91,
  0x91,
  0x52,
  0x34,
  0x18,
  0x0,
  0x1,
  0x1,
  0x1,
  0xFF,
  0x1,
  0x1,
  0x1,
  0x0,
  0xFF,
  0x60,
  0x20,
  0x18,
  0xC,
  0xFF
};

const unsigned char sau[] =
{
  0x3C,
  0x66,
  0xC3,
  0x81,
  0x81,
  0x0,
  0xC0,
  0x60,
  0x18,
  0x16,
  0x13,
  0x13,
  0x1C,
  0x30,
  0xC0,
  0x0,
  0x80,
  0x81,
  0x42,
  0x24,
  0x18,
  0x8,
  0x4,
  0x2,
  0x1
};

const unsigned char ksu[] =
{
  0xFF,
  0x18,
  0x24,
  0x42,
  0x81,
  0x0,
  0x3C,
  0x66,
  0xC3,
  0x81,
  0x81,
  0x0,
  0x80,
  0x81,
  0x42,
  0x24,
  0x18,
  0x8,
  0x4,
  0x2,
  0x1
};

const unsigned char fea[] =
{
  0x1C,
  0x22,
  0x22,
  0xFF,
  0x22,
  0x22,
  0x1C,
  0x0,
  0x91,
  0x91,
  0x91,
  0x52,
  0x52,
  0x34,
  0x18,
  0x0,
  0xC0,
  0x30,
  0x1C,
  0x14,
  0x13,
  0x13,
  0x14,
  0x1C,
  0x30,
  0xC0
};

const unsigned char raps[] =
{
  0xFF,
  0x21,
  0x21,
  0x12,
  0xC,
  0x0,
  0xC0,
  0x30,
  0x1C,
  0x13,
  0x13,
  0x1C,
  0x30,
  0xC0,
  0x0,
  0xFF,
  0x1,
  0x1,
  0x1,
  0x1,
  0xFF,
  0x0,
  0x3C,
  0x66,
  0xC3,
  0x81,
  0x81,
  0x81
};

const unsigned char etpt[] =
{
  0x91,
  0x91,
  0x52,
  0x52,
  0x52,
  0x3C,
  0x0,
  0x1,
  0x1,
  0x1,
  0xFF,
  0x1,
  0x1,
  0x1,
  0x0,
  0xFF,
  0x1,
  0x1,
  0x1,
  0x1,
  0xFF,
  0x0,
  0x1,
  0x1,
  0x1,
  0xFF,
  0x1,
  0x1,
  0x1
};

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
    bool updated = false;
    uchar rep = 0;
    // External Interrupt(s) initialization
    // INT5: On
    // INT5 Mode: Any change
    EICRA=0x00;
    EICRB=0x04;
    EIMSK=0x20;
    EIFR=0x20;
    sei();
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;
    DDRF = 0xFF;
    DDRE = _BV(SR_DATA) | _BV(SR_CLK) | _BV(SR_CLR);
    PORTE |= _BV(SR_CLR);
    uchar ang;
    while(1)
    {
        // LETI
        for(unsigned int l = 0; l < 4000; ++l)
        {
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;

                if(i == 13)
                {
                    for(uchar m = 0; m < sizeof(leti); ++m)
                    {
                        uchar pos1 = m;
                        uchar pos2 = pos1 + 64;
                        if(pos == pos1)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(leti[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTA = line >> (7 - (l / 500));
                        }
                        if(pos == pos2)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(leti[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTD = line >> (7 - (l / 500));
                        }
                    }
                }
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
        }
        ang = 0;
        for(unsigned int k = 0; k < 2* 51 * 127/*15000*/; ++k)
        {
            //static uchar ang = 0;
            static schar delay = 50;
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;

                if(i == 13)
                {
                    for(uchar m = 0; m < sizeof(leti); ++m)
                    {
                        uchar pos1 = m + ang;
                        while(pos1 > 127)
                        {
                            pos1 -= 127;
                        }
                        uchar pos2 = pos1 + 64;
                        if(pos2 > 127)
                        {
                            pos2 -= 127;
                        }
                        if(pos == pos1)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(leti[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTA = line;
                        }
                        if(pos == pos2)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(leti[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTD = line;
                        }
                    }
                }
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
            if(--delay < 0)
            {
                delay = 50;
                if(++ang > 127)
                {
                    ang = 0;
                }
            }
        }
        for(unsigned int l = 0; l < 4000; ++l)
        {
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;

                if(i == 13)
                {
                    for(uchar m = 0; m < sizeof(leti); ++m)
                    {
                        uchar pos1 = m;
                        uchar pos2 = pos1 + 64;
                        if(pos == pos1)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(leti[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTA = line >> ((l / 500));
                        }
                        if(pos == pos2)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(leti[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTD = line >> ((l / 500));
                        }
                    }
                }
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
        }

        // FEA
        for(unsigned int l = 0; l < 4000; ++l)
        {
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;

                if(i == 13)
                {
                    for(uchar m = 0; m < sizeof(fea); ++m)
                    {
                        uchar pos1 = m;
                        uchar pos2 = pos1 + 64;
                        if(pos == pos1)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(fea[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTA = line >> (7 - (l / 500));
                        }
                        if(pos == pos2)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(fea[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTD = line >> (7 - (l / 500));
                        }
                    }
                }
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
        }
        ang = 0;
        for(unsigned int k = 0; k < 2 * 51 * 127/*15000*/; ++k)
        {
            //static uchar ang = 0;
            static schar delay = 50;
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;

                if(i == 13)
                {
                    for(uchar m = 0; m < sizeof(fea); ++m)
                    {
                        uchar pos1 = m + ang;
                        while(pos1 > 127)
                        {
                            pos1 -= 127;
                        }
                        uchar pos2 = pos1 + 64;
                        if(pos2 > 127)
                        {
                            pos2 -= 127;
                        }
                        if(pos == pos1)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(fea[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTA = line;
                        }
                        if(pos == pos2)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(fea[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTD = line;
                        }
                    }
                }
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
            if(--delay < 0)
            {
                delay = 50;
                if(++ang > 127)
                {
                    ang = 0;
                }
            }
        }
        for(unsigned int l = 0; l < 4000; ++l)
        {
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;

                if(i == 13)
                {
                    for(uchar m = 0; m < sizeof(fea); ++m)
                    {
                        uchar pos1 = m;
                        uchar pos2 = pos1 + 64;
                        if(pos == pos1)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(fea[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTA = line >> ((l / 500));
                        }
                        if(pos == pos2)
                        {
                            uchar line = 0;
                            for(uchar a = 0; a < 8; ++a)
                            {
                                if(bit_is_set(fea[m], a))
                                {
                                    line |= _BV(7 - a);
                                }
                            }
                            PORTD = line >> ((l / 500));
                        }
                    }
                }
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
        }

        // double helix
        for(unsigned int k = 0; k < 15000; ++k)
        {
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;

                if(i < 4)
                {
                    if((pos == 12) || (pos == (12 + 64)))
                    {
                        PORTA |= _BV(1);
                        PORTD |= _BV(1);
                    }

                    if((pos == 28) || (pos == (28 + 64)))
                    {
                        PORTA |= _BV(3);
                        PORTD |= _BV(3);
                    }

                    if((pos == 44) || (pos == (44 + 64)))
                    {
                        PORTA |= _BV(5);
                        PORTD |= _BV(5);
                    }

                    if((pos == 60) || (pos == (60 + 64)))
                    {
                        PORTA |= _BV(7);
                        PORTD |= _BV(7);
                    }

                    if((pos == 76) || (pos == (76 - 64)))
                    {
                        PORTC |= _BV(1);
                        PORTF |= _BV(1);
                    }

                    if((pos == 90) || (pos == (90 - 64)))
                    {
                        PORTC |= _BV(3);
                        PORTF |= _BV(3);
                    }

                    if((pos == 106) || (pos == (106 - 64)))
                    {
                        PORTC |= _BV(5);
                        PORTF |= _BV(5);
                    }

                    if((pos == 122) || (pos == (122 - 64)))
                    {
                        PORTC |= _BV(7);
                        PORTF |= _BV(7);
                    }
                }

                if(i == 4)
                {
                    setPorts(pos / 8, 0);
                    setPorts(pos / 8 + 8, 1);
                    setPorts(pos / 8, 1);
                    setPorts(pos / 8 + 8, 0);
                    if((pos / 8) > 0)
                    {
                        setPorts(pos / 8 - 8, 1);
                        setPorts(pos / 8 - 8, 0);
                    }
                }

                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }

        }
        // flower
        for(unsigned int k = 0; k < 15000; ++k)
        {
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;
                setPorts(sinusF[((i + j + pos) % 16)], 0);
                setPorts(sinusF[((i + j + pos) % 16)], 1);
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
        }
        // Planet
        for(unsigned int k = 0; k < 15000; ++k)
        {
            static uchar l = 0;
            static uchar height = 12;
            static bool direction = false;
            static int delay = 355;
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            if(direction)
            {
                if(!(--delay > 0))
                {
                    delay = 255;
                    if(++height > 11)
                    {
                        direction = false;
                    }
                }
            }
            else
            {
                if(!(--delay > 0))
                {
                    delay = 255;
                    if(--height < 3)
                    {
                        direction = true;
                    }
                }
            }
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;
                if(i < sizeof(circleUp))
                {
                    PORTA = circleDown[i];
                    PORTC = circleUp[i];
                    PORTD = circleDown[i];
                    PORTF = circleUp[i];
                }
                l &= 127;
                if((pos == l) && (i == 8))
                {
                    setPorts(height, 0);
                }
                if((pos == ((64 + l) % 127)) && (i == 8))
                {
                    setPorts(height, 1);
                }
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
            if(pos == 127)
            {
                l += 3;
            }
        }
        // rotating rectangle
        for(unsigned int k = 0; k < 15000; ++k)
        {
            static uchar l = 0;
            unsigned char newPos;
            newPos = (pos + l) & 127;
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;
                if(newPos < 16)
                {
                    if(i < widthSq[newPos])
                    {
                        PORTA = _BV(0) | _BV(7);
                        PORTD = _BV(0) | _BV(7);
                    }
                    else if(i == widthSq[newPos])
                    {
                        PORTA = 255;
                        PORTD = 255;
                    }
                    else
                    {
                        PORTA = 0;
                        PORTC = 0;
                    }
                }
                else if(newPos >= 16 && newPos < 32)
                {
                    if(i < widthSq[16 - (newPos - 16)])
                    {
                        PORTA = _BV(0) | _BV(7);
                        PORTD = _BV(0) | _BV(7);
                    }
                    else if(i == widthSq[16 - (newPos - 16)])
                    {
                        PORTA = 255;
                        PORTD = 255;
                    }
                    else
                    {
                        PORTA = 0;
                        PORTC = 0;
                    }
                }
                else if((newPos >= 32) && (newPos < 48))
                {
                    if(i < widthSq[newPos - 32])
                    {
                        PORTA = _BV(0) | _BV(7);
                        PORTD = _BV(0) | _BV(7);
                    }
                    else if(i == widthSq[newPos - 32])
                    {
                        PORTA = 255;
                        PORTD = 255;
                    }
                    else
                    {
                        PORTA = 0;
                        PORTC = 0;
                    }
                }
                else if(newPos >= 48 && newPos < 64)
                {
                    if(i < widthSq[16 - (newPos - 48)])
                    {
                        PORTA = _BV(0) | _BV(7);
                        PORTD = _BV(0) | _BV(7);
                    }
                    else if(i == widthSq[16 - (newPos - 48)])
                    {
                        PORTA = 255;
                        PORTD = 255;
                    }
                    else
                    {
                        PORTA = 0;
                        PORTC = 0;
                    }
                }
                else if((newPos >= 64) && (newPos < 80))
                {
                    if(i < widthSq[newPos - 64])
                    {
                        PORTA = _BV(0) | _BV(7);
                        PORTD = _BV(0) | _BV(7);
                    }
                    else if(i == widthSq[newPos - 64])
                    {
                        PORTA = 255;
                        PORTD = 255;
                    }
                    else
                    {
                        PORTA = 0;
                        PORTC = 0;
                    }
                }
                else if(newPos >= 80 && newPos < 96)
                {
                    if(i < widthSq[16 - (newPos - 80)])
                    {
                        PORTA = _BV(0) | _BV(7);
                        PORTD = _BV(0) | _BV(7);
                    }
                    else if(i == widthSq[16 - (newPos - 80)])
                    {
                        PORTA = 255;
                        PORTD = 255;
                    }
                    else
                    {
                        PORTA = 0;
                        PORTC = 0;
                    }
                }
                else if((newPos >= 96) && (newPos < 112))
                {
                    if(i < widthSq[newPos - 96])
                    {
                        PORTA = _BV(0) | _BV(7);
                        PORTD = _BV(0) | _BV(7);
                    }
                    else if(i == widthSq[newPos - 96])
                    {
                        PORTA = 255;
                        PORTD = 255;
                    }
                    else
                    {
                        PORTA = 0;
                        PORTC = 0;
                    }
                }
                else if(newPos >= 112 && newPos < 128)
                {
                    if(i < widthSq[16 - (newPos - 112)])
                    {
                        PORTA = _BV(0) | _BV(7);
                        PORTD = _BV(0) | _BV(7);
                    }
                    else if(i == widthSq[16 - (newPos - 112)])
                    {
                        PORTA = 255;
                        PORTD = 255;
                    }
                    else
                    {
                        PORTA = 0;
                        PORTC = 0;
                    }
                }
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
            if(pos == 127)
            {
                l += 3;
            }
        }
        // wave
        for(unsigned int l = 0; l < 15000; ++l)
        {
            while(flag == false);
            flag = false;
            PORTE |= _BV(SR_DATA);
            for(uchar i = 0; i < 16; ++i)
            {
                PORTA = 0;
                PORTD = 0;
                PORTC = 0;
                PORTF = 0;
                if(pos < 64)
                {
                    setPorts(sinus[(int)(i % 32) * (int)cosinus[(int)pos % 64] / 10], 0);
                    setPorts(sinus[(sizeof(sinus) - 1) -
                            ((int)(i % 32) * (int)cosinus[(int)pos % 64] / 10)], 1);
                }
                else
                {
                    setPorts(sinus[(int)(i % 32) * (int)cosinus[(int)pos % 64] / 10], 1);
                    setPorts(sinus[(sizeof(sinus) - 1) -
                            ((int)(i % 32) * (int)cosinus[(int)pos % 64] / 10)], 0);
                }
                if((pos == 0) && (updated == false))
                {
                    if(++rep > 5)
                    {
                        uchar tmp = sinus[0];
                        for(uchar k = 0; k < (sizeof(sinus) - 1); ++k)
                        {
                            sinus[k] = sinus[k + 1];
                        }
                        sinus[sizeof(sinus) - 1] = tmp;
                        updated = true;
                        rep = 0;
                    }
                }
                else
                {
                    updated = false;
                }
                PORTE |= _BV(SR_CLK);
                PORTE &= ~(_BV(SR_CLK) | _BV(SR_DATA));
            }
        }
    }
}
