/////////////////////////////////////////////////////////////////////////////////////
///
/// \file artigo.c
/// \brief This is the source code for the RC MCU
///
/// Author(s): Michael Scherer<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@knights.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////

/*
Port      Pin       Comment        MyUse
--------------------------------------------
IO0       9         RXD1/PWM1      RX
IO1       8         TXD1/PWM2      TX
IO2       30        PWM3           RC1/side 1
IO3       21        PWM4           RC2/side 2
IO4       20        PWM5           RC3/ESTOP
IO5       29                       RC4/RCMode
IO6       4                        ESTOP
IO7       5                        RCMode
IO8       6
IO9       7         PWM6
IO10      13        PWM7
IO11      19        PWM8

IO14      16        EINT0
IO15      15        EING2

IO12      18
IO13      17

AD0       22        IO16
AD1       23        IO17
AD2       24        IO18
AD3       10        IO19
AD4       11        IO20
AD5       12        IO21
AD6       25        IO22
AD7          26        IO23
*/

#include "coridium.h"
#include "cor_init.h"
#include "cor_hwlib.h"
#include "cor_wrflash.h"

#include "mem.h"        // not used here, included only for reference
#include "string.h"
#include "printf.h"

#include <math.h>

//#define DEBUG

// CYCLE ~=~ 2ms
#define CYCLE 325

// HALF_CYCLE ~=~ 1ms
#define HALF_CYCLE CYCLE/2

// QUARTER_CYCLE ~=~ .5ms
#define QUARTER_CYCLE CYCLE/4

// CYCLE_OFFSET ~=~ 1.5ms
#define CYCLE_OFFSET HALF_CYCLE+QUARTER_CYCLE

#define LIGHTDUTY 100000

#define STATE_WAIT 0
#define STATE_COUNT 1

#define STATE_READ 2
#define STATE_SERIAL 3

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*
typedef enum Pins
{
RC1 = 30, // side 1 (right)
RC2 = 21, // side 2 (left)
RC3 = 20, // estop
RC4 = 29, // rc mode

TX = 8,
RX = 9,
ESTOP = 4
};
*/

typedef enum
{
    TX = 0,

    LIGHT = 1,

    RC1 = 4, // side 1 (right) [now forward/backward]
    RC2 = 3, // side 2 (left) [now left/right]
    RC3 = 6, // estop
    RC4 = 5, // rc mode

    ESTOP = 2,
    RCMode = 7,
    FreqPin = 8
} Pins;

typedef struct
{
    Pins pin;
    int state;
    int duty;
    int valid;
    int lastVal;
} Signal;

int flashcount = 0;

int sendBuffer[4];
char tx_buffer[6];
char num2Chars_out[2];

int gESTOP;

int getDuty(Signal* signal);
int sendSerial(int duty, int right_leftnot);
int sendESTOP(int high);
int num2Chars(int num);
char binToHexstr(char num);

char calcSum();

char statusMessage[8];

int main()
{
    int cTotal = 0;
    int cState = STATE_READ;

    char toggleOut = FALSE;

    Signal motor1;
    Signal motor2;
    Signal estop;
    Signal rcMode;

    init_coridium();

	setbaud(0, 17);
	
	
    statusMessage[0] = '#';
	statusMessage[1] = '%';
	statusMessage[2] = 0x82;



    INPUT(RC1); motor1.pin = RC1;
    INPUT(RC2); motor2.pin = RC2;
    INPUT(RC3); estop.pin = RC3;
    INPUT(RC4); rcMode.pin = RC4;


    OUTPUT(TX);
    OUTPUT(ESTOP); LOW(ESTOP);
    OUTPUT(RCMode);
    OUTPUT(FreqPin);
    OUTPUT(LIGHT);

    motor1.state = STATE_WAIT;
    motor1.duty = 0;
    motor1.valid = 0;

    motor2.state = STATE_WAIT;
    motor2.duty = 0;
    motor2.valid = 0;

    estop.state = STATE_WAIT;
    estop.duty = 0;
    estop.valid = 0;

    rcMode.state = STATE_WAIT;
    rcMode.duty = 0;
    rcMode.valid = 0;

    SLEEP(2);

    while (TRUE)
    {
        switch (cState)
        {
        case STATE_READ:
            if (toggleOut)
            {
                toggleOut = FALSE;
                HIGH(FreqPin);
            }
            else
            {
                toggleOut = TRUE;
                LOW(FreqPin);
            }

            cTotal += getDuty(&motor1);
            cTotal += getDuty(&motor2);
            cTotal += getDuty(&estop);
            cTotal += getDuty(&rcMode);

            if (cTotal > 8)
            {
                cState = STATE_SERIAL;
                cTotal = 0;
            }
            break;

        case STATE_SERIAL:
			if (estop.lastVal > 0)
            {
                sendESTOP(0);
                gESTOP = FALSE;
				
                if (rcMode.lastVal > 0)
                {
                    HIGH(LIGHT);
                    HIGH(RCMode);

                    // 11 == Manual (teleop) Mode
                    statusMessage[3] = 0x00;
                    statusMessage[4] = 0xFF;
                    statusMessage[5] = 0xFF;
                    statusMessage[6] = 0xFF;

                    calcSum();
					//statusMessage[7] = 0x84;
                    //SEROUT(TXD0,9600, 1, 3, statusMessage);
					TXD0(statusMessage[0]);
					TXD0(statusMessage[1]);
					TXD0(statusMessage[2]);
					TXD0(statusMessage[3]);
					TXD0(statusMessage[4]);
					TXD0(statusMessage[5]);
					TXD0(statusMessage[6]);
					TXD0(statusMessage[7]);
                }
                else
                {
                    if (flashcount > 0)
                    {
                        HIGH(LIGHT);
                    }
                    else
                    {
                        LOW(LIGHT);
                    }

                    if (flashcount > LIGHTDUTY)
                    {
                        flashcount = -LIGHTDUTY;
                    }

                    LOW(RCMode);

                    // 00 == Autonomous Mode
                    statusMessage[3] = 0x00;
                    statusMessage[4] = 0x00;
                    statusMessage[5] = 0x00;
                    statusMessage[6] = 0x00;

                    calcSum();
					//statusMessage[7] = 0x84;
                    //SEROUT(TXD0,9600, 1, 3, statusMessage);
					
					TXD0(statusMessage[0]);
					TXD0(statusMessage[1]);
					TXD0(statusMessage[2]);
					TXD0(statusMessage[3]);
					TXD0(statusMessage[4]);
					TXD0(statusMessage[5]);
					TXD0(statusMessage[6]);
					TXD0(statusMessage[7]);
                }
            }
            else
            {
                HIGH(LIGHT);
                HIGH(RCMode);
                sendESTOP(1);
                gESTOP = TRUE;
				
				// 11 == Manual (teleop) Mode
				statusMessage[3] = 0x00;
				statusMessage[4] = 0xFF;
				statusMessage[5] = 0xFF;
				statusMessage[6] = 0xFF;

				calcSum();
				//statusMessage[7] = 0x84;
				//SEROUT(TXD0,9600, 1, 3, statusMessage);
				TXD0(statusMessage[0]);
				TXD0(statusMessage[1]);
				TXD0(statusMessage[2]);
				TXD0(statusMessage[3]);
				TXD0(statusMessage[4]);
				TXD0(statusMessage[5]);
				TXD0(statusMessage[6]);
				TXD0(statusMessage[7]);
            }

#ifdef DEBUG
            putchar( binToHexstr((char)((motor1.lastVal >> 4) & 0x0F)) );
            putchar( binToHexstr((char)(motor1.lastVal & 0x0F)) );
            putchar(' ');
                        putchar( binToHexstr((char)((motor2.lastVal >> 4) & 0x0F)) );
            putchar( binToHexstr((char)(motor2.lastVal & 0x0F)) );
            putchar('\n');
            //printf("%d\n", estop.lastVal);
#endif
            // motor1 == forward value
            // motor2 == left/right value

            int left_motor = motor1.lastVal/2 - motor2.lastVal/2;
            int right_motor = motor1.lastVal/2 + motor2.lastVal/2;

            sendSerial(right_motor, 1);
            sendSerial(left_motor, 0);

            cState = STATE_READ;

            break;
        }
        flashcount++;
    }

    return 0;
}

int getDuty(Signal* signal)
{
    int input;
    input = IN(signal->pin);

    switch (signal->state)
    {
    case STATE_WAIT:
        signal->valid = 0;
        signal->duty = 0;

        if (input)
        {
            signal->duty++;
            signal->state = STATE_COUNT;
        }
        break;

    case STATE_COUNT:
        signal->duty++;

        if (!input)
        {
            signal->duty -= CYCLE_OFFSET;
            signal->valid = 1;
            //printf("%d: %d\n", signal->pin, signal->duty);

            signal->lastVal = signal->duty;
            signal->state = STATE_WAIT;

            /*
            duty2 = duty2 - CYCLE_OFFSET;
            duty3 = duty3 - CYCLE_OFFSET;
            duty4 = duty4 - CYCLE_OFFSET;
            */
        }
        break;
    }
    return signal->valid;
}

int sendSerial(int duty, int right_leftnot)
{
    if (gESTOP)
        duty = 0;

    tx_buffer[4] = 0x0D;
    tx_buffer[5] = '\0';
    if (duty < 0)
    {
        duty = -duty;
        num2Chars(duty);
        tx_buffer[2] = num2Chars_out[0];
        tx_buffer[3] = num2Chars_out[1];
        tx_buffer[0] = '!';
        if (right_leftnot)
            tx_buffer[1] = 'a';
        else
            tx_buffer[1] = 'b';

        SEROUT(TX, 9600, TRUE, 5, tx_buffer);
    }
    else
    {
        num2Chars(duty);
        tx_buffer[2] = num2Chars_out[0];
        tx_buffer[3] = num2Chars_out[1];
        tx_buffer[0] = '!';
        if (right_leftnot)
            tx_buffer[1] = 'A';
        else
            tx_buffer[1] = 'B';

        SEROUT(TX, 9600, TRUE, 5, tx_buffer);
    }
    //printf("Serial: %d: %s\n", right_leftnot, tx_buffer);
    return 1;
}

int sendESTOP(int high)
{
    //if (high)
    //{
    //    HIGH(ESTOP);
    //}
    //else
    //{
    //    LOW(ESTOP);
    //}
    return 1;
}

int num2Chars(int num)
{
    int tmp=num;

    tmp &= 0xF0;
    tmp = tmp >> 4;
    if (tmp > 9)
        tmp += 0x37;
    else
        tmp += '0';
    //tmp = tmp << 4;

    num &= 0x0F;
    if (num > 9)
        num += 0x37;
    else
        num += '0';
    //num |= tmp;

    num2Chars_out[0] = (char)tmp;
    num2Chars_out[1] = (char)num;
    return 1;
}

char binToHexstr(char num)
{
    if (num > 9)
        return num + 0x37;
    else
        return num + '0';
}

char calcSum()
{
	statusMessage[7] = statusMessage[6] + statusMessage[5]
						+ statusMessage[4] + statusMessage[3]
						+ statusMessage[2] + statusMessage[1]
						+ statusMessage[0];
}

